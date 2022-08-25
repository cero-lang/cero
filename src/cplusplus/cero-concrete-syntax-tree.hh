// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-abstract-syntax-tree.hh"
#include "cero-lexer.hh"

namespace Cero {

class ConcreteSyntaxTree {
public:
  ConcreteSyntaxTree(std::vector<std::string> &source)
      : m_tokens(source)
  {
  }

  auto create() -> std::unique_ptr<AbstractSyntaxTree>;
  auto create_function_definition(const Token &token) const -> std::optional<std::unique_ptr<AbstractSyntaxTree>>;

private:
  Lexer m_tokens;
};

} // namespace Cero
