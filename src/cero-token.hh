//! SPDX-License-Identifier: Apache-2.0

//! \class Cero::Token
//!
//! \detail In Cero, tokens are used to represent keywords, identifiers, literals, operators,
//!         and delimiters. They are produced during the lexical analysis phase of the compiler.
//!
//! \cpp
//!
//! #include <cero/token.hh>
//!
//! int main()
//! {
//!   [[maybe_unused]] const auto  a = Token { Token::Kind::END, "my_token" };  // const Token
//!   [[maybe_unused]] const auto &b = a;                                       // const Token&
//!   [[maybe_unused]] const auto  c = b();                                     // const Kind::End
//!
//!   return 0;
//! }
//!
//! \ecpp
//!
//! \out
//!
//! hello_world
//!
//! \eout
//!
//!
//! \fn Cero::Token::Token(Kind kind, std::optional<std::any> &&attribute = std::nullopt)
//!
//! \brief Constructs a token with the given kind and attribute.
//!
//! \detail The constructor can be seen as a factory function for tokens. The token is constructed
//!         with the given kind and attribute. Conveniant methods are provided to facilitate
//!         introspection.
//!
//! \param kind The token kind.
//! \param attribute The token attribute.
//! \return A token with the given kind and attribute.
//!
//!
//! \fn Cero::Token::operator ()() const
//!
//! \brief Returns the token kind.
//!
//! \detail The operator () is used to retrieve the token kind. It is a convenient way to
//!         retrieve the token kind without leaking the internal representation of the token.
//!
//! \cpp
//!
//! #include <cero/token.hh>
//!
//! int main()
//! {
//!   const auto a = Token { Token::Kind::END };
//!
//!   assert(a() == Token::Kind::END);
//!
//!   return 0;
//! }
//!
//! \ecpp
//!
//! \return The token kind.
//!
//!
//! \fn Cero::Token::operator==(const Token &lhs, const Token &rhs)
//!
//! \brief Compares two tokens for equality.
//!
//! \detail Two tokens are considered equal if they have the same kind and attribute.
//!
//! \cpp
//!
//! #include <cero/token.hh>
//!
//! int main()
//! {
//!   const auto a = Token { Token::Kind::END };
//!   const auto b = Token { Token::Kind::END };
//!
//!   assert(a == b);
//!   assert(!(a != b));
//!
//!   return 0;
//! }
//!
//! \ecpp
//!
//! \param lhs The left hand side token.
//! \param rhs The right hand side token.
//!
//! \return \c true if the tokens are equal, \c false otherwise.
//!
//!
//! \fn Cero::Token::operator!=(const Token &lhs, const Token &rhs)
//!
//! \brief Compares two tokens for inequality.
//!
//! \detail Two tokens are not considered equal if they have different kinds or attributes.
//!
//! \cpp
//!
//! #include <cero/token.hh>
//!
//! int main()
//! {
//!   const auto a = Token { Token::Kind::AUTO };
//!   const auto b = Token { Token::Kind::END  };
//!
//!   assert(a != b);
//!   assert(!(a == b));
//!
//!   return 0;
//! }
//!
//! \ecpp
//!
//! \param lhs The left hand side token.
//! \param rhs The right hand side token.
//!
//! \return \c true if the tokens are not equal, \c false otherwise.

// ReSharper disable CppClangTidyClangDiagnosticDocumentation

#pragma once

#include <any>
#include <optional>

namespace Cero {

struct Token {
  enum struct Kind : int_least8_t {
    #define CERO_TOKEN(kind) kind,
    #include "cero-token.def"
  };

  Token(Kind kind, std::optional<std::any> &&attribute = std::nullopt)
      : m_current { kind, attribute }
  {
  }

  [[nodiscard]] auto operator()() const -> Kind
  {
    return std::get<0>(m_current);
  }

  [[nodiscard]] friend auto operator==(const Token &lhs, const Token &rhs) -> bool
  {
    return lhs() == rhs();
  }

  [[nodiscard]] friend auto operator!=(const Token &lhs, const Token &rhs) -> bool
  {
    return lhs() != rhs();
  }

private:
  std::tuple<Kind, std::optional<std::any>> m_current;
};

} // namespace Cero
