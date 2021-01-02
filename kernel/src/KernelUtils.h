#ifndef KERNELUTILS_H
#define KERNELUTILS_H

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

struct KernelInfo
{
    PageTableManager*       pageTableManager;
};

void        PrepareMemory(BootInfo* bootInfo);
KernelInfo  InitialiseKernel(BootInfo* bootInfo);

#endif // KERNELUTILS_H