// SPDX-License-Identifier: Apache-2.0

#include "cero-ast.hh"

#include "cero-codegen-llvm.hh"

#include <llvm/IR/DerivedTypes.h>

namespace Cero {

auto FunctionDefinition::codegen() -> llvm::Function *
{
  // Void type for the return value.
  const auto fn_type = llvm::FunctionType::get(llvm::Type::getVoidTy(*context),
      false);

  auto name = m_token.string();

  // The type, linkage, name to use and which module to insert the function
  // into.
  const auto fn = llvm::Function::Create(fn_type,
      llvm::Function::ExternalLinkage, m_token.string(), module.get());

  return fn;
}

} // namespace Cero
