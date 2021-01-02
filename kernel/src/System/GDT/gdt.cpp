#include "gdt.h"

__attribute__((aligned(0x1000)))
GlobalDescriptorTable DefaultGDT =
{
    // null segment
    { 0, 0, 0, 0x00, 0x00, 0 },
    // kernelCodeSegment
    { 0, 0, 0, 0x9a, 0xa0, 0 },
    // kernelDataSegment
    { 0, 0, 0, 0x92, 0xa0, 0 },

    // userNullSe00ent
    { 0, 0, 0, 0x00, 0x00, 0 },
    // userCodeSegment
    { 0, 0, 0, 0x9a, 0xa0, 0 },
    // userDataSegment
    { 0, 0, 0, 0x92, 0xa0, 0 }
};