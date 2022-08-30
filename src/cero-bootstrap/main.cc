// SPDX-License-Identifier: Apache-2.0

#include "cero-codegen-llvm.hh"
#include "cero-concrete-syntax-tree.hh"
#include "cero-just-in-time.hh"

#include <llvm/Support/InitLLVM.h>
#include <llvm/ADT/ScopeExit.h>

#include <format>
#include <future>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#ifdef WIN32
  #include <Windows.h>
  #pragma warning(disable : 6387)
  #pragma warning(disable : 6001) // Using uninitialized memory. Trigger False positives.
#endif

namespace Cero {

// This is a helper function so that we can use std::async to call the lexer in
// a separate thread. This is necessary because the lexer is not thread-safe.
// TODO: Move this in cero-lexer.hh

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

  // see: https://llvm.org/docs/GettingStarted.html#initializing-llvm
  // and: https://llvm.org/docs/CommandLine.html

  llvm::InitLLVM init_llvm(argc, argv);
  llvm::errs().tie(&llvm::outs());
  llvm::cl::list<std::string> input_filenames("args", llvm::cl::desc("<input files>"), llvm::cl::Positional, llvm::cl::OneOrMore);
  llvm::cl::ParseCommandLineOptions(argc, argv);

  for (const auto &input : input_filenames) {

#ifdef WIN32

    // Address space fragmentation puts a limit on the size we can create since
    // a single view requires a contiguous address range.

    HANDLE file;
    HANDLE file_map;
    PCHAR  file_view;

    try {
      file = CreateFileA(input.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
      file_map = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
      file_view = static_cast<char *>(MapViewOfFile(file_map, FILE_MAP_READ, 0, 0, 0));
    } catch (...) {
      llvm::errs() << "Error: File mapping failed.\n";
      _exit(-1);
    }

    auto scope_exit = [file, file_map, file_view] {
      if (UnmapViewOfFile(file_view) == 0 || CloseHandle(file_map) == 0 || CloseHandle(file) == 0) {
        llvm::errs() << "Error: File unmapping failed.\n";
        _exit(-1);
      }
    }; auto scope_error = llvm::make_scope_exit([&] { scope_exit(); });

    // Tokenize the input file in parallel. Each thread will tokenize a line
    // and put the tokens in a queue. The main thread will dequeue the
    // tokens and merge them into a single inversed vector.

    auto line = strchr(file_view, '\n');
    auto line_len = line - file_view;
    auto line_view = std::string_view(file_view, line_len);

    std::vector<std::vector<Cero::Token>> tokens;
    std::queue<std::future<std::vector<Cero::Token>>> queue;

    while (line != nullptr) {
      if (queue.size() >= std::thread::hardware_concurrency())
        tokens.push_back(queue.front().get()), queue.pop();
      queue.emplace(std::async(std::launch::async, Cero::tokenize, line_view));

      file_view = line + 0x01;
      line = strchr(file_view, '\n');
      line_len = line - file_view;
      line_view = std::string_view(file_view, line_len);
    }

    while (!queue.empty())
      tokens.push_back(queue.front().get()), queue.pop();

    std::vector<Cero::Token> all_tokens;
    for (const auto &tokens_per_line : tokens)
      all_tokens.insert(all_tokens.end(), tokens_per_line.begin(), tokens_per_line.end());
    all_tokens.emplace_back(Cero::Token::Kind::END), std::ranges::reverse(all_tokens);

    scope_exit();
    scope_error.release();

#endif

    Cero::context = std::make_unique<llvm::LLVMContext>();
    Cero::module  = std::make_unique<llvm::Module>("Cero LLVM", *Cero::context);
    Cero::builder = std::make_unique<llvm::IRBuilder<>>(*Cero::context);
    Cero::ConcreteSyntaxTree concrete_syntax_tree(all_tokens);
    Cero::JIT jit;

  }

  return 0;
}
