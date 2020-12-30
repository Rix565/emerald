/* Modified version of @szhou42's PMM on github*/

#include "pmm.h"
#include <debug-utilities/logger.h>
#include <stdint.h>
uint8_t *bitmap = (uint8_t *)(&end);
uint8_t *mem_start;
uint32_t total_blocks;
uint32_t bitmap_size;
void *memset(void *bufptr, int value, size_t size)
{
    unsigned char *buf = (unsigned char *)bufptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = (unsigned char)value;
    return bufptr;
}
void EmeraldMem_PMM_pmm_init(uint32_t mem_size)
{
    total_blocks = mem_size / BLOCK_SIZE;
    bitmap_size = total_blocks / BLOCKS_PER_BUCKET;
    if (bitmap_size * BLOCKS_PER_BUCKET < total_blocks)
        bitmap_size++;

    memset(bitmap, 0, bitmap_size);

    int mb = mem_size / (1024 * 1024);
    log(INFO, "Initializing PMM...\033[0;37m Bitmap adress = %d,Memory size = %d mb,Bitmap size = %d %s", bitmap, mb, bitmap_size, "\033[1;0mDone");
}

//malloc
uint32_t EmeraldMem_PMM_allocate_block()
{
    uint32_t free_block = EmeraldMem_PMM_first_free_block();
    SETBIT(free_block);
    return free_block;
}
//free
void EmeraldMem_PMM_free_block(uint32_t block_num)
{
    CLEARBIT(block_num);
}

uint32_t last_free_block = 0;
uint32_t EmeraldMem_PMM_first_free_block()
{
    uint32_t i;
    for (i = last_free_block; i < total_blocks; i++)
    {
        if (!ISSET(i))
        {
            last_free_block = i;
            return i;
        }
    }
    last_free_block = 0;
    return EmeraldMem_PMM_first_free_block();
}