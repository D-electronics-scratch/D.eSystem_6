#ifndef PAGING_H
#define PAGING_H

#pragma once
#include <stdint.h>

#define PAGE_PRESENT    (1ull << 0)
#define PAGE_RW         (1ull << 1)
#define PAGE_USER       (1ull << 2)
#define PAGE_NO_CACHE   (1ull << 4)

void map_page(uint64_t virt, uint64_t phys, uint64_t flags);
void paging_init(uint64_t *root);
#endif
