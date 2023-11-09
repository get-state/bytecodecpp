#include "chunk.h"
#include "debug.h"
#include "vm.h"

#define PROJECT_NAME "bcpplox"

int main(int argc, char **argv) {
	Chunk chunk;
	VM vm;
	auto constant = chunk.addConstant(1.2);
	chunk.writeChunk(OP_CONSTANT, 123);
	chunk.writeChunk(constant, 123);

	constant = chunk.addConstant(3.4);
	chunk.writeChunk(OP_CONSTANT, 123);
	chunk.writeChunk(constant, 123);

	chunk.writeChunk(OP_ADD, 123);

	constant = chunk.addConstant(5.6);
	chunk.writeChunk(OP_CONSTANT, 123);
	chunk.writeChunk(constant, 123);

	chunk.writeChunk(OP_DIVIDE, 123);

	chunk.writeChunk(OP_NEGATE, 123);
	chunk.writeChunk(OP_RETURN, 123);
	/* std::string_view t = "test chunk"; */
	vm.interpret(chunk);
	/* disassembleChunk(chunk, t); */
	chunk.freeChunk();
	/* int constant = addConstant(&chunk, 1.2); */
	return 0;
}
