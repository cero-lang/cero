// SPDX-License-Identifier: Apache-2.0

#include "cero-codegen-llvm.hh"
#include "cero-concrete-syntax-tree.hh"

#include <format>
#include <llvm/Support/InitLLVM.h>

#include <fstream>
#include <string>
#include <vector>

auto main(int argc, char *argv[]) -> int
{
  llvm::setBugReportMsg("Cero crashed. Please report the bug to"
                        " https://github.com/cero-lang/cero/issues "
                        "with the stack trace.\n");

  llvm::InitLLVM init_llvm(argc, argv);
  llvm::errs().tie(&llvm::outs());
  llvm::cl::opt<std::vector<std::string>> sources(llvm::cl::Positional, llvm::cl::desc("source files"), llvm::cl::Required);
  llvm::cl::ParseCommandLineOptions(argc, argv);

  Cero::context = std::make_unique<llvm::LLVMContext>();
  Cero::module  = std::make_unique<llvm::Module>("Cero LLVM", *Cero::context);
  Cero::builder = std::make_unique<llvm::IRBuilder<>>(*Cero::context);

  for (auto &source : sources) {
    if (std::ifstream istrm(source); istrm.is_open()) {
      std::vector<std::string> source;
      std::string line;
      while (std::getline(istrm, line))
        source.push_back(line);
      Cero::ConcreteSyntaxTree concrete_syntax_tree(source);
      concrete_syntax_tree.create()->codegen();
    } else {
      throw std::runtime_error(std::format("failed to read {}", source));
    }
  }

  Cero::module->print(llvm::errs(), nullptr);
  return 0;
}
