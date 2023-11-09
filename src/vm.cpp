#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include <iostream>

InterpretResult VM::run() {

#define READ_CONSTANT()                                                        \
  (this->chunk.getConstantValue(this->ip++)) // reads one byte ahead.

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
    switch (instruction = this->chunk.getAtOffset(this->ip)) {
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
    }

    this->ip++;
  }
#undef READ_CONSTANT
}

InterpretResult VM::interpret(Chunk &chunk) {
  this->chunk = chunk;
  this->ip = 0; // sets index to 0 before interpreting.
  return VM::run();
}
