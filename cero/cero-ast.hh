// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-ast-visitor.hh"

#include "cero-lexer.hh"

namespace Cero {

class AST {
public:
  virtual ~AST() = default;
  virtual void Accept(ASTVisitor*) const {}
};

class FunctionDeclaration : public AST {
public:
  FunctionDeclaration(Token token) : token(token) {}

  void Accept(ASTVisitor *visitor) const override {
    visitor->VisitFunctionDeclaration(this);
  }

private:
  Token token;
};

} // namespace Cero
