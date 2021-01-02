#include "KernelUtils.h"

extern "C" void _start(BootInfo* bootInfo) 
{
    KernelInfo kernelInfo = InitialiseKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);

    newRenderer.Print("Kernel has been initialised!");

	return;
}