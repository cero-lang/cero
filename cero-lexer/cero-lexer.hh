// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>
#include <unordered_map>

namespace Cero {

struct Token {
  enum struct Kind {
    Identifier,
    Number,
    Auto,
    End,
    Unexpected,
  };

  inline static std::unordered_map<std::string, Kind> keywords{{"auto", Kind::Auto}};

  Token(const Kind kind) noexcept : m_kind{kind} {}
  Token(const Kind kind, std::string string) noexcept : m_kind{kind}, m_string{std::move(string)} {}
  Token(const Kind kind, const double value) noexcept : m_kind{kind}, m_value{value} {}

  [[nodiscard]] auto kind() const -> Kind { return m_kind; }
  [[nodiscard]] auto string() const -> std::string { return m_string; }
  [[nodiscard]] auto value() const -> double { return m_value; }

private:
  Kind m_kind;
  std::string m_string{};
  double m_value{};
};

class Lexer {
public:
  Lexer(const std::string_view string_view = "") : m_string_view{string_view} {}

  [[nodiscard]] auto next() -> Token;
  [[nodiscard]] auto read() -> char;
  [[nodiscard]] auto string() const -> std::string_view { return m_string_view; }

private:
  std::string_view m_string_view;
  std::size_t m_position{};
};

} // namespace Cero
