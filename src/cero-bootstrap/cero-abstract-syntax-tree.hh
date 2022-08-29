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

  virtual auto visit(AbstractSyntaxTreeVisitor *visitor) const -> void ;
  virtual auto codegen() -> void;

  auto add_node(std::unique_ptr<AbstractSyntaxTree> s) -> void;
  [[nodiscard]] auto get_nodes() const -> const std::vector<std::unique_ptr<AbstractSyntaxTree>> &;

private:
  std::vector<std::unique_ptr<AbstractSyntaxTree>> m_child;
};

class FunctionDefinition : public AbstractSyntaxTree {
public:
  FunctionDefinition(std::string name, std::pair<bool, std::string> is_namespace)
      : m_name(std::move(name))
      , m_namespace(std::move(is_namespace))
  {
  }

  auto visit(AbstractSyntaxTreeVisitor *visitor) const -> void override { visitor->visit(this); }
  auto codegen() -> void override;

private:
  std::string m_name;
  std::vector<llvm::Type *> m_param;
  std::pair<bool, std::string> m_namespace;
};

} // namespace Cero
