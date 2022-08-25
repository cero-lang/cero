// SPDX-License-Identifier: Apache-2.0

#include "cero-concrete-syntax-tree.hh"

#include "cero-lexer.hh"

#include <memory>

namespace Cero {

auto ConcreteSyntaxTree::create() -> std::unique_ptr<AbstractSyntaxTree>
{
  auto root = std::make_unique<AbstractSyntaxTree>();

  // Iterate over the tokens and create the abstract syntax tree. The tokens
  // were created in the order they are encountered in the sourcefile.
  for (const auto &token : m_tokens.get_tokens()) {
    auto node = create_function_definition(token);
    if (node)
      root->add_node(std::move(node.value()));
  }
  return root;
}

auto ConcreteSyntaxTree::create_function_definition(const Token &token) const -> std::optional<std::unique_ptr<AbstractSyntaxTree>>
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
