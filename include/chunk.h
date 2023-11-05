#ifndef clox_chunk_h
#define clox_chunk_h

#include "value.h"
#include <cstdint>
#include <vector>

typedef enum {
  OP_CONSTANT,
  OP_RETURN,
} OpCode;

class Chunk {
public:
  /* Chunk(){ */
  /* 	this->v = {}; */
  /* } */

  void writeChunk(uint8_t byte, int line) {
    this->v.push_back(byte);
    this->lines.push_back(line);
    this->count = v.size();
  }

  int addConstant(Value value) {
    constants.push_back(value);
    return constants.size() - 1;
  }

  void freeChunk() {
    this->v = {};
    this->count = v.size();
  }

  size_t getCount() { return this->count; }

  // returns a chunk at the specified offset
  size_t getAtOffset(size_t offset) { return this->v[offset]; }

  // returns a constant at the specified offset
  Value getConstantValue(uint8_t offset) { return constants.at(offset); }

  // returns the line number at specific offset
  int getLine(uint8_t offset) { return lines.at(offset); }

private:
  // stores chunks
  std::vector<std::uint8_t> v;
  size_t count;
  ValueArray constants;
  std::vector<int> lines;
  /* data */
};
#endif /* ifndef clox_chunk_h */
