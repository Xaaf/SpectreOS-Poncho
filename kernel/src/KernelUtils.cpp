#include "KernelUtils.h"
#include "./System/GDT/gdt.h"

KernelInfo kernelInfo;
PageTableManager pageTableManager = NULL;

void PrepareMemory(BootInfo* bootInfo)
{
    uint64_t memoryMapEntries = bootInfo->memoryMapSize / bootInfo->memoryMapDescriptorSize;

    GlobalAllocator = PageFrameAllocator();
	GlobalAllocator.ReadEfiMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->memoryMapDescriptorSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    GlobalAllocator.LockPages(&_KernelStart, kernelPages);
    
    PageTable* pml4 = (PageTable*)GlobalAllocator.RequestPage();
    MemorySet(pml4, 0, 0x1000);

    pageTableManager = PageTableManager(pml4);

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

    kernelInfo.pageTableManager = &pageTableManager;
}

KernelInfo InitialiseKernel(BootInfo* bootInfo)
{
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.size = sizeof(GlobalDescriptorTable) - 1;
    gdtDescriptor.offset = (uint64_t)&DefaultGDT;

    LoadGDT(&gdtDescriptor);

    PrepareMemory(bootInfo);
    MemorySet(bootInfo->framebuffer->baseAddress, 0, bootInfo->framebuffer->bufferSize);

    return kernelInfo;
}