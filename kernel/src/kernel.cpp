#include <stdint.h>

#include "./Rendering/BasicRenderer.h"
#include "./Utils/cstr.h"
#include "./Utils/Memory/Bitmap.h"
#include "./Utils/Memory/EfiMemory.h"
#include "./Utils/Memory/Memory.h"
#include "./Utils/Memory/Paging/PageFrameAllocator.h"

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

	PageFrameAllocator newAllocator;
	newAllocator.ReadEfiMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->memoryMapDescriptorSize);

    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};
    newRenderer.Print("Free RAM: ");
    newRenderer.Print(to_string(newAllocator.GetFreeRAM() / 1024));
    newRenderer.Print(" KB");
    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};

    newRenderer.Print("Used RAM: ");
    newRenderer.Print(to_string(newAllocator.GetUsedRAM() / 1024));
    newRenderer.Print(" KB");
    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};

    newRenderer.Print("Reserved RAM: ");
    newRenderer.Print(to_string(newAllocator.GetReservedRAM() / 1024));
    newRenderer.Print(" KB");
    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    newAllocator.LockPages(&_KernelStart, kernelPages);

    for (int i = 0; i < 20; i++)
    {
        void* address = newAllocator.RequestPage();

        newRenderer.Print(to_hstring((uint64_t)address));
        newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};
    }

	return;
}