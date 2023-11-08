#include "debug.h"
#include "chunk.h"
#include "value.h"
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

static int constantInstruction(std::string_view name, Chunk &chunk,
                               size_t offset) {
  uint8_t constant = chunk.getAtOffset(offset + 1);
  std::cout << name << " " << constant;
  printValue(chunk.getConstantValue(constant));
  return offset + 2;
}

static int simpleInstruction(std::string_view name, size_t offset) {
  std::cout << name << std::endl;
  return offset + 1;
}

int disassembleInstruction(Chunk &chunk, size_t offset) {
  std::cout << offset << " ";

  if (offset > 0 && chunk.getLine(offset) == chunk.getLine(offset - 1)) {
    std::cout << "   | ";
  } else {
    std::cout << chunk.getLine(offset) << " ";
  }

  uint8_t instruction = chunk.getAtOffset(offset);
  switch (instruction) {
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", chunk, offset);
  default:
    std::cout << "unknown opcode " << instruction << std::endl;
  }
  return -1;
}
