// SPDX-License-Identifier: Apache-2.0

#include "cero-codegen-llvm.hh"

namespace Cero {

std::unique_ptr<llvm::LLVMContext> context;
std::unique_ptr<llvm::Module> module;
std::unique_ptr<llvm::IRBuilder<>> builder;
std::map<std::string, llvm::Value *> named_values;

} // namespace Cero
