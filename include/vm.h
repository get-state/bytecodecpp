#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

class VM
{
public:
	VM (Chunk chunk) : chunk{chunk}{};

private:
	Chunk chunk;/* data */
};

#endif // ifndef clox_vm_h 
