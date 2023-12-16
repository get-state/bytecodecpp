#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include <string>

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

class VM {
public:
  /* VM(); */

  InterpretResult interpret(std::string const &source);

private:
  /* data */
  Chunk chunk;
  size_t ip; // instruction pointer
  std::vector<Value> stack = {};
  std::vector<cloxObj*> obj_ptrs = {}; // list to all (heap) allocated objects.

  /* methods */
  InterpretResult run();

  std::vector<Value> getVStack() { return this->stack; }

  void push(Value constant) { this->stack.push_back(constant); }

  Value pop() {
    Value tmp = this->stack.back();
    this->stack.pop_back();
    return tmp;
  }

  Value peek(int distance) { return this->stack.at(stack.size() - 1 - distance); }
  
  void concatenate();
};

#endif // ifndef clox_vm_h
