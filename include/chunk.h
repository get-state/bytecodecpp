#ifndef clox_chunk_h
#define clox_chunk_h 

#include "common.h"
#include "value.h"
#include <cstdint>
#include <vector>

typedef	enum {
	OP_CONSTANT,
	OP_RETURN,
} OpCode;

class Chunk
{
public:
	/* Chunk(){ */
	/* 	this->v = {}; */
	/* } */

	void writeChunk(uint8_t byte){
		this->v.push_back(byte);
		this->count = v.size();
	}

	int addConstant(Value value){
		constants.push_back(value);
		return constants.size() -1;
	}

	void freeChunk(){
		this->v = {};
		this->count = v.size();
	}
	
	size_t getCount(){
		return this->count;
	}
	 
	size_t getAtOffset(size_t offset){
		return this->v[offset];
	}
	
	Value getConstantValue(uint8_t constant){
		return constants.at(constant);
	}

private:
	std::vector<std::uint8_t> v;
	size_t count;
	ValueArray constants;
	/* data */
};
#endif /* ifndef clox_chunk_h */
