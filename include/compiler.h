#ifndef clox_compiler_h
#define clox_compiler_h
#include "chunk.h"
#include "scanner.h"
#include <string>

// A small class to hold the current and previous tokens. Managed by the Compiler.
class Parser {
public:
  Parser() = delete;

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

// Glues together the frontend and backend.
// the compiler grabs a token from the scanner and manages the parser's state,
// in  case of any errors while parsing, we have a "Compiler" invariant 
// and the Compiler constructor will throw.
class Compiler {
public:
  Compiler() = delete;

  explicit Compiler(std::string const &source);

  explicit Compiler(std::string const &source, Chunk chunk);

private:

  void advance();

  void errorAtCurrent(std::string_view message);

  void error(std::string_view message);

  void errorAt(Token token, std::string_view message);

  /* data */
  Parser parser;
  Scanner scanner;
};

#endif /* ifndef clox_compiler_h */
