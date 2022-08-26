// SPDX-License-Identifier: Apache-2.0

#include "cero-codegen-llvm.hh"
#include "cero-concrete-syntax-tree.hh"
#include "cero-semantic.hh"

#include <format>
#include <llvm/Support/InitLLVM.h>

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

  llvm::InitLLVM init_llvm(argc, argv);
  llvm::errs().tie(&llvm::outs());
  // See: https://llvm.org/docs/CommandLine.html
  llvm::cl::list<std::string> input_filenames("args",
      llvm::cl::desc("Input filenames"), llvm::cl::Positional,
      llvm::cl::OneOrMore);
  llvm::cl::ParseCommandLineOptions(argc, argv);

  Cero::context = std::make_unique<llvm::LLVMContext>();
  Cero::module = std::make_unique<llvm::Module>("Cero LLVM", *Cero::context);
  Cero::builder = std::make_unique<llvm::IRBuilder<>>(*Cero::context);

  // std::ifstream actually copies the data when input.read() is called. This is
  // a problem when the input is a large file. The solution is to use
  // MapViewOfFile — mmap on Linux — where the data will only be read from disk
  // when we access the virtual memory that the pointer points to.

  // TODO: Create a application programming interface like
  // https://github.com/alitrack/mman-win32. This would allow us to use mmap on
  // Windows and unify the code.

  for (const auto &input : input_filenames) {

#ifdef WIN32

    // Remarks: Address space fragmentation puts a limit on the size of the view
    // we can create since a single view requires a contiguous address range.

    const auto file = CreateFileA(input.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
      std::cerr << "Could not open file: " << input << std::endl;
      return 1;
    }

    const auto file_map = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (file_map == nullptr || GetLastError() == ERROR_ALREADY_EXISTS) {
      std::cerr << "Could not create file mapping: " << input << std::endl;
      return 1;
    }

    auto file_view = static_cast<char *>(MapViewOfFile(file_map, FILE_MAP_READ, 0, 0, 0));
    if (file_view == nullptr) {
      std::cerr << "Could not map view of file: " << input << std::endl;
      return 1;
    }

    // FIXME: Look for string-literals and comments and ensure that the EOL
    // sequence is not enclosed in them. This is not perfect, but it should work
    // for the majority of cases.
    // https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of

    auto line_end = strchr(file_view, '\n');
    auto line_length = line_end - file_view;
    auto line = std::string_view(file_view, line_length);

    // Create a queue to parallelize the lexing of the source files.
    // We only map a small portion of the file data at one time.
    std::vector<std::vector<Cero::Token>> tokens;
    std::queue<std::future<std::vector<Cero::Token>>> queue;

    // Tokenize the source file. Each thread will tokenize a small portion of
    // the file and put the tokens in a queue. The main thread will dequeue the
    // tokens and merge them into a single vector. This will allow us to
    // tokenize the entire file in parallel and ensure that the tokens are in
    // the same order as the source file.
    const auto hardware_concurrency = std::thread::hardware_concurrency();
    while (line_end != nullptr) {
      // Lock the queue if we've reached the maximum number of threads.
      // This will prevent the queue from getting too large.
      if (queue.size() >= hardware_concurrency)
        tokens.push_back(queue.front().get()), queue.pop();
      queue.emplace(std::async(std::launch::async, Cero::tokenize, line));

      // Cute trick: We can advance the pointer to the next line by adding one
      // byte to the pointer. This is because the line ends with either '\r\n'
      // or '\n'.
      file_view = line_end + 0x01;
      line_end = strchr(file_view, '\n');
      line_length = line_end - file_view;
      line = std::string_view(file_view, line_length);
    }

    // Wait for all the threads to finish. We can't use a queue because the
    // threads are not guaranteed to finish in the same order as they were
    // started.
    while (!queue.empty())
      tokens.push_back(queue.front().get()), queue.pop();

    // Merge the tokens into a single vector.
    std::vector<Cero::Token> all_tokens;
    for (const auto &tokens_per_line : tokens) {
      all_tokens.insert(all_tokens.end(), tokens_per_line.begin(),
          tokens_per_line.end());
    }

    // Clean up. We don't need the file anymore.
    if (UnmapViewOfFile(file_view) == 0 || CloseHandle(file_map) == 0 || CloseHandle(file) == 0) {
      throw std::runtime_error("Failed to unmap the file. Error: "
          + std::to_string(GetLastError()));
    }

    // Parse the tokens. This is the main part of the compiler.
    Cero::ConcreteSyntaxTree concrete_syntax_tree(all_tokens);

#endif

  }

  Cero::module->print(llvm::errs(), nullptr);
  return 0;
}
