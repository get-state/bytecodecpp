#include "scanner.h"

Scanner::Scanner(std::string_view const &source) : source{source} {}

bool Scanner::isAtEnd() { return this->source[current] == '\0'; }

char Scanner::advance() {
  ++this->current;
  return this->source[current - 1];
}

char Scanner::peek() { return this->source[current]; }

char Scanner::peekNext() {
	if (isAtEnd()) return '\0';	
	return this->source[current+1]; 
}

bool Scanner::match(char expected) {
  if (isAtEnd())
    return false;
  if (this->source[current] != expected)
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
    char c = peek();
    if (isspace(c)) {
      if (c == '\n')
        ++this->line;
      advance();
    } else if (c == '/') {
      if (peekNext() == '/') {
        while (peek() != '\n' && !isAtEnd())
          advance();
      }
    } else {
      return;
    }
  }
}

Token Scanner::scanToken() {
  this->skipWhitespace();
  this->start = this->current;

  if (isAtEnd())
    return makeToken(TokenType::EoF);

  char c = advance();

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
    return makeToken(match('!') ? TokenType::BANG_EQUAL : TokenType::BANG);
  case '=':
    return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
  case '<':
    return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
  case '>':
    return makeToken(match('=') ? TokenType::GREATER_EQUAL
                                : TokenType::GREATER);
  }

  return errorToken("Unexpected Character.");
}
