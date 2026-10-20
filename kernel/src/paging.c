#include <stdint.h>
#include "paging.h"
#include "memory.h"

static uint64_t *pml4 = NULL;


//extern uint64_t *pml4;

static inline uint64_t *get_next_level(uint64_t *table, uint64_t index, uint64_t flags) {
    if (!(table[index] & PAGE_PRESENT)) {
        uint64_t *new_page = alloc_page();
        for (int i = 0; i < 512; i++) new_page[i] = 0;

        table[index] = ((uint64_t)new_page) | flags;
        return new_page;
    }
    return (uint64_t *)(table[index] & ~0xFFF);
}

void map_page(uint64_t virt, uint64_t phys, uint64_t flags) {
    uint64_t pml4_i = (virt >> 39) & 0x1FF;
    uint64_t pdpt_i = (virt >> 30) & 0x1FF;
    uint64_t pd_i   = (virt >> 21) & 0x1FF;
    uint64_t pt_i   = (virt >> 12) & 0x1FF;

    uint64_t *pdpt = get_next_level(pml4, pml4_i, flags);
    uint64_t *pd   = get_next_level(pdpt, pdpt_i, flags);
    uint64_t *pt   = get_next_level(pd, pd_i, flags);

    pt[pt_i] = (phys & ~0xFFF) | flags;
}





void paging_init(uint64_t *root) {
    pml4 = root;
}

