// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-ast-visitor.hh"

#include "cero-lexer.hh"

#include <llvm/IR/Value.h>

namespace Cero {

class Ast {
public:
  virtual ~Ast() = default;
  virtual auto accept(AstVisitor *) const -> void { }

  virtual auto codegen() -> llvm::Function*
  {
    return nullptr;
  }
};

class FunctionDefinition : public Ast {
public:
  FunctionDefinition(Token token)
      : m_token(std::move(token))
  {
  }

  auto accept(AstVisitor *visitor) const -> void override
  {
    visitor->visit_function_definition(this);
  }

  auto codegen() -> llvm::Function * override;

private:
  Token m_token;
};

} // namespace Cero
