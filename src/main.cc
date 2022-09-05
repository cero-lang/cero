//! \file main.cc
//!
//! # Cero's compiler driver.
//!
//! This file is the entry point for the compiler. It main purpose is to provide a simple and
//! consistent interface to all compilation tasks.
//!
//! Below is a list of the available compilation tasks:
//!   * Collect command line options.
//!   * Parse the source file.
//!
//! SPDX-License-Identifier: Apache-2.0

#include "cero-lexical-analysis.hh"

#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/CommandLine.h>

#include <filesystem>
#ifdef DEBUG
  #include <fstream>
#endif

using namespace std;
using namespace filesystem;
using namespace llvm;
using namespace cl;
//!  \namespace Cero Root namespace.
using namespace Cero;

auto main(int argc, char *argv[]) -> int
{
  setBugReportMsg("Cero crashed. Please report the bug to"
                  " https://github.com/cero-lang/cero/issues "
                  "with the stack trace.\n");

  InitLLVM init_llvm(argc, argv);
  errs().tie(&outs());

#ifdef NDEBUG
  cl::list<std::string> input_files(
  "args", desc("<input files>"), Positional, OneOrMore);
  ParseCommandLineOptions(argc, argv);
#endif

#ifdef DEBUG

  // This code isn't meant for production use. It's just a quick way to glob
  // all the files for debugging purposes.

  try {
    for (const auto &directory : directory_iterator(current_path().string())) {
      if (const auto &file = directory.path(); file.extension() == ".cero") {
        if (ifstream stream(file); stream.is_open()) {
          for (string line; getline(stream, line);) {
            LexicalAnalysis lexical_analysis(line);
          }
        }
      }
    }
  } catch (std::exception &e) {
    return errs() << "error: " << e.what(), EXIT_FAILURE;
  }

#endif

  return EXIT_SUCCESS;
}
