// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-ast.hh"

#include <memory>

namespace Cero {

class Parser : public Lexer {
public:
  Parser();
  std::unique_ptr<FunctionDeclaration> ParseFunctionDeclaration(const Token &token);
};

} // namespace Cero
