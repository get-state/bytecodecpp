#include "common.h"
#include <cstdint>
#include <vector>

typedef	enum {
	OP_RETURN,
} OpCode;

class Chunk
{
public:
	/* Chunk(){ */
	/* 	this->v = {}; */
	/* } */

	void writeChunk(OpCode op ){
		this->v.push_back(op);
	}

	void freeChunk(){
		this->v = {};
	}

private:
	std::vector<std::uint8_t> v;
	/* data */
};
