#ifndef KERNELUTILS_H
#define KERNELUTILS_H

#include <stdint.h>

#include "./Rendering/BasicRenderer.h"
#include "./System/cstr.h"
#include "./Memory/Bitmap.h"
#include "./Memory/EfiMemory.h"
#include "./Memory/Memory.h"
#include "./Memory/Paging/PageFrameAllocator.h"
#include "./Memory/Paging/PageMapIndexer.h"
#include "./Memory/Paging/PageTableManager.h"
#include "./Memory/Paging/Paging.h"

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

struct KernelInfo
{
    PageTableManager*       pageTableManager;
};

void        PrepareMemory(BootInfo* bootInfo);
KernelInfo  InitialiseKernel(BootInfo* bootInfo);

#endif // KERNELUTILS_H