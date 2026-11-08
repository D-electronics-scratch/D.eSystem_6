#ifndef DISK_H
#define DISK_H

#include <stdint.h>
//#include <limine.h>     
#include <stddef.h>     

//extern disk_api_t disk_api;

typedef struct {
    uint32_t lba;
    uint32_t count;
    uint8_t *buffer;
} disk_read_t;

//typedef struct {
//    int (*read)(disk_read_t *req);
//} disk_api_t;



//int read_sector(uint32_t lba, uint8_t * buffer);

//static volatile struct limine_disk_request disk_request = {
//    .id = LIMINE_DISK_REQUEST,
//    .revision = 0
//};

typedef struct {

    uint32_t lba;
    uint32_t count;
    uint8_t *buffer;

} disk_write_t;

typedef struct {

    int (*read)(disk_read_t *req);
    int (*write)(disk_write_t *req);

} disk_api_t;

extern disk_api_t disk_api;

int read_sector(uint32_t lba, uint8_t *buffer);
int write_sector(uint32_t lba, uint8_t *buffer);
//int write_sector(uint32_t lba, uint8_t *buffer);



#endif