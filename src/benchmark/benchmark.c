#include <psp2/kernel/processmgr.h>
#include <psp2/types.h>
#include <stdbool.h>
#include <stdint.h>

#include "benchmark.h"
#include "dhrystone.h"
#include "linpack.h"
#include "memcpy.h"
#include "latency.h"
#include "io.h"

static bool g_benchmark_running = false;
static benchmark_results_t g_benchmark_results;
static benchmark_test_t g_current_test = BENCHMARK_MAX;

static void run_on_thread_and_wait(SceUID thread, int (*fun)(), int *result);
static void run_on_thread(SceUID thread, int (*fun)(), int *result);

bool is_benchmark_running() {
    return g_benchmark_running;
}

benchmark_test_t get_running_test() {
    return g_current_test;
}

benchmark_results_t *get_benchmark_results() {
    return &g_benchmark_results;
}

static void run_benchmark_cpu(SceUID thread[3]) {
    g_current_test = BENCHMARK_CPU;
    sceKernelDelayThread(100000);

    // DHRYSTONE
    run_on_thread(thread[0], run_dhrystone_T0, &g_benchmark_results.dmips[0]);
    run_on_thread(thread[1], run_dhrystone_T1, &g_benchmark_results.dmips[1]);
    run_on_thread(thread[2], run_dhrystone_T2, &g_benchmark_results.dmips[2]);
    sceKernelWaitThreadEnd(thread[0], NULL, NULL);
    sceKernelWaitThreadEnd(thread[1], NULL, NULL);
    sceKernelWaitThreadEnd(thread[2], NULL, NULL);

    sceKernelDelayThread(100000);

    // LINPACK
    run_on_thread(thread[0], run_linpack, &g_benchmark_results.linpack[0]);
    run_on_thread(thread[1], run_linpack, &g_benchmark_results.linpack[1]);
    run_on_thread(thread[2], run_linpack, &g_benchmark_results.linpack[2]);
    sceKernelWaitThreadEnd(thread[0], NULL, NULL);
    sceKernelWaitThreadEnd(thread[1], NULL, NULL);
    sceKernelWaitThreadEnd(thread[2], NULL, NULL);
}

static void run_benchmark_cpu_st(SceUID thread) {
    g_current_test = BENCHMARK_CPU_ST;
    sceKernelDelayThread(100000);

    // DHRYSTONE ST
    run_on_thread_and_wait(thread, run_dhrystone_T0, &g_benchmark_results.dmips_st);

    // LINPACK ST
    run_on_thread_and_wait(thread, run_linpack, &g_benchmark_results.linpack_st);
}

static void run_benchmark_memory(SceUID thread) {
    g_current_test = BENCHMARK_MEMORY;
    sceKernelDelayThread(100000);

    // memcpy(): Uncache -> Uncache
    run_on_thread_and_wait(thread, run_memcpy_uc2uc, &g_benchmark_results.memcpy_uc2uc);
    // memcpy(): PhyCont -> PhyCont
    run_on_thread_and_wait(thread, run_memcpy_phy2phy, &g_benchmark_results.memcpy_phy2phy);
    // memcpy(): Cdram -> Cdram
    run_on_thread_and_wait(thread, run_memcpy_cd2cd, &g_benchmark_results.memcpy_cd2cd);
    // memcpy(): Uncache -> Cdram
    run_on_thread_and_wait(thread, run_memcpy_uc2cd, &g_benchmark_results.memcpy_uc2cd);

    // DMA: Uncache -> Uncache
    run_on_thread_and_wait(thread, run_dma_uc2uc, &g_benchmark_results.dma_uc2uc);
    // DMA: PhyCont -> PhyCont
    run_on_thread_and_wait(thread, run_dma_phy2phy, &g_benchmark_results.dma_phy2phy);
    // DMA: Cdram -> Cdram
    run_on_thread_and_wait(thread, run_dma_cd2cd, &g_benchmark_results.dma_cd2cd);
    // DMA: Uncache -> Cdram
    run_on_thread_and_wait(thread, run_dma_uc2cd, &g_benchmark_results.dma_uc2cd);
}

static void run_benchmark_latency(SceUID thread) {
    g_current_test = BENCHMARK_MEMORY_LATENCY;
    sceKernelDelayThread(100000);

    // LPDDR
    run_on_thread_and_wait(thread, run_latency_lpddr, &g_benchmark_results.latency_lpddr);
    // L1 cache
    run_on_thread_and_wait(thread, run_latency_l1, &g_benchmark_results.latency_l1);
    // L2 cache
    run_on_thread_and_wait(thread, run_latency_l2, &g_benchmark_results.latency_l2);
    // CDRAM
    run_on_thread_and_wait(thread, run_latency_cdram, &g_benchmark_results.latency_cdram);
}

static void run_benchmark_io(SceUID thread) {
    g_current_test = BENCHMARK_STORAGE;
    sceKernelDelayThread(100000);

    do_io_prepare();

    // ux0
    run_on_thread_and_wait(thread, run_io_ux0_write, &g_benchmark_results.io_write_ux0);
    run_on_thread_and_wait(thread, run_io_ux0_write_4k, &g_benchmark_results.io_write_ux0_4k);
    run_on_thread_and_wait(thread, run_io_ux0_read, &g_benchmark_results.io_read_ux0);
    run_on_thread_and_wait(thread, run_io_ux0_read_4k, &g_benchmark_results.io_read_ux0_4k);
    // uma0
    run_on_thread_and_wait(thread, run_io_uma0_write, &g_benchmark_results.io_write_uma0);
    run_on_thread_and_wait(thread, run_io_uma0_write_4k, &g_benchmark_results.io_write_uma0_4k);
    run_on_thread_and_wait(thread, run_io_uma0_read, &g_benchmark_results.io_read_uma0);
    run_on_thread_and_wait(thread, run_io_uma0_read_4k, &g_benchmark_results.io_read_uma0_4k);
    // imc0
    run_on_thread_and_wait(thread, run_io_imc0_write, &g_benchmark_results.io_write_imc0);
    run_on_thread_and_wait(thread, run_io_imc0_write_4k, &g_benchmark_results.io_write_imc0_4k);
    run_on_thread_and_wait(thread, run_io_imc0_read, &g_benchmark_results.io_read_imc0);
    run_on_thread_and_wait(thread, run_io_imc0_read_4k, &g_benchmark_results.io_read_imc0_4k);

    do_io_cleanup();
}

static void run_on_thread_and_wait(SceUID thread, int (*fun)(), int *result) {
    run_on_thread(thread, fun, result);
    sceKernelWaitThreadEnd(thread, NULL, NULL);
}

static void run_on_thread(SceUID thread, int (*fun)(), int *result) {
    benchmark_thread_data_t data;
    data.test_fun = fun;
    data.result = result;
    sceKernelStartThread(thread, sizeof(benchmark_thread_data_t), &data);
}

static int run_on_thread_entry(SceSize args, void *argp) {
    benchmark_thread_data_t data = *(benchmark_thread_data_t *)argp;
    *(data.result) = data.test_fun();
    return 0;
}

static int run_manager(SceSize args, void *argp) {
    benchmark_test_t test = *(benchmark_test_t *)argp;
    g_benchmark_running = true;

    SceUID worker[3];

    worker[0] = sceKernelCreateThread("worker1", run_on_thread_entry,
            SCE_KERNEL_HIGHEST_PRIORITY_USER, 0x100000, 0, SCE_KERNEL_CPU_MASK_USER_0, NULL);
    worker[1] = sceKernelCreateThread("worker2", run_on_thread_entry,
            SCE_KERNEL_HIGHEST_PRIORITY_USER, 0x100000, 0, SCE_KERNEL_CPU_MASK_USER_1, NULL);
    worker[2] = sceKernelCreateThread("worker3", run_on_thread_entry,
            SCE_KERNEL_HIGHEST_PRIORITY_USER, 0x100000, 0, SCE_KERNEL_CPU_MASK_USER_2, NULL);

    switch (test) {
        case BENCHMARK_CPU:
            run_benchmark_cpu(worker);
            break;
        case BENCHMARK_CPU_ST:
            run_benchmark_cpu_st(worker[0]);
            break;
        case BENCHMARK_MEMORY:
            run_benchmark_memory(worker[0]);
            break;
        case BENCHMARK_MEMORY_LATENCY:
            run_benchmark_latency(worker[0]);
            break;
        case BENCHMARK_STORAGE:
            run_benchmark_io(worker[0]);
            break;
        case BENCHMARK_MAX:
            run_benchmark_cpu(worker);
            run_benchmark_cpu_st(worker[0]);
            run_benchmark_memory(worker[0]);
            run_benchmark_latency(worker[0]);
            run_benchmark_io(worker[0]);
            break;
    }

    sceKernelDeleteThread(worker[0]);
    sceKernelDeleteThread(worker[1]);
    sceKernelDeleteThread(worker[2]);

    g_benchmark_running = false;
    g_current_test = BENCHMARK_MAX;
    sceKernelExitDeleteThread(0);
    return 0;
}

void run_benchmark(benchmark_test_t test) {
    SceUID manager = sceKernelCreateThread("manager", run_manager,
            0x10000100, 0x10000, 0, SCE_KERNEL_CPU_MASK_USER_ALL, NULL);
    sceKernelStartThread(manager, sizeof(benchmark_test_t), &test);
}
