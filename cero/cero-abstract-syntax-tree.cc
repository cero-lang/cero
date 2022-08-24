// SPDX-License-Identifier: Apache-2.0

#include "cero-abstract-syntax-tree.hh"

#include "cero-codegen-llvm.hh"

#include <llvm/IR/DerivedTypes.h>

namespace Cero {

auto AbstractSyntaxTree::codegen() -> llvm::Value *
{
  // Emit the code for the abstract syntax tree. This will generate the LLVM IR
  // for the program.
  for (auto &node : get_nodes())
    node->codegen();
  return nullptr;
}

auto AbstractSyntaxTree::add_node(std::unique_ptr<AbstractSyntaxTree> s) -> void
{
  m_child.push_back(std::move(s));
}

auto AbstractSyntaxTree::get_nodes() const -> const std::vector<std::unique_ptr<AbstractSyntaxTree>> &
{
  return m_child;
}

auto FunctionDefinition::codegen() -> llvm::Value*
{
  // Void type for the return value.
  const auto fn_type = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);

  // The type, linkage, name to use and which module to insert the function
  // into.
  return llvm::Function::Create(fn_type, llvm::Function::ExternalLinkage, m_name, module.get());
}

} // namespace Cero
