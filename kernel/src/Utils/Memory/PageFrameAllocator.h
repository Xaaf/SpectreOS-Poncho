#ifndef PAGEFRAMEALLOCATOR_H
#define PAGEFRAMEALLOCATOR_H

#include <stdint.h>

#include "Bitmap.h"
#include "EfiMemory.h"
#include "Memory.h"

class PageFrameAllocator
{
public:
	Bitmap pageBitmap;

public:
	void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, size_t memoryMapSize, size_t memoryMapDescriptorSize);

	void FreePage(void* address);
    void FreePages(void* address, uint64_t pageCount);
    void LockPage(void* address);
    void LockPages(void* address, uint64_t pageCount);

    uint64_t GetFreeRAM();
    uint64_t GetUsedRAM();
    uint64_t GetReservedRAM();

private:
    void InitBitmap(size_t bitmapSize, void* bufferAddress);
	void ReservePage(void* address);
    void ReservePages(void* address, uint64_t pageCount);
	void UnreservePage(void* address);
    void UnreservePages(void* address, uint64_t pageCount);
};

#endif // PAGEFRAMEALLOCATOR_H