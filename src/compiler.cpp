#include "compiler.h"
#include "scanner.h"
#include <iostream>

Compiler::Compiler(std::string const &source) {
  Scanner scanner(source);
  int line = -1;
  for (;;) {
    Token token = scanner.scanToken();
    if (token.line != line) {
      std::cout << token.line << " ";
      line = token.line;
    } else {
      std::cout << "\t| ";
    }
    std::cout << static_cast<int>(token.type) << " '" << token.token << "'"
              << std::endl;

    if (token.type == TOKEN_EOF)
      break;
  }
}
