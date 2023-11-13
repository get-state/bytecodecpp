#include "compiler.h"
#include "scanner.h"
#include <iostream>
#include <stdexcept>

/* Compiler::Compiler(std::string const &source) { */
/*   Scanner scanner(source); */
/*   int line = -1; */
/*   for (;;) { */
/*     Token token = scanner.scanToken(); */
/*     if (token.line != line) { */
/*       std::cout << token.line << " "; */
/*       line = token.line; */
/*     } else { */
/*       std::cout << "\t| "; */
/*     } */
/*     std::cout << static_cast<int>(token.type) << " '" << token.token << "'"
 */
/*               << std::endl; */

/*     if (token.type == TokenType::EoF) */
/*       break; */
/*   } */
/* } */

void Compiler::errorAtCurrent(std::string_view message) {
  errorAt(this->parser.getCurrent(), message);
}

void Compiler::error(std::string_view message) {
  errorAt(this->parser.getPrevious(), message);
}

void Compiler::errorAt(Token token, std::string_view message) {
  std::cerr << "[line " << token.line << "] Error";

  if (token.type == TokenType::EoF) {
    std::cerr << " at end";
  } else if (token.type == TokenType::ERROR) {
    // nothing
  } else {
    std::cerr << " at '" << token.token << "'";
  }

  std::cerr << ": " << message << std::endl;
  throw std::logic_error(""); // panic and unwind to vm
}

void Compiler::advance() {
  this->parser.setPrevious(this->parser.getCurrent());

  for (;;) {
    this->parser.setCurrent(this->scanner.scanToken());
    if (parser.getCurrent().type != TokenType::ERROR) {
      break;
    }
    errorAtCurrent(this->parser.getCurrent().token);
  }
}

Compiler::Compiler(std::string const &source, Chunk chunk)
    : scanner{Scanner(source)} {
  this->advance();
  this->expression();
  consume(TokenType::EoF, "Expected end of expression.");
}
