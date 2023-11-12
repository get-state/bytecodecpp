#ifndef clox_scanner_h
#define clox_scanner_h

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

  ERROR,
  EoF
};

typedef struct {
  TokenType type;
  std::string_view token;
  int line;
} Token;

class Scanner {
public:
  Scanner() = delete;
  explicit Scanner(std::string_view const& source);

  Token scanToken();

  bool isAtEnd();

  Token makeToken(TokenType type);
  
  Token errorToken(std::string const& message);
  
  char advance();
  
  char peek();

  char peekNext();
  
  void skipWhitespace();
  
  bool match(char expected);

private:
  /* data */
  std::string_view const &source;
  std::string_view currentToken;
  size_t start = 0;
  size_t current = 0;
  size_t line = 1;
};

#endif /* ifndef clox_scanner_h */
