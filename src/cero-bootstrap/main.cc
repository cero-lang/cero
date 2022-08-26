// SPDX-License-Identifier: Apache-2.0

#include "cero-codegen-llvm.hh"
#include "cero-concrete-syntax-tree.hh"

#include <format>
#include <llvm/Support/InitLLVM.h>

#include <future>
#include <queue>
#include <string>
#include <vector>

#ifdef WIN32
#include <Windows.h>
#pragma warning(disable : 6387)
#endif

namespace Cero {

// This is a helper function so that we can use std::async to call the lexer in
// a separate thread. This is necessary because the lexer is not thread-safe.
// TODO: Move this in cero-lexer.hh

auto tokenize(const std::string_view s) -> std::vector<Token>
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

  llvm::InitLLVM init_llvm(argc, argv);
  llvm::errs().tie(&llvm::outs());
  // FIXME: LLVM doesn't accept our input.
  llvm::cl::opt<std::string> sources(llvm::cl::Positional,
      llvm::cl::desc("source files"), llvm::cl::Required);
  llvm::cl::ParseCommandLineOptions(argc, argv);
  // FIXME: This is a hack. See above.
  std::string source = {
    "main.cero"
  };

  Cero::context = std::make_unique<llvm::LLVMContext>();
  Cero::module = std::make_unique<llvm::Module>("Cero LLVM", *Cero::context);
  Cero::builder = std::make_unique<llvm::IRBuilder<>>(*Cero::context);

  // std::ifstream actually copies the data when input.read() is called. This is
  // a problem when the input is a large file. The solution is to use
  // MapViewOfFile — mmap on Linux — where the data will only be read from disk
  // when we access the virtual memory that the pointer points to.

#ifdef WIN32

  // This creates a "view" of the file but doesn't read data from the file.
  auto file = CreateFileA(source.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  auto file_map = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
  auto file_base = static_cast<char *>(MapViewOfFile(file_map, FILE_MAP_READ, 0, 0, 0));

  // Windows EOL sequence always ends with '\r\n'.
  // map the page that is closest to the line offset into memory.
  auto line_end = strchr(file_base, '\r');
  auto line_length = line_end - file_base;
  auto line = std::string_view(file_base, line_length);

  // Create a queue of futures to be executed in parallel. this is used to
  // parallelize the lexing of the source files.
  const auto number_of_threads = std::thread::hardware_concurrency();
  std::queue<std::future<std::vector<Cero::Token>>> futures;
  std::vector<std::vector<Cero::Token>> tokens;

  // Tokenize the source file.
  while (line_end != nullptr) {
    if (futures.size() >= number_of_threads)
      tokens.push_back(futures.front().get()), futures.pop();
    futures.emplace(std::async(std::launch::async, Cero::tokenize, line));
    // Advance the pointer to the next line.
    file_base = line_end + 0x02;
    line_end = strchr(file_base, '\r');
    line_length = line_end - file_base;
    line = std::string_view(file_base, line_length);
  }

  // Wait for all the futures to complete. This is necessary because the futures
  // are executed in parallel.
  while (!futures.empty())
    tokens.push_back(futures.front().get()), futures.pop();

  // Each line has its own vector of tokens. We need to merge them into a single
  // vector. NOTE: We need a more efficient way to merge the tokens.
  std::vector<Cero::Token> all_tokens;
  for (const auto &x : tokens)
    all_tokens.insert(all_tokens.end(), x.begin(), x.end());

  // Parse the tokens. This is the main part of the compiler.
  Cero::ConcreteSyntaxTree concrete_syntax_tree(all_tokens);
  concrete_syntax_tree.create()->codegen();

  // Free up the memory that the file was mapped into.
  if (UnmapViewOfFile(file_base) == 0 || CloseHandle(file_map) == 0) {
    throw std::runtime_error("Failed to unmap the file. Error: "
        + std::to_string(GetLastError()));
  }

#endif

  Cero::module->print(llvm::errs(), nullptr);
  return 0;
}
