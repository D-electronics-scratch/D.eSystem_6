#include "disk.h"
#include <string.h>
#include "fat32_fs.h"

//static EFI_BLOCK_IO_PROTOCOL *g_block_io = NULL;

static int disk_read_impl(disk_read_t *req) {
    if (req == NULL || req->buffer == NULL || req->count == 0)
        return -1;

    if (fs_image == NULL || fs_image_size == 0)
        return -1;

    //uint64_t buffer_size =
        //(uint64_t)req->count * g_block_io->Media->BlockSize;

    uint64_t offset = (uint64_t)req->lba * 512;
    uint64_t size = (uint64_t)req->count * 512;

    if (offset > fs_image_size) {
        return -1;
    }

    if (size > fs_image_size - offset) {
        return -1;
    }

    memcpy(req->buffer, fs_image + offset, size);
    //EFI_STATUS status = g_block_io->ReadBlocks(
    //    g_block_io,
    //    g_block_io->Media->MediaId,
    //    req->lba,
    //    buffer_size,
    //    req->buffer
    //);

    return 0;
}

disk_api_t disk_api = {
    .read = disk_read_impl
};

int write_sector(uint32_t lba, uint8_t *buffer) {
    if (buffer == NULL)
        return -1;

    if (fs_image == NULL || fs_image_size == 0)
        return -1;

    uint64_t offset = (uint64_t)lba * 512;

    if (offset > fs_image_size)
        return -1;

    if (512 > fs_image_size - offset)
        return -1;

    memcpy(fs_image + offset, buffer, 512);

    return 0;
}