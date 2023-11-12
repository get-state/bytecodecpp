#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include <stack>
#include <string>

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

class VM {
public:
  /* VM(); */

  InterpretResult run();

  InterpretResult interpret(std::string const& source);

  std::stack<Value> getStack() { return this->stack; }

  void push(Value constant) { this->stack.push(constant); }

  Value pop() {
    Value tmp = this->stack.top();
    this->stack.pop();
    return tmp;
  }

private:
  Chunk chunk;
  size_t ip; // instruction pointer
  std::stack<Value> stack = {};
  /* data */
};

#endif // ifndef clox_vm_h
