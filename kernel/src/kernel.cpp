#include <stdint.h>

#include "./Rendering/BasicRenderer.h"
#include "./Utils/cstr.h"
#include "./Utils/Memory/Bitmap.h"
#include "./Utils/Memory/EfiMemory.h"
#include "./Utils/Memory/Memory.h"
#include "./Utils/Memory/PageFrameAllocator.h"

struct BootInfo
{
    Framebuffer* framebuffer;
    PSF1_FONT* psf1_font;
    EFI_MEMORY_DESCRIPTOR* memoryMap;
    uint64_t memoryMapSize;
    uint64_t memoryMapDescriptorSize;
};

uint8_t testBuffer[20];

extern "C" void _start(BootInfo* bootInfo) 
{
	BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);

//	newRenderer.Print("Memory Map Size: ");
//	newRenderer.Print(to_string(bootInfo->memoryMapSize));
//    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};
//    newRenderer.Print("Memory Descriptor Map Size: ");
//    newRenderer.Print(to_string(bootInfo->memoryMapDescriptorSize));
//    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};

	PageFrameAllocator newAllocator;
	newAllocator.ReadEfiMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->memoryMapDescriptorSize);

    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};
    newRenderer.Print("Free RAM: ");
    newRenderer.Print(to_string(newAllocator.GetFreeRAM() / 1024));
    newRenderer.Print(" KB ");
    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};

    newRenderer.Print("Used RAM: ");
    newRenderer.Print(to_string(newAllocator.GetUsedRAM()));
    newRenderer.Print(" KB ");
    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};

    newRenderer.Print("Reserved RAM: ");
    newRenderer.Print(to_string(newAllocator.GetReservedRAM() / 1024));
    newRenderer.Print(" KB ");
    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};

    newRenderer.Print("Temp RAM: ");
    newRenderer.Print(to_string(newAllocator.GetTempRAM()));
    newRenderer.Print(" KB ");
    newRenderer.cursorPosition = {0, newRenderer.cursorPosition.y + 16};

	return;
}