#ifndef GDT
#define GDT

#include <stdint.h>

struct GDTDescriptor
{
    uint16_t    size;
    uint64_t    offset;
}__attribute__((packed));

struct GDTEntry
{
    uint16_t    limit0;
    uint16_t    base0;
    uint8_t     base1;
    uint8_t     accessByte;
    uint8_t     limit1_flags;
    uint8_t     base2;
}__attribute__((packed));

// TODO: Rename this to GDT instead; currently broken for some reason
struct GlobalDescriptorTable
{
    GDTEntry    null;               // 0x00
    GDTEntry    kernelCodeSegment;  // 0x08
    GDTEntry    kernelDataSegment;  // 0x10
    GDTEntry    userNullSegment;
    GDTEntry    userCodeSegment;
    GDTEntry    userDataSegment;
} __attribute__((packed))
__attribute((aligned(0x1000)));

extern GlobalDescriptorTable DefaultGDT;
extern "C" void LoadGDT(GDTDescriptor* gdtDescriptor);

#endif // GDT