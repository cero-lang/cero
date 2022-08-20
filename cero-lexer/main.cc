// SPDX-License-Identifier: GPL-3.0-or-later

#include "cero-lexer.hh"

#include <magic_enum.hpp>

#include <iomanip>
#include <iostream>

auto operator<<(std::ostream &os, const Cero::Token::Kind &kind) -> std::ostream&
{
  return os << magic_enum::enum_name(kind);
}

auto main(const int argc, char *argv[]) noexcept -> int
{
  const auto source = "auto";

  Cero::Lexer lex(source);
  for (auto token = lex.next(); token.kind() != Cero::Token::Kind::End && token.kind() != Cero::Token::Kind::Unexpected; token = lex.next())
    std::cout << std::setw(12) << "\n" << token.kind() << " : " << token.string();

  return 0;
}
