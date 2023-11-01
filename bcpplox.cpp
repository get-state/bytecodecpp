#include <iostream>
#include "include/common.h"
#include "chunk.h"
#include "debug.h"

#define PROJECT_NAME "bcpplox"

int main(int argc, char **argv) {
	Chunk chunk;
	chunk.writeChunk(OP_RETURN);
	chunk.freeChunk();
	std::string_view t = "test chunk";
	disassembleChunk(chunk, t);
	return 0;
}
