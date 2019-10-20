#ifndef BENCHMARK_H
#define BENCHMARK_H

#define SCE_KERNEL_HIGHEST_PRIORITY_USER (64)

#define SCE_KERNEL_CPU_MASK_SHIFT		(16)
#define SCE_KERNEL_CPU_MASK_USER_0		(0x01 << SCE_KERNEL_CPU_MASK_SHIFT)
#define SCE_KERNEL_CPU_MASK_USER_1		(0x02 << SCE_KERNEL_CPU_MASK_SHIFT)
#define SCE_KERNEL_CPU_MASK_USER_2		(0x04 << SCE_KERNEL_CPU_MASK_SHIFT)
#define SCE_KERNEL_CPU_MASK_USER_ALL	\
	(SCE_KERNEL_CPU_MASK_USER_0 | SCE_KERNEL_CPU_MASK_USER_1 | SCE_KERNEL_CPU_MASK_USER_2)

typedef enum {
    BENCHMARK_CPU,
    BENCHMARK_CPU_ST,
    BENCHMARK_MEMORY,
    BENCHMARK_MEMORY_LATENCY,
    BENCHMARK_STORAGE,
    BENCHMARK_MAX
} benchmark_test_t;

typedef struct {
    int dmips[3];
    int dmips_st;
    int linpack[3];
    int linpack_st;

    int memcpy_uc2uc;
    int memcpy_phy2phy;
    int memcpy_cd2cd;
    int memcpy_uc2cd;
    int dma_uc2uc;
    int dma_phy2phy;
    int dma_cd2cd;
    int dma_uc2cd;

    int latency_lpddr;
    int latency_l1;
    int latency_l2;
    int latency_cdram;

    int io_write_ux0_4k;
    int io_read_ux0_4k;
    int io_write_ux0;
    int io_read_ux0;
    int io_write_uma0_4k;
    int io_read_uma0_4k;
    int io_write_uma0;
    int io_read_uma0;
    int io_write_imc0_4k;
    int io_read_imc0_4k;
    int io_write_imc0;
    int io_read_imc0;
} benchmark_results_t;

typedef struct {
    int (*test_fun)();
    int *result;
} benchmark_thread_data_t;

bool is_benchmark_running();
benchmark_test_t get_running_test();
benchmark_results_t *get_benchmark_results();

void run_benchmark(benchmark_test_t test);

#endif
