//! SPDX-License-Identifier: Apache-2.0

//! \class Cero::LexicalAnalysis
//!
//!
//! \details In Cero, lexical analysis is the process of converting a sequence of characters into a
//!         sequence of tokens (strings with an assigned and thus identified meaning). A program
//!         that performs lexical analysis may be termed a lexer, tokenizer, or scanner, though
//!         scanner is also a term for the first stage of a lexer.
//!
//! \cpp
//!
//! #include <cero/lexical-analysis.hh>
//!
//! int main()
//! {
//!   [[maybe_unused]] const auto  a = LexicalAnalysis { "my_token" };  // const LexicalAnalysis
//!   [[maybe_unused]] const auto &b = a;                               // const LexicalAnalysis&
//!
//!   return 0;
//! }
//!
//! \ecpp
//!
//!
//! \fn Cero::LexicalAnalysis::LexicalAnalysis(const std::string &stream)
//!
//! \brief Convert a sequence of characters into a sequence of \ref Cero::Token from the given
//! stream.
//!
//! \details Iters through the given stream and converts it into a sequence of tokens. Tokens are
//!         wrapped in an \ref std::optional to indicate whether or not the token is valid. The
//!         lexical analysis is aborted should \ref std::optional return \ref std::nullopt_t at any
//!         point.
//!
//! \note \ref std::optional is initialized with \ref std::nullopt_t to indicate that the token is
//!       invalid before the lexical analysis is performed. This is done to prevent the use of
//!       uninitialized variables. The \ref std::optional is then initialized with the token if the
//!       lexical analysis is successful.
//!
//! \param stream The stream to be converted into a sequence of tokens.
//! \return A sequence of tokens.
//!
//!
//! \fn Cero::LexicalAnalysis::next() const
//!
//! \brief Advances the stream to the next token.
//!
//! \note The cursor is declared as mutable to allow the stream to be iterated through.
//!       However, the stream is not modified in any way therefore next() is declared as const
//!       to indicate that the stream is not modified.
//!
//!
//! \fn Cero::LexicalAnalysis::keywords() const
//!
//! \brief Returns either a keywords or an identifier.
//!
//! \detail The keyword() method is used to determine whether the next token is a keyword or an
//!         identifier by checking the next token against the list of keywords enumerated in
//!         cero-token.def.
//!
//! \note Non-alphanumeric characters are allowed in identifiers. However, they are not allowed in
//!       keywords. This is because keywords are used to identify the type of token. For example,
//!       the keyword `int` is used to identify the token as an integer. If non-alphanumeric
//!       characters were allowed in keywords, the keyword `int32_t` would be interpreted as the
//!       keyword `int` followed by the identifier `32_t`. This would cause the lexical analysis to
//!       fail.
//!       <br><br>
//!       In constrast, non-alphanumeric characters are allowed in identifiers because they are
//!       used to identify the name of a variable. For example, the identifier `my_variable` is used
//!       to identify the variable `my_variable`. If non-alphanumeric characters were not allowed
//!       in identifiers, the identifier `my_variable` would be interpreted as the identifier `my`
//!       followed by the identifier `variable`. This would cause the lexical analysis to fail.
//!
//! \return An \ref std::optional containing a \ref Cero::Token if the token is a keyword or an
//!         identifier, otherwise \ref std::nullopt_t.
//!
//!
//! \fn Cero::LexicalAnalysis::symbols() const
//!
//! \brief Returns a symbol.
//!
//! \detail The symbol() method is used to determine whether the next token is a symbol by checking
//!         the next token against the list of symbols enumerated in cero-token.def.
//!
//! \note Symbols are used to identify the type of token. For example, the symbol "(" is used
//!       to identify the token as a left parenthesis.
//!
//! \return An \ref std::optional containing a \ref Cero::Token if the token is a symbol, otherwise
//!         \ref std::nullopt_t.

#pragma once

#include "cero-token.hh"

#include <llvm/ADT/StringSwitch.h>

#include <vector>

namespace Cero {

class LexicalAnalysis {
using matches = llvm::StringSwitch<Token>;

public:
  LexicalAnalysis(std::string& stream);

protected:
                auto next()     const -> void;
  [[nodiscard]] auto read()     const -> char;
  [[nodiscard]] auto keywords() const -> std::optional<Token>;
  [[nodiscard]] auto symbols()  const -> std::optional<Token>;

private:
  std::vector<Token> m_tokens;
  mutable std::pair<std::string_view, std::size_t> m_stream;
};

} // namespace cero
