#include "PageFrameAllocator.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool initialised = false;

void PageFrameAllocator::ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, size_t memoryMapSize, size_t memoryMapDescriptorSize)
{
    if (initialised) return;
    initialised = true;

    uint64_t memoryMapEntries = memoryMapSize / memoryMapDescriptorSize;

    void* largestFreeMemorySegment = NULL;
    size_t largestFreeMemorySegmentSize = 0;

    for (int i = 0; i < memoryMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = ((EFI_MEMORY_DESCRIPTOR*) (uint64_t)memoryMap + (i * memoryMapDescriptorSize));

        // Type = EfiConventionalMemory
        if (descriptor->type == 7)
        {
            if (descriptor->numPages * 4096 > largestFreeMemorySegmentSize)
            {
                largestFreeMemorySegment = descriptor->physicalAddr;
                largestFreeMemorySegmentSize = descriptor->numPages * 4096;
            }
        }
    }

    uint64_t memorySize = GetMemorySize(memoryMap, memoryMapEntries, memoryMapDescriptorSize);
    freeMemory = memorySize;

    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    InitBitmap(bitmapSize, largestFreeMemorySegment);

    // Lock pages of bitmap & reserve the pages of unusable/reserved memory
    LockPages(&pageBitmap, pageBitmap.size / 4096 + 1);

    for (int i = 0; i < memoryMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = ((EFI_MEMORY_DESCRIPTOR*) (uint64_t) memoryMap + (i * memoryMapDescriptorSize));

        // Memory is NOT EfiConventionalMemory
        if (descriptor->type != 7)
        {
            ReservePages(descriptor->physicalAddr, descriptor->numPages);
        }
    }
}

uint64_t PageFrameAllocator::GetFreeRAM()
{
    return freeMemory;
}

uint64_t PageFrameAllocator::GetUsedRAM()
{
    return usedMemory;
}

uint64_t PageFrameAllocator::GetReservedRAM()
{
    return reservedMemory;
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress)
{
    pageBitmap.size = bitmapSize;
    pageBitmap.buffer = (uint8_t*)bufferAddress;

    for (int i = 0; i < bitmapSize; i++)
    {
        *(uint8_t*)(pageBitmap.buffer + i) = 0;
    }
}

void PageFrameAllocator::FreePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;

    if (pageBitmap[index] == false)
        return;

    pageBitmap.Set(index, false);
    freeMemory += 4096;
    usedMemory -= 4096;
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; i++)
    {
        FreePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::LockPage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;

    if (pageBitmap[index]) return;

    pageBitmap.Set(index, true);

    freeMemory -= 4096;
    usedMemory += 4096;
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; i++)
    {
        LockPage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::ReservePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;

    if (pageBitmap[index] == true)
        return;

    pageBitmap.Set(index, true);
    freeMemory -= 4096;
    reservedMemory += 4096;
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; i++)
    {
        ReservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::UnreservePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;

    if (pageBitmap[index] == false)
        return;

    pageBitmap.Set(index, false);
    freeMemory += 4096;
    reservedMemory -= 4096;
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; i++)
    {
        UnreservePage((void*)((uint64_t)address + (i * 4096)));
    }
}