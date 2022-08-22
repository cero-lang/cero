// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-ast.hh"

#include <memory>

namespace Cero {

class Parser {
public:
  Parser(std::vector<std::string> &source)
      : tokens(source)
  {
  }

  // clang-format off
  auto parse() -> std::unique_ptr<AST>;
  auto parseFunctionDeclaration(const Token &name) -> std::unique_ptr<FunctionDeclaration>;
  // clang-format on

private:
  Lexer tokens;
};

} // namespace Cero
