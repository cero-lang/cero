// SPDX-License-Identifier: Apache-2.0

#include "cero-codegen-llvm.hh"
#include "cero-concrete-syntax-tree.hh"
#include "cero-just-in-time.hh"
#include "cero-lexer.hh"

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/raw_ostream.h>

#include <fstream>
#include <future>
#include <queue>

namespace Cero {

// This is a helper function for std::async to call the lexer in
// a separate thread. TODO: Move this in cero-lexer.hh

auto tokenize(const std::string_view &s) -> std::vector<Token>
{
  Lexer lexer;
  return lexer.lex(s);
}

}

auto main(int argc, char *argv[]) -> int
{
  llvm::setBugReportMsg("Cero crashed. Please report the bug to"
                        " https://github.com/cero-lang/cero/issues "
                        "with the stack trace.\n");

  // clang-format off

  llvm::InitLLVM init_llvm(argc, argv);
  llvm::errs().tie(&llvm::outs());
  llvm::cl::list<std::string> input_files(
    "args", llvm::cl::desc("<input files>"), llvm::cl::Positional, llvm::cl::OneOrMore);
  llvm::cl::ParseCommandLineOptions(argc, argv);

  // clang-format on

  // We need to create a cross-platform library for memory-mapped files. The few
  // libraries we found are not maintained or have performance problems.
  //   https://github.com/mandreyel/mio/issues/80

  for (const auto &file : input_files) {
    std::ifstream input(file);
    std::vector<std::vector<Cero::Token>> tokens;
    std::queue<std::future<std::vector<Cero::Token>>> queue;

    if (!input.is_open())
      return llvm::errs() << "Could not open file " << file << "\n", 1;

    // Tokenize the input file in parallel. Each thread will tokenize a line
    // and put the tokens in a queue. The main thread will dequeue the
    // tokens and merge them into a single inversed vector.

    for (std::string line; getline(input, line);) {
      if (queue.size() >= std::thread::hardware_concurrency())
        tokens.push_back(queue.front().get()), queue.pop();
      queue.emplace(async(std::launch::async, Cero::tokenize, line));
    } while (!queue.empty()) tokens.push_back(queue.front().get()), queue.pop();

    std::vector<Cero::Token> all_tokens;
    for (const auto &tokens_per_line : tokens)
      all_tokens.insert(all_tokens.end(), tokens_per_line.begin(), tokens_per_line.end());
    all_tokens.emplace_back(Cero::Token::Kind::END), std::ranges::reverse(all_tokens);

    Cero::context = std::make_unique<llvm::LLVMContext>();
    Cero::module  = std::make_unique<llvm::Module>("Cero LLVM", *Cero::context);
    Cero::builder = std::make_unique<llvm::IRBuilder<>>(*Cero::context);
    Cero::ConcreteSyntaxTree concrete_syntax_tree(all_tokens);
  }

  Cero::JIT jit;
  return 0;
}
