#ifndef clox_chunk_h
#define clox_chunk_h 

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
		this->count = v.size();
	}

	void freeChunk(){
		this->v = {};
		this->count = v.size();
	}
	
	size_t getCount(){
		return this->count;
	}
	 
	size_t getInstruction(size_t offset){
		return this->v[offset];
	}

   

private:
	std::vector<std::uint8_t> v;
	size_t count;
	/* data */
};
#endif /* ifndef clox_chunk_h */
