#ifndef clox_compiler_h
#define clox_compiler_h
#include "chunk.h"
#include "scanner.h"
#include <string>

class Parser
{
public:
	Parser() = delete;
	
	Parser(Token current, Token previous) : current{current}, previous{previous}{

	}

private:
	/* data */
	Token current;
	Token previous;
};

class Compiler {
public:
  Compiler() = delete;

  explicit Compiler(std::string const& source);

  explicit Compiler(std::string const& source, Chunk chunk);

  void advance() {
	  
  }

  bool isOk();

private:
  /* data */
  Parser parser;
};

#endif /* ifndef clox_compiler_h */
