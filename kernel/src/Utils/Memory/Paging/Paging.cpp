#include "Paging.h"

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled)
{
    uint64_t bitSelector = (uint64_t)1 << flag;
    value &= ~bitSelector;

    if (enabled)
        value |= bitSelector;
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag)
{
    uint64_t bitSelector = (uint64_t)1 << flag;

    return value & bitSelector > 0 ? true : false;
}

uint64_t PageDirectoryEntry::GetAddress()
{
    return (value & 0x000ffffffffff000) >> 12;
}

void PageDirectoryEntry::SetAddress(uint64_t address)
{
    address &= 0x000000ffffffffff;
    value &= 0xfff0000000000fff;
    value |= (address << 12);
}