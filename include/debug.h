#include <string_view>
#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassembleChunk(Chunk &chunk, std::string_view const &strView);
int disassembleInstruction(Chunk &chunk, size_t offset);

#endif /* ifndef clox_debug_h */
