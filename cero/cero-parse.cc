// SPDX-License-Identifier: Apache-2.0

#include "cero-parse.hh"
#include "cero-ast.hh"

namespace Cero {

auto Parser::parse() -> std::unique_ptr<AST>
{
  std::unique_ptr<AST> ast = std::make_unique<AST>();

  if (tokens.lex().kind() == Token::Kind::Auto) {
    if (tokens.lex().kind() == Token::Kind::Identifier) {
      auto name = tokens.pop().string();
      // ParseFunctionDeclaration ...
    }
  }

  return ast;
}

auto Parser::parseFunctionDeclaration(const Token &name) -> std::unique_ptr<FunctionDeclaration>
{
  return std::make_unique<FunctionDeclaration>(name);
}

} // namespace Cero
