#include "debug.h"
#include "chunk.h"
#include "value.h"
#include <cstdint>
#include <cstdio>
#include <iostream>

void disassembleChunk(Chunk& chunk, std::string_view const &name) {
  std::cout << "== " << name << " ==" << std::endl;

  for (size_t offset = 0; offset < chunk.getCount();) {
    offset = disassembleInstruction(chunk, offset);
  }
  return;
}

static int constantInstruction(std::string_view name, Chunk& chunk,
                               size_t offset) {
  uint8_t constant = chunk.getAtOffset(offset + 1);
  std::cout << name << " " << constant;
  value::printValue(chunk.getConstantValue(constant));
  std::cout << std::endl;
  return offset + 2;
}

static int simpleInstruction(std::string_view name, size_t offset) {
  std::cout << name << std::endl;
  return offset + 1;
}

int disassembleInstruction(Chunk& chunk, size_t offset) {
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
  case OP_NEGATE:
    return simpleInstruction("OP_NEGATE", offset);
  case OP_ADD:
    return simpleInstruction("OP_ADD", offset);
  case OP_SUBTRACT:
    return simpleInstruction("OP_SUBTRACT", offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY", offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE", offset);
  case OP_TRUE:
    return simpleInstruction("OP_TRUE", offset);
  case OP_FALSE:
    return simpleInstruction("OP_FALSE", offset);
  case OP_NIL:
    return simpleInstruction("OP_NIL", offset);
  case OP_NOT:
    return simpleInstruction("OP_NOT", offset);
  case OP_EQUAL:
    return simpleInstruction("OP_EQUAL", offset);
  case OP_GREATER:
    return simpleInstruction("OP_GREATER", offset);
  case OP_LESS:
    return simpleInstruction("OP_LESS", offset);
  default:
    std::cout << "unknown opcode " << instruction << std::endl;
  }
  return -1;
}
