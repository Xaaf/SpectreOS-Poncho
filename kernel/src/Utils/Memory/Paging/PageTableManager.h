#ifndef PAGETABLEMANAGER_H
#define PAGETABLEMANAGER_H

#include "Paging.h"

class PageTableManager
{
public:
    PageTable*  pml4Address;
    
public:
    PageTableManager(PageTable* pml4Address);

    void        MapMemory(void* virtualMemory, void* physicalMemory);
};

#endif // PAGETABLEMANAGER_H