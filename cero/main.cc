// SPDX-License-Identifier: Apache-2.0

#include "cero-codegen-llvm.hh"
#include "cero-parse.hh"

#include <fstream>
#include <string>
#include <vector>

auto main([[maybe_unused]] const int argc, [[maybe_unused]] char *argv[]) -> int
{
  std::ifstream read("main.cero");
  std::vector<std::string> source;
  std::string line;

  while (std::getline(read, line))
    source.push_back(line);

  Cero::Parser parser(source);
  Cero::context = std::make_unique<llvm::LLVMContext>();
  Cero::module = std::make_unique<llvm::Module>("Cero LLVM", *Cero::context);
  Cero::builder = std::make_unique<llvm::IRBuilder<>>(*Cero::context);

  parser.parse();
  // TODO: Jit layer and function pointer to main in cero.
  Cero::module->print(llvm::errs(), nullptr);
  return 0;
}
