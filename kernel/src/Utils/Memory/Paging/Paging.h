#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

enum PT_Flag
{
    present = 0,
    readWrite = 1,
    userSuper = 2,
    writeThrough = 3,
    cacheDisabled = 4,
    accessed = 5,
    largerPages = 7,
    custom0 = 9,
    custom1 = 10,
    custom2 = 11,
    NX = 63    // Only if supported
};

struct PageDirectoryEntry
{
    uint64_t    value;
    void        SetFlag(PT_Flag flag, bool enabled);
    bool        GetFlag(PT_Flag flag);

    void        SetAddress(uint64_t address);
    uint64_t    GetAddress();
};

struct PageTable
{
    PageDirectoryEntry entries[512];
}__attribute__((aligned(0x1000)));

#endif // PAGING_H