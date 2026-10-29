#pragma once
#include <stdint.h>

typedef uint64_t EFI_STATUS;
typedef void *EFI_HANDLE;

#define EFI_ERROR(x) ((x) != 0)

typedef struct {
    uint32_t MediaId;
    uint32_t BlockSize;
} EFI_BLOCK_IO_MEDIA;

typedef struct EFI_BLOCK_IO_PROTOCOL {
    EFI_BLOCK_IO_MEDIA *Media;
    EFI_STATUS (*ReadBlocks)(
        struct EFI_BLOCK_IO_PROTOCOL *This,
        uint32_t MediaId,
        uint64_t LBA,
        uint64_t BufferSize,
        void *Buffer
    );
} EFI_BLOCK_IO_PROTOCOL;
