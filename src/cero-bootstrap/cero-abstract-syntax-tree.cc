// SPDX-License-Identifier: Apache-2.0

#include "cero-abstract-syntax-tree.hh"

#include "cero-codegen-llvm.hh"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Verifier.h>

namespace Cero {

auto AbstractSyntaxTree::visit(AbstractSyntaxTreeVisitor *visitor) const -> void
{
  for (auto &node : get_nodes())
    node->visit(visitor);
}

auto AbstractSyntaxTree::codegen() -> void
{
  for (auto &node : get_nodes())
    node->codegen();
}

auto AbstractSyntaxTree::add_node(std::unique_ptr<AbstractSyntaxTree> s) -> void
{
  m_child.push_back(std::move(s));
}

auto AbstractSyntaxTree::get_nodes() const -> const std::vector<std::unique_ptr<AbstractSyntaxTree>> &
{
  return m_child;
}

auto FunctionDefinition::codegen() -> void
{
  // Void type for the return value.
  const auto fn_type = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);

  // The type, linkage, name to use and which module to insert the function
  // into.
  const auto fn = llvm::Function::Create(fn_type, llvm::Function::ExternalLinkage, "main", module.get());
  const auto fn_body = llvm::BasicBlock::Create(module->getContext(), "entry", fn);

  builder->SetInsertPoint(fn_body);
  builder->CreateRet(nullptr); // Passing nullptr to CreateRet should give us ret void.

  verifyFunction(*fn);
}

} // namespace Cero
