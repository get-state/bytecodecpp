#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>

#define PROJECT_NAME "bcpplox"

static void repl() {
  std::string line;
  VM vm;
  for (;;) {
    std::cout << "> ";
    std::getline(std::cin, line);
    vm.interpret(line);
  }
}

static void runFile(std::filesystem::path path) {
  auto fStream = std::ifstream(path);
  if (fStream.fail()) {
    std::cerr << "Could not open file" << std::endl;
    exit(74);
  }

  std::string source;
  try {
    std::stringstream ss;
    ss << fStream.rdbuf();
    source = ss.str();
  } catch (std::bad_alloc const &) {
    std::cerr << "Out of memory!" << std::endl;
    exit(74);
  }

  VM vm;
  InterpretResult result = vm.interpret(source);
  if (result == INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    runFile(std::filesystem::path(argv[1]));
  } else {
    std::cerr << "Usage: clox [path]" << std::endl;
    exit(64);
  }
}
