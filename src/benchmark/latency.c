#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>

#define LOOP_COUNT 1000
#define BLOCK_SIZE 100 * 1024 * 1024 // 100 MB
#define CDRAM_SIZE 10 * 1024 * 1024

typedef struct llist_t {
    struct llist_t *next;
} llist_t;

typedef struct l2_llist_t {
    struct l2_llist_t *next;
    char pad[48 * 1024]; // 48 kB
} l2_llist_t;

#define TEN(x) x x x x x x x x x x
#define FIFTY(x) TEN(x) TEN(x) TEN(x) TEN(x) TEN(x)
#define TWO_HUNDRED(x) FIFTY(x) FIFTY(x) FIFTY(x) FIFTY(x)
#define THOUSAND(x) TWO_HUNDRED(x) TWO_HUNDRED(x) TWO_HUNDRED(x) TWO_HUNDRED(x) TWO_HUNDRED(x)
#define TWO_THOUSAND(x) THOUSAND(x) THOUSAND(x)
#define FIVE_THOUSAND(x) THOUSAND(x) THOUSAND(x) THOUSAND(x) THOUSAND(x) THOUSAND(x)
#define TEN_THOUSAND(x) FIVE_THOUSAND(x) FIVE_THOUSAND(x)
#define FIFTY_THOUSAND(x) TEN_THOUSAND(x) TEN_THOUSAND(x) TEN_THOUSAND(x) TEN_THOUSAND(x) TEN_THOUSAND(x)

int run_latency_lpddr() {
    llist_t *cell;
    SceUID block = sceKernelAllocMemBlock("lpddr", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, BLOCK_SIZE, NULL);
    sceKernelGetMemBlockBase(block, (void **)&cell);

    for (int i = 0; i < 10002; i++) {
        cell[i].next = &cell[i + 1];
    }

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;
    for (int i = 0; i < LOOP_COUNT; i++) {
        start_time = sceKernelGetProcessTimeLow();

        asm ("mov r2, %0\n\t"
             TEN_THOUSAND("ldr r2, [r2]\n\t")
             :: "r" (&cell[0]));

        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    sceKernelFreeMemBlock(block);

    return ((float)total_time / total_n) * 1000.0f / 10000;
}

int run_latency_l1() {
    llist_t *cell;
    SceUID block = sceKernelAllocMemBlock("lpddr", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, BLOCK_SIZE, NULL);
    sceKernelGetMemBlockBase(block, (void **)&cell);

    for (int i = 0; i < 5002; i++) {
        cell[i].next = &cell[i + 1];
    }

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;
    for (int i = 0; i < LOOP_COUNT; i++) {
        start_time = sceKernelGetProcessTimeLow();

        asm ("mov r2, %0\n\t"
             FIVE_THOUSAND("ldr r2, [r2]\n\t")
             :: "r" (&cell[0]));

        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    sceKernelFreeMemBlock(block);

    return ((float)total_time / total_n) * 1000.0f / 5000;
}

int run_latency_l2() {
    l2_llist_t *cell;
    SceUID block = sceKernelAllocMemBlock("lpddr", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, BLOCK_SIZE, NULL);
    sceKernelGetMemBlockBase(block, (void **)&cell);

    for (int i = 0; i < 2002; i++) {
        cell[i].next = &cell[i + 1];
    }

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;
    for (int i = 0; i < LOOP_COUNT; i++) {
        start_time = sceKernelGetProcessTimeLow();

        asm ("mov r2, %0\n\t"
             TWO_THOUSAND("ldr r2, [r2]\n\t")
             :: "r" (&cell[0]));

        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    sceKernelFreeMemBlock(block);

    return ((float)total_time / total_n) * 1000.0f / 2000;
}

int run_latency_cdram() {
    llist_t *cell;
    SceUID block = sceKernelAllocMemBlock("lpddr", SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, CDRAM_SIZE, NULL);
    sceKernelGetMemBlockBase(block, (void **)&cell);

    for (int i = 0; i < 5002; i++) {
        cell[i].next = &cell[i + 1];
    }

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;
    for (int i = 0; i < LOOP_COUNT; i++) {
        start_time = sceKernelGetProcessTimeLow();

        asm ("mov r2, %0\n\t"
             FIVE_THOUSAND("ldr r2, [r2]\n\t")
             :: "r" (&cell[0]));

        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    sceKernelFreeMemBlock(block);

    return ((float)total_time / total_n) * 1000.0f / 5000;
}
