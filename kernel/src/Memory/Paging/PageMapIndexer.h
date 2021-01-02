#ifndef PAGEMAPINDEXER_H
#define PAGEMAPINDEXER_H

#include <stdint.h>

class PageMapIndexer
{
public:
    uint64_t pdp_i;
    uint64_t pd_i;
    uint64_t pt_i;
    uint64_t p_i;

public:
    PageMapIndexer(uint64_t virtualAddress);
};

#endif // PAGEMAPINDEXER_H