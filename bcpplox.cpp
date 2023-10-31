#include <iostream>
#include "include/common.h"
#include "include/chunk.h"

#define PROJECT_NAME "bcpplox"

int main(int argc, char **argv) {
	Chunk chunk;
	chunk.writeChunk(OP_RETURN);
	chunk.freeChunk();
	return 0;
}
