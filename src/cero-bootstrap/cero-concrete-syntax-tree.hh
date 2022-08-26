// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-abstract-syntax-tree.hh"
#include "cero-lexer.hh"

namespace Cero {

class ConcreteSyntaxTree {
public:
  ConcreteSyntaxTree(std::vector<Token> tokens)
      : m_tokens(std::move(tokens))
  {
  }

  auto create() -> std::unique_ptr<AbstractSyntaxTree>;
  auto create_function_definition(const Token &token) const -> std::optional<std::unique_ptr<AbstractSyntaxTree>>;

private:
  std::vector<Token> m_tokens;
};

} // namespace Cero
