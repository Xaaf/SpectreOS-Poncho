#ifndef EFIMEMORY_H
#define EFIMEMORY_H

#include <stdint.h>

struct EFI_MEMORY_DESCRIPTOR 
{
	uint32_t 	type;
	void* 		physicalAddr;
	void* 		virtualAddr;
	uint64_t 	numPages;
	uint64_t 	attribs;
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];

#endif // EFIMEMORY_H