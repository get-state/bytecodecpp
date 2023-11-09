#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include <stack>

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

class VM {
public:
  /* VM(); */

  InterpretResult run();

  InterpretResult interpret(Chunk &chunk);

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
  std::stack<Value> stack;
  /* data */
};

#endif // ifndef clox_vm_h
