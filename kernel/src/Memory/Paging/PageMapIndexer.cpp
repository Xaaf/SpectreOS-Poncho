#include "PageMapIndexer.h"

PageMapIndexer::PageMapIndexer(uint64_t virtualAddress)
{
    virtualAddress >>= 12;
    p_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    pt_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    pd_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    pdp_i = virtualAddress & 0x1ff;
}