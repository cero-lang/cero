// SPDX-License-Identifier: Apache-2.0

#include <cero-lexer/cero-lexer.hh>

#include <magic_enum.hpp>

#include <iomanip>
#include <iostream>

auto operator<<(std::ostream &os, const Cero::Token::Kind &kind) -> std::ostream &
{
  return os << magic_enum::enum_name(kind);
}

auto main([[maybe_unused]] const int argc, [[maybe_unused]] char *argv[]) -> int
{
  const auto source = "auto";

  Cero::Lexer lex(source);
  for (auto token = lex.next(); token.kind() != Cero::Token::Kind::End
       && token.kind() != Cero::Token::Kind::Unexpected;
       token = lex.next()) {
    std::cout << token.kind() << " : " << token.string() << "\n";
  }

  return 0;
}
