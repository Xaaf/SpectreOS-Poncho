#include "PageTableManager.h"

#include <stdint.h>

#include "../Memory.h"

#include "PageFrameAllocator.h"
#include "PageMapIndexer.h"

PageTableManager::PageTableManager(PageTable* pml4Address)
{
    this->pml4Address = pml4Address;
}

void PageTableManager::MapMemory(void* virtualMemory, void* physicalMemory)
{
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
    PageDirectoryEntry pde;

    pde = pml4Address->entries[indexer.pdp_i];
    PageTable* pdp;

    if (!pde.GetFlag(PT_Flag::present))
    {
        pdp = (PageTable*)GlobalAllocator.RequestPage();
        MemorySet(pdp, 0, 0x1000);

        pde.SetAddress((uint64_t)pdp >> 12);
        pde.SetFlag(PT_Flag::present, true);
        pde.SetFlag(PT_Flag::readWrite, true);

        pml4Address->entries[indexer.pdp_i] = pde;
    }
    else
    {
        pdp = (PageTable*)((uint64_t)pde.GetAddress() << 12);
    }

    pde = pdp->entries[indexer.pd_i];
    PageTable* pd;

    if (!pde.GetFlag(PT_Flag::present))
    {
        pd = (PageTable*)GlobalAllocator.RequestPage();
        MemorySet(pd, 0, 0x1000);

        pde.SetAddress((uint64_t)pd >> 12);
        pde.SetFlag(PT_Flag::present, true);
        pde.SetFlag(PT_Flag::readWrite, true);

        pdp->entries[indexer.pd_i] = pde;
    }
    else
    {
        pd = (PageTable*)((uint64_t)pde.GetAddress() << 12);
    }

    pde = pd->entries[indexer.pt_i];
    PageTable* pt;

    if (!pde.GetFlag(PT_Flag::present))
    {
        pt = (PageTable*)GlobalAllocator.RequestPage();
        MemorySet(pt, 0, 0x1000);

        pde.SetAddress((uint64_t)pt >> 12);
        pde.SetFlag(PT_Flag::present, true);
        pde.SetFlag(PT_Flag::readWrite, true);

        pd->entries[indexer.pt_i] = pde;
    }
    else
    {
        pt = (PageTable*)((uint64_t)pde.GetAddress() << 12);
    }

    pde = pt->entries[indexer.p_i];

    pde.SetAddress((uint64_t)physicalMemory >> 12);
    pde.SetFlag(PT_Flag::present, true);
    pde.SetFlag(PT_Flag::readWrite, true);

    pt->entries[indexer.p_i] = pde;
}