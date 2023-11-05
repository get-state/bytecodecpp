#include "vm.h"

InterpretResult VM::run() {
  for (;;) {
    uint8_t instruction;
    switch (instruction = this->chunk.getAtOffset(this->ip)) {
    case OP_RETURN: {
      return INTERPRET_OK;
    }
    }
  }
}

InterpretResult VM::interpret(Chunk &chunk) {
  this->chunk = chunk;
  this->ip = 0; // sets index to 0 before interpreting.
  return VM::run();
}
