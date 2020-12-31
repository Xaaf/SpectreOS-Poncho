#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

typedef struct 
{
    void* baseAddress;
    size_t bufferSize;
    unsigned int width;
    unsigned int height;
    unsigned int pixelsPerScanline;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct
{
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charsize;
} PSF1_HEADER;

typedef struct
{
    PSF1_HEADER* psf1_header;
    void* glyphBuffer;
} PSF1_FONT;

typedef struct
{
    Framebuffer* framebuffer;
    PSF1_FONT* psf1_font;
    EFI_MEMORY_DESCRIPTOR* memoryMap;
    UINTN memoryMapSize;
    UINTN memoryMapDescriptorSize;
} BootInfo;

Framebuffer framebuffer;

Framebuffer* initialise_gop()
{
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_STATUS status;

    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);

    if (EFI_ERROR(status))
    {
        Print(L"Unable to locate GOP\n\r");
        return NULL;
    }

    Print(L"Located GOP\n\r");

    framebuffer.baseAddress = (void*)gop->Mode->FrameBufferBase;
    framebuffer.bufferSize = gop->Mode->FrameBufferSize;
    framebuffer.width = gop->Mode->Info->HorizontalResolution;
    framebuffer.height = gop->Mode->Info->VerticalResolution;
    framebuffer.pixelsPerScanline = gop->Mode->Info->PixelsPerScanLine;

    return &framebuffer;
}

EFI_FILE* load_file(EFI_FILE* directory, CHAR16* path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
    EFI_FILE* loadedFile;
    EFI_LOADED_IMAGE_PROTOCOL* loadedImage;

    systemTable->BootServices->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loadedImage);

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem;
    systemTable->BootServices->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&fileSystem);

    if (directory == NULL)
    {
        fileSystem->OpenVolume(fileSystem, &directory);
    }

    EFI_STATUS status = directory->Open(directory, &loadedFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (status != EFI_SUCCESS)
    {
        return NULL;
    }

    return loadedFile;
}

PSF1_FONT* load_psf1_font(EFI_FILE* directory, CHAR16* path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
    EFI_FILE* font = load_file(directory, path, imageHandle, systemTable);
    
    if (font == NULL) return NULL;

    PSF1_HEADER* fontHeader;
    systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&fontHeader);

    UINTN size = sizeof(PSF1_HEADER);
    font->Read(font, &size, fontHeader);

    if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1) return NULL;

    UINTN glyphBufferSize = fontHeader->charsize * 256;
    if (fontHeader->mode == 1) // Glyph mode 512
    {
        glyphBufferSize = fontHeader->charsize * 512;
    }

    void* glyphBuffer;
    {
        font->SetPosition(font, sizeof(PSF1_HEADER));
        systemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
        font->Read(font, &glyphBufferSize, glyphBuffer);
    }

    PSF1_FONT* finishedFont;
    systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont);

    finishedFont->psf1_header = fontHeader;
    finishedFont->glyphBuffer = glyphBuffer;

    return finishedFont;
}

int memory_compare(const void* aptr, const void* bptr, size_t n)
{
    const unsigned char* a = aptr;
    const unsigned char* b = bptr;

    for (size_t i = 0; i < n; i++)
    {
        if (a[i] < b[i]) return -1;
        else if (a[i] > b[i]) return 1;
    }

    return 0;
}

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
    InitializeLib(imageHandle, systemTable);
    Print(L"First ever message in SpectreOS!\n\r");

    EFI_FILE* kernel = load_file(NULL, L"kernel.elf", imageHandle, systemTable);
    if (kernel == NULL)
    {
        Print(L"Could not load kernel\n\r");
    }
    else
    {
        Print(L"Kernel loaded successfully\n\r");
    }

    
    Elf64_Ehdr header;
    {
        UINTN fileInfoSize;
        EFI_FILE_INFO* fileInfo;

        kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, NULL);
        systemTable->BootServices->AllocatePool(EfiLoaderData, fileInfoSize, (void**)&fileInfo);

        kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, (void**)&fileInfo);

        UINTN size = sizeof(header);
        kernel->Read(kernel, &size, &header);
    }

    if (
        memory_compare(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0
        || header.e_ident[EI_CLASS] != ELFCLASS64
        || header.e_ident[EI_DATA] != ELFDATA2LSB
        || header.e_type != ET_EXEC
        || header.e_machine != EM_X86_64
        || header.e_version != EV_CURRENT
    )
    {
        Print(L"Kernel format is bad\n\r");
    }
    else
    {
        Print(L"Kernel header successfully verified\n\r");
    }

    Elf64_Phdr* pheaders;
    {
        kernel->SetPosition(kernel, header.e_phoff);
        UINTN size = header.e_phnum * header.e_phentsize;

        systemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&pheaders);
        kernel->Read(kernel, &size, pheaders);
    }

    for (
        Elf64_Phdr* pheader = pheaders;
        (char*)pheader < (char*)pheaders + header.e_phnum * header.e_phentsize;
        pheader = (Elf64_Phdr*)((char*)pheader + header.e_phentsize)
    )
    {
        switch(pheader->p_type)
        {
            case PT_LOAD:
            {
                int pages = (pheader->p_memsz + 0x1000 - 1) / 0x1000;
                Elf64_Addr segment = pheader->p_paddr;
                systemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

                kernel->SetPosition(kernel, pheader->p_offset);
                UINTN size = pheader->p_filesz;
                kernel->Read(kernel, &size, (void*)segment);

                break;
            }
        }
    }

    Print(L"Kernel loaded...\n\r");

    PSF1_FONT* newFont = load_psf1_font(NULL, L"zap-light16.psf", imageHandle, systemTable);
    if (newFont == NULL)
    {
        Print(L"Font was invalid or not found!\n\r");
    }
    else 
    {
        Print(L"Font was found! Charsize = %d\n\r", newFont->psf1_header->charsize);
    }

    Framebuffer* newBuffer = initialise_gop();

    Print(L"Base: 0x%x; \nSize 0x%x; \nWidth: %d; \nHeight: %d; \nPixels Per Scanline: %d \n\r",
        newBuffer->baseAddress,
        newBuffer->bufferSize,
        newBuffer->width,
        newBuffer->height,
        newBuffer->pixelsPerScanline
    );

    EFI_MEMORY_DESCRIPTOR* map = NULL;
    UINTN mapSize, mapKey;
    UINTN descriptorSize;
    UINT32 descriptorVersion;
    {
        systemTable->BootServices->GetMemoryMap(&mapSize, map, &mapKey, &descriptorSize, &descriptorVersion);
        systemTable->BootServices->AllocatePool(EfiLoaderData, mapSize, (void**)&map);
        systemTable->BootServices->GetMemoryMap(&mapSize, map, &mapKey, &descriptorSize, &descriptorVersion);
    }

    void (*KernelStart)(BootInfo*) = ((__attribute__((sysv_abi)) void (*)(BootInfo*) ) header.e_entry);

    BootInfo bootInfo;
    bootInfo.framebuffer = newBuffer;
    bootInfo.psf1_font = newFont;
    bootInfo.memoryMap = map;
    bootInfo.memoryMapSize = mapSize;
    bootInfo.memoryMapDescriptorSize = descriptorSize;

    systemTable->BootServices->ExitBootServices(imageHandle, mapKey);

    KernelStart(&bootInfo);

    return EFI_SUCCESS; // Exit the UEFI application
}
