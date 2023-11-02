#include <iostream>
#include "include/common.h"
#include "chunk.h"
#include "debug.h"

#define PROJECT_NAME "bcpplox"

int main(int argc, char **argv) {
	Chunk chunk;
	int constant = chunk.addConstant(1.2);
	chunk.writeChunk(OP_CONSTANT);
	chunk.writeChunk(constant);
	chunk.writeChunk(OP_RETURN);
	std::string_view t = "test chunk";
	disassembleChunk(chunk, t);
	chunk.freeChunk();
	/* int constant = addConstant(&chunk, 1.2); */
	return 0;
}
