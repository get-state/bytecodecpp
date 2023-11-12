#ifndef clox_compiler_h
#define clox_compiler_h
#include <string>

class Compiler {
public:
  Compiler() = delete;
  explicit Compiler(std::string const &source);

private:
  /* data */
};

#endif /* ifndef clox_compiler_h */
