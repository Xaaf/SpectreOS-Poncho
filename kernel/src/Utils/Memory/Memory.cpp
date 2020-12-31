#include "Memory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* memoryMap, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize)
{
	static uint64_t memorySizeBytes = 0;
	
	if (memorySizeBytes > 0)
		return memorySizeBytes;

	for (int i = 0; i < memoryMapEntries; i++)
	{
		EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap + (i * memoryMapDescriptorSize));
		memorySizeBytes += descriptor->numPages * 4096;
	}

	return memorySizeBytes;
}