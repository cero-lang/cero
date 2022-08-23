// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-ast.hh"

#include <memory>

namespace Cero {

class Parser {
public:
  Parser(std::vector<std::string> &source)
      : m_tokens(source)
  {
  }

  // clang-format off

  auto parse() -> void;
  [[nodiscard]] auto parse_function_definition(const Token &token) const -> std::unique_ptr<FunctionDefinition>;

  // clang-format on

private:
  Lexer m_tokens;
};

} // namespace Cero
