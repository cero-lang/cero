// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <map>

#include <llvm/ADT/APFloat.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace Cero {

extern std::unique_ptr<llvm::LLVMContext> context;
extern std::unique_ptr<llvm::Module> module;
extern std::unique_ptr<llvm::IRBuilder<>> builder;
extern std::map<std::string, llvm::Value *> named_values;

} // namespace Cero
