#ifndef clox_compiler_h
#define clox_compiler_h
#include "chunk.h"
#include "scanner.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

// A small class to hold the current and previous tokens. Managed by the
// Compiler.
class Parser {
public:
  explicit Parser() {
    Token tmp = Token{.type = TokenType::NIL, .token = "NIL", .line = 0};
    this->current = tmp;
    this->previous = tmp;
  }

  Parser(Token current, Token previous)
      : current{current}, previous{previous} {}

  [[nodiscard]] Token getPrevious() { return this->previous; }

  [[nodiscard]] Token getCurrent() { return this->current; }

  void setPrevious(Token prev) { this->previous = prev; }

  void setCurrent(Token current) { this->current = current; }

  /* void setErrorState(bool state) { this->error = state; } */

  /* bool hadError() { return this->error; } */

private:
  /* data */
  Token current;
  Token previous;
  /* bool error; */
};

// in-order, from lowest to highest.
enum class Precedence {
  NONE,
  ASSIGNMENT, // =
  OR,         // or
  AND,        // and
  EQUALITY,   // == !=
  COMPARISON, // < > <= >=
  TERM,       // + -
  FACTOR,     // * /
  UNARY,      // ! -
  CALL,       // . ()
  PRIMARY
};

// Glues together the frontend and backend.
// the compiler grabs a token from the scanner and manages the parser's state,
// in  case of any errors while parsing, we have a "Compiler" invariant
// and the Compiler constructor will throw.
class Compiler {
public:
  Compiler() = delete;

  explicit Compiler(std::string const &source);

  explicit Compiler(std::string const &source, Chunk &chunk);

private:
  typedef void (Compiler::*ParseFn)();

  struct ParseRule {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
  };

  void advance();

  void errorAtCurrent(std::string_view message);

  void error(std::string_view message);

  void errorAt(Token token, std::string_view message);

  void consume(TokenType type, std::string_view message);

  void emitByte(std::uint8_t byte);

  void emitByte(std::uint8_t byte, std::uint8_t byte2);

  void emitReturn();

  void emitConstant(Value value);

  [[nodiscard]] uint8_t makeConstant(Value value);

  void expression();

  void number();

  void grouping();

  void unary();

  void parsePrecedence(Precedence precedence);

  void binary();

  std::array<ParseRule, TokenTypeCardinality()> buildParseTable();

  ParseRule getRule(TokenType type);

  /* data */
  Parser parser;
  Scanner scanner;
  Chunk &chunk;

  // Parse table
  std::array<ParseRule, TokenTypeCardinality()> rules = {
      ParseRule{.prefix = NULL, .infix = NULL, .precedence = Precedence::NONE}};
};

#endif /* ifndef clox_compiler_h */
