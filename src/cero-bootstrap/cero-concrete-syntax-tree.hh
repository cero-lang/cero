// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-abstract-syntax-tree.hh"
#include "cero-lexer.hh"
#include "cero-semantic.hh"

namespace Cero {

class ConcreteSyntaxTree {
public:
  ConcreteSyntaxTree(std::vector<Token> tokens);
  auto expect(Token::Kind kind) -> Token;
  auto lookahead(unsigned long long index) -> Token;
  [[nodiscard]] auto parse_function_definition() -> void;

private:
  std::vector<Token> m_tokens;
  unsigned long long m_index;
  Token m_token;
  std::unique_ptr<AbstractSyntaxTree> m_abstract_syntax_tree;
  std::unique_ptr<Semantic> m_semantic;
};

} // namespace Cero
