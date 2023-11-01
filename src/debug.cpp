#include "debug.h"
#include "chunk.h"
#include <cstdint>
#include <cstdio>
#include <iostream>

void disassembleChunk(Chunk &chunk, std::string_view const &name) {
  std::cout << "== " << name << " ==" << std::endl;

  for (size_t offset = 0; offset < chunk.getCount();) {
    offset = disassembleInstruction(chunk, offset);
  }
  return;
}

static int simpleInstruction(std::string_view name, size_t offset) {
  std::cout << name << std::endl;
  return offset + 1;
}

int disassembleInstruction(Chunk& chunk, size_t offset) {
  printf("%04zu", offset);

  uint8_t instruction = chunk.getInstruction(offset);
  switch (instruction) {
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  default:
    printf("unknown opcode %d\n", instruction);
  }
  return -1;
}
