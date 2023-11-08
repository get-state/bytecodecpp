#include "vm.h"
#include "chunk.h"
#include "value.h"

InterpretResult VM::run() {

#define READ_CONSTANT() (this->chunk.getConstantValue(this->ip++)) // reads one byte ahead.

  for (;;) {
   #ifdef DEBUG_TRACE_HIGHLIGHTING
   	disassembleInstruction(this->chunk, (int) this->ip - vm.chunk.getAtOffset(0)) 
   #endif
    uint8_t instruction;
    switch (instruction = this->chunk.getAtOffset(this->ip)) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      printValue(constant);
      break;
    }
    case OP_RETURN: {
      return INTERPRET_OK;
    }
    }
  }
#undef READ_CONSTANT
}

InterpretResult VM::interpret(Chunk &chunk) {
  this->chunk = chunk;
  this->ip = 0; // sets index to 0 before interpreting.
  return VM::run();
}
