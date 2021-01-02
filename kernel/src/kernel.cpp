#include <stdint.h>

#include "./Rendering/BasicRenderer.h"
#include "./Utils/cstr.h"
#include "./Utils/Memory/Bitmap.h"
#include "./Utils/Memory/EfiMemory.h"
#include "./Utils/Memory/Memory.h"
#include "./Utils/Memory/Paging/PageFrameAllocator.h"
#include "./Utils/Memory/Paging/PageMapIndexer.h"
#include "./Utils/Memory/Paging/PageTableManager.h"
#include "./Utils/Memory/Paging/Paging.h"

struct BootInfo
{
    Framebuffer*            framebuffer;
    PSF1_FONT*              psf1_font;
    EFI_MEMORY_DESCRIPTOR*  memoryMap;
    uint64_t                memoryMapSize;
    uint64_t                memoryMapDescriptorSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start(BootInfo* bootInfo) 
{

	BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
    uint64_t memoryMapEntries = bootInfo->memoryMapSize / bootInfo->memoryMapDescriptorSize;

    GlobalAllocator = PageFrameAllocator();
	GlobalAllocator.ReadEfiMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->memoryMapDescriptorSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    GlobalAllocator.LockPages(&_KernelStart, kernelPages);
    
    PageTable* pml4 = (PageTable*)GlobalAllocator.RequestPage();
    MemorySet(pml4, 0, 0x1000);

    PageTableManager pageTableManager = PageTableManager(pml4);

    for (uint64_t i = 0; i < GetMemorySize(bootInfo->memoryMap, memoryMapEntries, bootInfo->memoryMapDescriptorSize); i+= 0x1000)
    {
        pageTableManager.MapMemory((void*)i, (void*)i);
    }

    uint64_t framebufferBase = (uint64_t)bootInfo->framebuffer->baseAddress;
    uint64_t framebufferSize = (uint64_t)bootInfo->framebuffer->bufferSize + 0x1000;

    GlobalAllocator.LockPages((void*)framebufferBase, framebufferSize / 0x1000 + 1);

    for (uint64_t i = framebufferBase; i < framebufferBase + framebufferSize; i += 4096)
    {
        pageTableManager.MapMemory((void*)i, (void*)i);
    }

    asm ("mov %0, %%cr3" : : "r" (pml4));

    MemorySet(bootInfo->framebuffer->baseAddress, 0, bootInfo->framebuffer->bufferSize);

	return;
}