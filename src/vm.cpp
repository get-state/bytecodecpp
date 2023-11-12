#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include <iostream>

InterpretResult VM::run() {

#define READ_CONSTANT()                                                        \
  (this->chunk.getConstantValue(                                               \
      this->chunk.getAtOffset(this->ip++))) // reads one byte ahead.
                                            //
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = this->pop();                                                    \
    double a = this->pop();                                                    \
    this->push(a op b);                                                        \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    std::cout << "\t";
    auto stackCopy = this->getStack();
    while (!stackCopy.empty()) {
      std::cout << "[ ";
      printValue(stackCopy.top());
      stackCopy.pop();
      std::cout << " ]";
    }
    std::cout << std::endl;
    disassembleInstruction(
        this->chunk,
        static_cast<size_t>(this->ip - this->chunk.getAtOffset(0)));
#endif
    uint8_t instruction;
    switch (instruction = this->chunk.getAtOffset(this->ip++)) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      this->push(constant);
      break;
    }
    case OP_RETURN: {
      printValue(this->pop());
      std::cout << std::endl;
      return INTERPRET_OK;
    }
    case OP_NEGATE: {
      this->push(-this->pop());
      break;
    }
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    }
  }
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult VM::interpret(std::string const &source) {
  Compiler compile(source);
  return INTERPRET_OK;
}
