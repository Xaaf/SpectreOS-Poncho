#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stddef.h>

struct Framebuffer 
{
    void* baseAddress;
    size_t bufferSize;
    unsigned int width;
    unsigned int height;
    unsigned int pixelsPerScanline;
};

#endif // FRAMEBUFFER_H