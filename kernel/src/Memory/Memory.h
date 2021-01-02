#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "EfiMemory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* memoryMap, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize);
void     MemorySet(void* startAddress, uint8_t value, uint64_t num);

#endif // MEMORY_H