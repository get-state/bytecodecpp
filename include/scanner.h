#include "common.h"
#ifndef clox_scanner_h
#define clox_scanner_h

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

#include <string>
#include <string_view>

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,
  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,
  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FOR,
  FUN,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,
  // Errors.
  ERROR,
  EoF
};

// returns the total amount of TokenTypes in the enum class.
constexpr int TokenTypeCardinality() {
  return static_cast<int>(TokenType::EoF) + 1;
}

typedef struct {
  TokenType type;
  std::string token;
  int line;
} Token;

class Scanner {
public:
  Scanner() = delete;
  explicit Scanner(std::string const &source);

  Token scanToken();

private:
  /* data */
  std::string const &source;
  std::string_view currentToken;
  size_t start{0};
  size_t current{0};
  size_t line{1};

  /* methods */
  [[nodiscard]] bool isAtEnd();

  [[nodiscard]] Token makeToken(TokenType type);

  [[nodiscard]] Token errorToken(std::string const &message);

  [[nodiscard]] Token string();

  char advance();

  [[nodiscard]] char peek();

  [[nodiscard]] char peekNext();

  void skipWhitespace();

  [[nodiscard]] Token number();

  [[nodiscard]] Token identifier();

  [[nodiscard]] TokenType identifierType();

  TokenType checkKeyword(size_t start, size_t length, std::string_view rest,
                         TokenType type);

  [[nodiscard]] bool match(char expected);
};

#endif /* ifndef clox_scanner_h */
