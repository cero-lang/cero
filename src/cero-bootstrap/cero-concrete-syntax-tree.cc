// SPDX-License-Identifier: Apache-2.0

#include "cero-concrete-syntax-tree.hh"

#include "cero-lexer.hh"
#include "cero-semantic.hh"

#include <memory>

namespace Cero {

ConcreteSyntaxTree::ConcreteSyntaxTree(std::vector<Token> tokens)
    : m_tokens(std::move(tokens))
{
  const auto semantic = std::make_unique<Semantic>();
  const auto abstract_syntax_tree = std::make_unique<AbstractSyntaxTree>();

  for (const auto &token : m_tokens) {
    if (auto node = parse_function_definition(token))
      abstract_syntax_tree->add_node(std::move(node.value()));
  }

  abstract_syntax_tree->visit(semantic.get());
  abstract_syntax_tree->codegen();
}

auto ConcreteSyntaxTree::parse_function_definition(const Token &token) const -> std::optional<std::unique_ptr<AbstractSyntaxTree>>
{
  // TODO: *(&token + 1) is a hack to get the next token. We should use a proper
  // peek function.
  if (token.kind() == Token::Kind::Auto) {
    if (const auto peek = *(&token + 1); peek.kind() == Token::Kind::Identifier)
      return std::make_unique<FunctionDefinition>(peek.string());
  }

  return std::nullopt;
}

} // namespace Cero
