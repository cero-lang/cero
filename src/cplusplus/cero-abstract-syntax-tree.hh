// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-abstract-syntax-tree-visitor.hh"

#include <llvm/IR/Value.h>

#include <string>
#include <vector>

namespace Cero {

class AbstractSyntaxTree {
public:
  virtual ~AbstractSyntaxTree() = default;

  virtual auto visit(AbstractSyntaxTreeVisitor *visitor) const -> void { }
  virtual auto codegen() -> llvm::Value *;

  auto add_node(std::unique_ptr<AbstractSyntaxTree> s) -> void;
  auto get_nodes() const -> const std::vector<std::unique_ptr<AbstractSyntaxTree>> &;

private:
  std::vector<std::unique_ptr<AbstractSyntaxTree>> m_child;
};

class FunctionDefinition : public AbstractSyntaxTree {
public:
  FunctionDefinition(std::string name)
      : m_name(std::move(name))
  {
  }

  auto visit(AbstractSyntaxTreeVisitor *visitor) const -> void override
  {
    visitor->visit(this);
  }

  auto codegen() -> llvm::Value * override;

private:
  std::string m_name;
};

} // namespace Cero
