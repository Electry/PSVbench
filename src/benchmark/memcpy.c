#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/dmac.h>
#include <psp2/gxm.h>
#include <string.h>

#define CDRAM_BLOCK_SIZE 40 * 1024 * 1024
#define LPDDR_BLOCK_SIZE 100 * 1024 * 1024
#define PHYCONT_BLOCK_SIZE 13 * 1024 * 1024

int run_cpy_loops(SceKernelMemBlockType dest_type, SceKernelMemBlockType src_type, uint32_t size,
        int loops, void *(*cpy)(void *dest, const void *src, size_t size)) {

    SceUID block[2];
    void *base[2];

    block[0] = sceKernelAllocMemBlock("dest_block", dest_type, size, NULL);
    block[1] = sceKernelAllocMemBlock("src_block", src_type, size, NULL);
    sceKernelGetMemBlockBase(block[0], &base[0]);
    sceKernelGetMemBlockBase(block[1], &base[1]);

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;

    for (int i = 0; i < loops; i++) {
        start_time = sceKernelGetProcessTimeLow();
        cpy(base[0], base[1], size);
        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    sceKernelFreeMemBlock(block[0]);
    sceKernelFreeMemBlock(block[1]);

    return (1000000.0f / (total_time / total_n)) * (size / 1024.0f / 1024); // MB/s
}

int run_memcpy_uc2uc() {
    return run_cpy_loops(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
        LPDDR_BLOCK_SIZE,
        5,
        memcpy);
}

int run_memcpy_phy2phy() {
    return run_cpy_loops(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW,
        SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW,
        PHYCONT_BLOCK_SIZE,
        5,
        memcpy);
}

int run_memcpy_cd2cd() {
    return run_cpy_loops(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
        SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
        CDRAM_BLOCK_SIZE,
        5,
        memcpy);
}

int run_memcpy_uc2cd() {
    return run_cpy_loops(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
        CDRAM_BLOCK_SIZE,
        5,
        memcpy);
}

int run_dma_uc2uc() {
    return run_cpy_loops(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
        LPDDR_BLOCK_SIZE,
        20,
        sceDmacMemcpy);
}

int run_dma_phy2phy() {
    return run_cpy_loops(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW,
        SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW,
        PHYCONT_BLOCK_SIZE,
        40,
        sceDmacMemcpy);
}

int run_dma_cd2cd() {
    return run_cpy_loops(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
        SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
        CDRAM_BLOCK_SIZE,
        40,
        sceDmacMemcpy);
}

int run_dma_uc2cd() {
    return run_cpy_loops(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
        2 * CDRAM_BLOCK_SIZE,
        20,
        sceDmacMemcpy);
}
