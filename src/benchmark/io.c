#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/io/fcntl.h>
#include <stdlib.h>

#define UX0_PATH "ux0:psvbench.tmp"
#define UMA0_PATH "uma0:psvbench.tmp"
#define IMC0_PATH "imc0:psvbench.tmp"

#define IO_4K_LOOPS 256
#define IO_4K_SIZE  4 * 1024 // 4 kB
#define IO_SEQ_SIZE 50 * 1024 * 1024 // 50 MB

static SceUID g_lpddr_block = -1;
static void *g_lpddr_base = NULL;

int run_io_seq_write(const char *path, int loops) {
    if (!g_lpddr_base)
        return -1;

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;
    int bytes = 0;

    for (int i = 0; i < loops; i++) {
        start_time = sceKernelGetProcessTimeLow();

        SceUID fd = sceIoOpen(path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
        if (fd < 0)
            return fd;

        bytes = sceIoWrite(fd, g_lpddr_base, IO_SEQ_SIZE);
        if (bytes < IO_SEQ_SIZE) {
            sceIoClose(fd);
            return -1;
        }
        sceIoClose(fd);

        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    return (1000000.0f / (total_time / total_n)) * (IO_SEQ_SIZE / 1024.0f); // kB/s
}

int run_io_4k_write(const char *path, int loops) {
    if (!g_lpddr_base)
        return -1;

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;
    int bytes = 0;

    for (int i = 0; i < loops; i++) {
        start_time = sceKernelGetProcessTimeLow();

        SceUID fd = sceIoOpen(path, SCE_O_WRONLY | SCE_O_CREAT, 0777);
        if (fd < 0)
            return fd;

        for (int i = 0; i < IO_4K_LOOPS; i++) {
            bytes = sceIoPwrite(fd, g_lpddr_base, IO_4K_SIZE, rand() % (IO_SEQ_SIZE - IO_4K_SIZE));
            if (bytes < IO_4K_SIZE) {
                sceIoClose(fd);
                return -1;
            }
        }
        sceIoClose(fd);

        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    return (1000000.0f / (total_time / total_n)) * (IO_4K_SIZE / 1024.0f) * IO_4K_LOOPS; // kB/s
}

int run_io_seq_read(const char *path, int loops) {
    if (!g_lpddr_base)
        return -1;

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;
    int bytes = 0;

    for (int i = 0; i < loops; i++) {
        start_time = sceKernelGetProcessTimeLow();

        SceUID fd = sceIoOpen(path, SCE_O_RDONLY, 0777);
        if (fd < 0)
            return fd;

        bytes = sceIoRead(fd, g_lpddr_base, IO_SEQ_SIZE);
        if (bytes < IO_SEQ_SIZE) {
            sceIoClose(fd);
            return -1;
        }
        sceIoClose(fd);

        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    return (1000000.0f / (total_time / total_n)) * (IO_SEQ_SIZE / 1024.0f); // kB/s
}

int run_io_4k_read(const char *path, int loops) {
    if (!g_lpddr_base)
        return -1;

    uint32_t total_time = 0, total_n = 0;
    uint32_t start_time;
    int bytes = 0;

    for (int i = 0; i < loops; i++) {
        start_time = sceKernelGetProcessTimeLow();

        SceUID fd = sceIoOpen(path, SCE_O_RDONLY, 0777);
        if (fd < 0)
            return fd;

        for (int i = 0; i < IO_4K_LOOPS; i++) {
            bytes = sceIoPread(fd, g_lpddr_base, IO_4K_SIZE, rand() % (IO_SEQ_SIZE - IO_4K_SIZE));
            if (bytes < IO_4K_SIZE) {
                sceIoClose(fd);
                return -1;
            }
        }
        sceIoClose(fd);

        total_time += sceKernelGetProcessTimeLow() - start_time;
        total_n++;
    }

    return (1000000.0f / (total_time / total_n)) * (IO_4K_SIZE / 1024.0f) * IO_4K_LOOPS; // kB/s
}

int run_io_ux0_write() { return run_io_seq_write(UX0_PATH, 3); }
int run_io_ux0_write_4k() { return run_io_4k_write(UX0_PATH, 1); }
int run_io_ux0_read() { return run_io_seq_read(UX0_PATH, 3); }
int run_io_ux0_read_4k() { return run_io_4k_read(UX0_PATH, 1); }

int run_io_uma0_write() { return run_io_seq_write(UMA0_PATH, 3); }
int run_io_uma0_write_4k() { return run_io_4k_write(UMA0_PATH, 1); }
int run_io_uma0_read() { return run_io_seq_read(UMA0_PATH, 3); }
int run_io_uma0_read_4k() { return run_io_4k_read(UMA0_PATH, 1); }

int run_io_imc0_write() { return run_io_seq_write(IMC0_PATH, 3); }
int run_io_imc0_write_4k() { return run_io_4k_write(IMC0_PATH, 1); }
int run_io_imc0_read() { return run_io_seq_read(IMC0_PATH, 3); }
int run_io_imc0_read_4k() { return run_io_4k_read(IMC0_PATH, 1); }

void do_io_prepare() {
    g_lpddr_block = sceKernelAllocMemBlock("io_data", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, IO_SEQ_SIZE, NULL);
    sceKernelGetMemBlockBase(g_lpddr_block, &g_lpddr_base);
}

void do_io_cleanup() {
    sceKernelFreeMemBlock(g_lpddr_block);
    g_lpddr_base = NULL;

    sceIoRemove(UX0_PATH);
    sceIoRemove(UMA0_PATH);
    sceIoRemove(IMC0_PATH);
}
