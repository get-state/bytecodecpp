#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include <cstdint>
#include <memory>

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

class VM {
public:
  VM();

  InterpretResult run();

  InterpretResult interpret(Chunk &chunk);

private:
  Chunk chunk;
  size_t ip; // instruction pointer
             /* data */
};

#endif // ifndef clox_vm_h
