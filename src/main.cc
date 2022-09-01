//! Cero's compiler driver.
//!
//! This file is the entry point for the compiler. It main purpose is to provide a simple and
//! consistent interface to all compilation tasks.
//!
//! Below is a list of the available compilation tasks:
//!   - Collect command line options.
//!   - Parse the source file.
//!
//! SPDX-License-Identifier: Apache-2.0

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/raw_ostream.h>

#include <filesystem>
#include <fstream>
#include <future>
#include <queue>

#include "cero-lexical-analysis.hh"

auto main(int argc, char *argv[]) -> int
{
  using namespace std;
  namespace fs =  filesystem;

  llvm::setBugReportMsg("Cero crashed. Please report the bug to"
                        " https://github.com/cero-lang/cero/issues "
                        "with the stack trace.\n");

  // clang-format off

  // https://llvm.org/docs/CommandLine.html
  // https://llvm.org/doxygen/classllvm_1_1InitLLVM.html

  llvm::InitLLVM init_llvm(argc, argv);
  llvm::errs().tie(&llvm::outs());
  llvm::cl::opt<bool> language_server_protocol(
    "lsp", llvm::cl::desc("Spawn a language server protocol server"), llvm::cl::init(false));
  llvm::cl::ParseCommandLineOptions(argc, argv);

  // clang-format on

  if (language_server_protocol)
    return -1; // TODO

  for (const auto &entry : fs::directory_iterator(fs::current_path().string())) {
    if (const auto &file = entry.path(); file.extension() == ".cero") {
      ifstream input(file);
      vector<vector<cero::token>> tokens;
      queue<future<vector<cero::token>>> queue;

      if (!input.is_open())
        return llvm::errs() << "Could not open file " << file.filename().string() << "\n", 1;

      //! Tokenize the input file in parallel. Each thread will tokenize a line
      //! and put the tokens in a queue. The main thread will dequeue the
      //! tokens and merge them into a single inversed vector.

      for (std::string line; getline(input, line);) {
        if (queue.size() >= std::thread::hardware_concurrency())
          tokens.push_back(queue.front().get()), queue.pop();
        queue.emplace(async(std::launch::async, cero::lexer::async, line));
      } while (!queue.empty()) tokens.push_back(queue.front().get()), queue.pop();

      std::vector<cero::token> all_tokens;
      for (const auto &tokens_per_line : tokens)
        all_tokens.insert(all_tokens.end(), tokens_per_line.begin(), tokens_per_line.end());
      all_tokens.emplace_back(cero::token::kind::END), std::ranges::reverse(all_tokens);
    }
  }

  return 0;
}
