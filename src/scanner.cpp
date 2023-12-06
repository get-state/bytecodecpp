#include "scanner.h"
#include <cctype>

Scanner::Scanner(std::string const &source) : source{source} {}

bool Scanner::isAtEnd() { return this->source[this->current] == '\0'; }

char Scanner::advance() {
  ++this->current;
  return this->source[this->current - 1];
}

char Scanner::peek() { return this->source[this->current]; }

char Scanner::peekNext() {
  if (isAtEnd())
    return '\0';
  return this->source[this->current + 1];
}

bool Scanner::match(char expected) {
  if (isAtEnd())
    return false;
  if (this->source[this->current] != expected)
    return false;
  ++this->current;
  return true;
}

Token Scanner::makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.token = this->source.substr(this->start, this->current - this->start);
  token.line = this->line;
  return token;
}

Token Scanner::errorToken(std::string const &message) {
  Token token;
  token.type = TokenType::ERROR;
  token.token = message;
  token.line = this->line;
  return token;
}

void Scanner::skipWhitespace() {
  for (;;) {
    const char c = peek();
    if (std::isspace(c)) {
      if (c == '\n')
        ++this->line;
      advance();
    } else if (c == '/') {
      if (peekNext() == '/') {
        while (peek() != '\n' && !isAtEnd())
          advance();
      } else {
        return;
      }
    } else {
      return;
    }
  }
}
// start, len, rest (of string), TokenType
TokenType Scanner::checkKeyword(size_t start, size_t length,
                                std::string_view rest, TokenType type) {
  auto tmp = this->source.substr(this->start + start, length);
  if (this->current - this->start == start + length && tmp == rest) {
    return type;
  }
  return TokenType::IDENTIFIER;
}

TokenType Scanner::identifierType() {
  switch (this->source[this->start]) {
  case 'a':
    return checkKeyword(1, 2, "nd", TokenType::AND);
  case 'c':
    return checkKeyword(1, 4, "class", TokenType::CLASS);
  case 'e':
    return checkKeyword(1, 3, "lse", TokenType::ELSE);
  case 'f':
    if (this->current - this->start > 1) {
      switch (this->source[this->start + 1]) {
      case 'a':
        return checkKeyword(2, 3, "lse", TokenType::FALSE);
      case 'o':
        return checkKeyword(2, 1, "r", TokenType::FOR);
      case 'u':
        return checkKeyword(2, 1, "n", TokenType::FUN);
      }
    }
    break;
  case 'i':
    return checkKeyword(1, 1, "f", TokenType::IF);
  case 'n':
    return checkKeyword(1, 2, "il", TokenType::NIL);
  case 'o':
    return checkKeyword(1, 1, "r", TokenType::OR);
  case 'p':
    return checkKeyword(1, 4, "rint", TokenType::PRINT);
  case 'r':
    return checkKeyword(1, 5, "eturn", TokenType::RETURN);
  case 's':
    return checkKeyword(1, 4, "uper", TokenType::SUPER);
  case 't':
    if (this->current - this->start > 1) {
      switch (this->source.at(this->start + 1)) {
      case 'h':
        return checkKeyword(2, 2, "is", TokenType::THIS);
      case 'r':
        return checkKeyword(2, 2, "ue", TokenType::TRUE);
      }
    }
    break;
  case 'v':
    return checkKeyword(1, 2, "ar", TokenType::VAR);
  case 'w':
    return checkKeyword(1, 4, "hile", TokenType::WHILE);
  }
  return TokenType::IDENTIFIER;
}

Token Scanner::number() {
  while (isdigit(peek()))
    advance();

  if (peek() == '.' && isdigit(peekNext())) {
    advance();

    while (isdigit(peek()))
      advance();
  }

  return makeToken(TokenType::NUMBER);
}

Token Scanner::identifier() {
  while (isalpha(peek()) || isdigit(peek()))
    advance();
  return makeToken(identifierType());
}

Token Scanner::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n')
      ++this->line;
    advance();
  }
  if (isAtEnd())
    return errorToken("Unterminated string.");

  // closing quote.
  advance();
  return makeToken(TokenType::STRING);
}

Token Scanner::scanToken() {
  this->skipWhitespace();
  this->start = this->current;

  if (isAtEnd())
    return makeToken(TokenType::EoF);

  const char c = advance();

  if (isalpha(c)) {
    return identifier();
  }

  if (isdigit(c))
    return number();

  switch (c) {
  case '(':
    return makeToken(TokenType::LEFT_PAREN);
  case ')':
    return makeToken(TokenType::RIGHT_PAREN);
  case '{':
    return makeToken(TokenType::LEFT_BRACE);
  case '}':
    return makeToken(TokenType::RIGHT_BRACE);
  case ';':
    return makeToken(TokenType::SEMICOLON);
  case ',':
    return makeToken(TokenType::COMMA);
  case '.':
    return makeToken(TokenType::DOT);
  case '-':
    return makeToken(TokenType::MINUS);
  case '+':
    return makeToken(TokenType::PLUS);
  case '/':
    return makeToken(TokenType::SLASH);
  case '*':
    return makeToken(TokenType::STAR);
  case '!':
    return makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
  case '=':
    return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
  case '<':
    return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
  case '>':
    return makeToken(match('=') ? TokenType::GREATER_EQUAL
                                : TokenType::GREATER);
  case '"':
    return string();
  }

  return errorToken("Unexpected Character.");
}
