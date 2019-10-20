#include <psp2/ctrl.h>
#include <psp2/display.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>
#include <stdbool.h>

#include "main.h"
#include "display.h"
#include "benchmark/benchmark.h"

static view_t                    g_view          = VIEW_MENU_MAIN;
static menu_control_t            g_menu_control  = MENU_CTRL_FULL_BENCHMARK;
static menu_individual_control_t g_menu_control2 = MENU_CTRL_INDIVIDUAL_CPU;

static uint32_t g_buttons_last = 0;

void _draw_menu_item(const char *label, int x, int y, int len, bool selected) {
    display_printf(x, y, label);
    if (selected) {
        display_set_color(0, 200, 255, 255);
        display_printf(x - ANCHOR_LX(0, 2), y, ">");
        display_printf(x + ANCHOR_LX(0, 1 + len), y, "<");
        display_set_color(255, 255, 255, 255);
    }
}

void draw_view() {
    if (is_benchmark_running()) {
        g_view = VIEW_RUNNING;
    }

    switch (g_view) {
        default: break;
        case VIEW_MENU_MAIN:
            _draw_menu_item("Start benchmark",
                        ANCHOR_CX(15), ANCHOR_CY(3) + ANCHOR_TY(0, 0), 15,
                        g_menu_control == MENU_CTRL_FULL_BENCHMARK);
            _draw_menu_item("Individual tests",
                        ANCHOR_CX(16), ANCHOR_CY(3) + ANCHOR_TY(0, 1), 16,
                        g_menu_control == MENU_CTRL_INDIVIDUAL_TESTS);
            _draw_menu_item("Results",
                        ANCHOR_CX(7), ANCHOR_CY(3) + ANCHOR_TY(0, 2), 7,
                        g_menu_control == MENU_CTRL_RESULTS);
            _draw_menu_item("Quit",
                        ANCHOR_CX(4), ANCHOR_CY(3) + ANCHOR_TY(0, 3), 4,
                        g_menu_control == MENU_CTRL_QUIT);
            break;
        case VIEW_MENU_INDIVIDUAL:
            _draw_menu_item("CPU",
                        ANCHOR_CX(3), ANCHOR_CY(6) + ANCHOR_TY(0, 0), 3,
                        g_menu_control2 == MENU_CTRL_INDIVIDUAL_CPU);
            _draw_menu_item("CPU single-thread",
                        ANCHOR_CX(17), ANCHOR_CY(6) + ANCHOR_TY(0, 1), 17,
                        g_menu_control2 == MENU_CTRL_INDIVIDUAL_CPU_ST);
            _draw_menu_item("Memory throughput",
                        ANCHOR_CX(17), ANCHOR_CY(6) + ANCHOR_TY(0, 2), 17,
                        g_menu_control2 == MENU_CTRL_INDIVIDUAL_MEMORY);
            _draw_menu_item("Memory latency",
                        ANCHOR_CX(14), ANCHOR_CY(6) + ANCHOR_TY(0, 3), 14,
                        g_menu_control2 == MENU_CTRL_INDIVIDUAL_MEMORY2);
            _draw_menu_item("Storage I/O",
                        ANCHOR_CX(11), ANCHOR_CY(6) + ANCHOR_TY(0, 4), 11,
                        g_menu_control2 == MENU_CTRL_INDIVIDUAL_IO);
            _draw_menu_item("go back",
                        ANCHOR_CX(7), ANCHOR_CY(6) + ANCHOR_TY(0, 6), 7,
                        g_menu_control2 == MENU_CTRL_INDIVIDUAL_BACK);
            break;
        case VIEW_RUNNING:
            if (!is_benchmark_running()) {
                g_view = VIEW_RESULTS;
                break;
            }

            display_set_scale(2.0f);
            switch (get_running_test()) {
                default:
                    display_printf(ANCHOR_CX2(3, 2.0f), ANCHOR_CY2(1, 2.0f), "...");
                    break;
                case BENCHMARK_CPU:
                    display_printf(ANCHOR_CX2(20, 2.0f), ANCHOR_CY2(1, 2.0f), "Running CPU tests...");
                    break;
                case BENCHMARK_CPU_ST:
                    display_printf(ANCHOR_CX2(23, 2.0f), ANCHOR_CY2(1, 2.0f), "Running CPU ST tests...");
                    break;
                case BENCHMARK_MEMORY:
                    display_printf(ANCHOR_CX2(23, 2.0f), ANCHOR_CY2(1, 2.0f), "Running memory tests...");
                    break;
                case BENCHMARK_MEMORY_LATENCY:
                    display_printf(ANCHOR_CX2(24, 2.0f), ANCHOR_CY2(1, 2.0f), "Running latency tests...");
                    break;
                case BENCHMARK_STORAGE:
                    display_printf(ANCHOR_CX2(20, 2.0f), ANCHOR_CY2(1, 2.0f), "Running I/O tests...");
                    break;
            }
            display_set_scale(1.0f);

            break;
        case VIEW_RESULTS: ;
            benchmark_results_t *results = get_benchmark_results();

            display_set_scale(2.0f);
            display_printf(ANCHOR_LX(25, 0),        ANCHOR_TY(100, 0), "CPU");
            display_printf(ANCHOR_LX(480 + 125, 0), ANCHOR_TY(100, 0), "CPU ST");
            display_printf(ANCHOR_LX(25, 0),        ANCHOR_TY(226, 0), "Memory");
            display_printf(ANCHOR_LX(480 + 125, 0), ANCHOR_TY(226, 0), "Latency");
            display_printf(ANCHOR_LX(25, 0),        ANCHOR_TY(400, 0), "Storage");
            display_set_scale(1.0f);

            // CPU
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(148, 0), "Dhrystone:        DMIPS");
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(148, 1), "Linpack:          MFLOPS");

            // CPU ST
            display_printf(ANCHOR_LX(480 + 140, 0), ANCHOR_TY(148, 0), "Dhrystone:        DMIPS");
            display_printf(ANCHOR_LX(480 + 140, 0), ANCHOR_TY(148, 1), "Linpack:          MFLOPS");

            // Memory
            display_printf(ANCHOR_LX(40, 18), ANCHOR_TY(242, 0), "memcpy    dma");
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(274, 0), "LPDDR -> LPDDR:      MB/s       MB/s");
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(274, 1), "  PHY -> PHY:        MB/s       MB/s");
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(274, 2), "CDRAM -> CDRAM:      MB/s       MB/s");
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(274, 3), "LPDDR -> CDRAM:      MB/s       MB/s");

            // Latency
            display_printf(ANCHOR_LX(480 + 140, 17), ANCHOR_TY(242, 0), "dword");
            display_printf(ANCHOR_LX(480 + 140, 0), ANCHOR_TY(274, 0), "L1 cache:            ns");
            display_printf(ANCHOR_LX(480 + 140, 0), ANCHOR_TY(274, 1), "L2 cache:            ns");
            display_printf(ANCHOR_LX(480 + 140, 0), ANCHOR_TY(274, 2), "LPDDR:               ns");
            display_printf(ANCHOR_LX(480 + 140, 0), ANCHOR_TY(274, 3), "CDRAM:               ns");

            // Storage
            display_printf(ANCHOR_LX(40, 18), ANCHOR_TY(416, 0),
                    "write 4K    read 4K    write seq   read seq");
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(448, 0),
                    "ux0:                   MB/s        MB/s        MB/s        MB/s");
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(448, 1),
                    "uma0:                  MB/s        MB/s        MB/s        MB/s");
            display_printf(ANCHOR_LX(40, 0), ANCHOR_TY(448, 2),
                    "imc0:                  MB/s        MB/s        MB/s        MB/s");

            display_set_color(50, 220, 255, 255);

            // CPU results
            if (results->dmips[0])
                display_printf(ANCHOR_LX(40, 13), ANCHOR_TY(148, 0), "%4d",
                    results->dmips[0] + results->dmips[1] + results->dmips[2]);
            if (results->linpack[0])
                display_printf(ANCHOR_LX(40, 13), ANCHOR_TY(148, 1), "%4d",
                    results->linpack[0] + results->linpack[1] + results->linpack[2]);

            // CPU 1T results
            if (results->dmips_st)
                display_printf(ANCHOR_LX(480 + 140, 13), ANCHOR_TY(148, 0), "%4d", results->dmips_st);
            if (results->linpack_st)
                display_printf(ANCHOR_LX(480 + 140, 13), ANCHOR_TY(148, 1), "%4d", results->linpack_st);

            // Memory results
            if (results->memcpy_uc2uc)
                display_printf(ANCHOR_LX(40, 17), ANCHOR_TY(274, 0), "%3d", results->memcpy_uc2uc);
            if (results->memcpy_phy2phy)
                display_printf(ANCHOR_LX(40, 17), ANCHOR_TY(274, 1), "%3d", results->memcpy_phy2phy);
            if (results->memcpy_cd2cd)
                display_printf(ANCHOR_LX(40, 17), ANCHOR_TY(274, 2), "%3d", results->memcpy_cd2cd);
            if (results->memcpy_uc2cd)
                display_printf(ANCHOR_LX(40, 17), ANCHOR_TY(274, 3), "%3d", results->memcpy_uc2cd);
            if (results->dma_uc2uc)
                display_printf(ANCHOR_LX(40, 28), ANCHOR_TY(274, 0), "%3d", results->dma_uc2uc);
            if (results->dma_phy2phy)
                display_printf(ANCHOR_LX(40, 28), ANCHOR_TY(274, 1), "%3d", results->dma_phy2phy);
            if (results->dma_cd2cd)
                display_printf(ANCHOR_LX(40, 28), ANCHOR_TY(274, 2), "%3d", results->dma_cd2cd);
            if (results->dma_uc2cd)
                display_printf(ANCHOR_LX(40, 28), ANCHOR_TY(274, 3), "%3d", results->dma_uc2cd);

            // Latency results
            if (results->latency_l1)
                display_printf(ANCHOR_LX(480 + 140, 17), ANCHOR_TY(274, 0), "%3d", results->latency_l1);
            if (results->latency_l2)
                display_printf(ANCHOR_LX(480 + 140, 17), ANCHOR_TY(274, 1), "%3d", results->latency_l2);
            if (results->latency_lpddr)
                display_printf(ANCHOR_LX(480 + 140, 17), ANCHOR_TY(274, 2), "%3d", results->latency_lpddr);
            if (results->latency_cdram)
                display_printf(ANCHOR_LX(480 + 140, 17), ANCHOR_TY(274, 3), "%3d", results->latency_cdram);

            // Storage results
            if (results->io_write_ux0_4k > 0)
                display_printf(ANCHOR_LX(40, 16), ANCHOR_TY(448, 0), "%6.2f", results->io_write_ux0_4k / 1024.0f);
            if (results->io_read_ux0_4k > 0)
                display_printf(ANCHOR_LX(40, 28), ANCHOR_TY(448, 0), "%6.2f", results->io_read_ux0_4k / 1024.0f);
            if (results->io_write_ux0 > 0)
                display_printf(ANCHOR_LX(40, 40), ANCHOR_TY(448, 0), "%6.2f", results->io_write_ux0 / 1024.0f);
            if (results->io_read_ux0 > 0)
                display_printf(ANCHOR_LX(40, 52), ANCHOR_TY(448, 0), "%6.2f", results->io_read_ux0 / 1024.0f);
            // uma0
            if (results->io_write_uma0_4k > 0)
                display_printf(ANCHOR_LX(40, 16), ANCHOR_TY(448, 1), "%6.2f", results->io_write_uma0_4k / 1024.0f);
            if (results->io_read_uma0_4k > 0)
                display_printf(ANCHOR_LX(40, 28), ANCHOR_TY(448, 1), "%6.2f", results->io_read_uma0_4k / 1024.0f);
            if (results->io_write_uma0 > 0)
                display_printf(ANCHOR_LX(40, 40), ANCHOR_TY(448, 1), "%6.2f", results->io_write_uma0 / 1024.0f);
            if (results->io_read_uma0 > 0)
                display_printf(ANCHOR_LX(40, 52), ANCHOR_TY(448, 1), "%6.2f", results->io_read_uma0 / 1024.0f);
            // imc0
            if (results->io_write_imc0_4k > 0)
                display_printf(ANCHOR_LX(40, 16), ANCHOR_TY(448, 2), "%6.2f", results->io_write_imc0_4k / 1024.0f);
            if (results->io_read_imc0_4k > 0)
                display_printf(ANCHOR_LX(40, 28), ANCHOR_TY(448, 2), "%6.2f", results->io_read_imc0_4k / 1024.0f);
            if (results->io_write_imc0 > 0)
                display_printf(ANCHOR_LX(40, 40), ANCHOR_TY(448, 2), "%6.2f", results->io_write_imc0 / 1024.0f);
            if (results->io_read_imc0 > 0)
                display_printf(ANCHOR_LX(40, 52), ANCHOR_TY(448, 2), "%6.2f", results->io_read_imc0 / 1024.0f);

            display_set_color(255, 255, 255, 255);
            break;
    }
}

void input_cross() {
    switch (g_view) {
        default: break;
        case VIEW_MENU_MAIN:
            switch (g_menu_control) {
                default: break;
                case MENU_CTRL_FULL_BENCHMARK:
                    run_benchmark(BENCHMARK_MAX);
                    break;
                case MENU_CTRL_INDIVIDUAL_TESTS:
                    g_view = VIEW_MENU_INDIVIDUAL;
                    break;
                case MENU_CTRL_RESULTS:
                    g_view = VIEW_RESULTS;
                    break;
                case MENU_CTRL_QUIT:
                    sceKernelExitProcess(0);
                    break;
            }
            break;
        case VIEW_MENU_INDIVIDUAL:
            switch (g_menu_control2) {
                default: break;
                case MENU_CTRL_INDIVIDUAL_CPU:
                    run_benchmark(BENCHMARK_CPU);
                    break;
                case MENU_CTRL_INDIVIDUAL_CPU_ST:
                    run_benchmark(BENCHMARK_CPU_ST);
                    break;
                case MENU_CTRL_INDIVIDUAL_MEMORY:
                    run_benchmark(BENCHMARK_MEMORY);
                    break;
                case MENU_CTRL_INDIVIDUAL_MEMORY2:
                    run_benchmark(BENCHMARK_MEMORY_LATENCY);
                    break;
                case MENU_CTRL_INDIVIDUAL_IO:
                    run_benchmark(BENCHMARK_STORAGE);
                    break;
                case MENU_CTRL_INDIVIDUAL_BACK:
                    g_view = VIEW_MENU_MAIN;
                    break;
            }
    }
}

void input_circle() {
    switch (g_view) {
        default: break;
        case VIEW_MENU_INDIVIDUAL:
            g_view = VIEW_MENU_MAIN;
            break;
        case VIEW_RESULTS:
            g_view = VIEW_MENU_MAIN;
            break;
    }
}

void input_scroll(bool up) {
    switch (g_view) {
        default: break;
        case VIEW_MENU_MAIN:
            if (!up && g_menu_control < MENU_CTRL_MAX - 1)
                g_menu_control++;
            else if (up && g_menu_control > 0)
                g_menu_control--;
            break;
        case VIEW_MENU_INDIVIDUAL:
            if (!up && g_menu_control2 < MENU_CTRL_INDIVIDUAL_MAX - 1)
                g_menu_control2++;
            else if (up && g_menu_control2 > 0)
                g_menu_control2--;
            break;
    }
}

void check_input() {
    SceCtrlData data;
    int ret = sceCtrlPeekBufferPositive(0, &data, 1);
    if (ret <= 0)
        sceCtrlPeekBufferPositive(1, &data, 1);

    uint32_t buttons_new = data.buttons & ~g_buttons_last;

    if (buttons_new & SCE_CTRL_CROSS)
        input_cross();
    else if (buttons_new & SCE_CTRL_CIRCLE)
        input_circle();
    else if (buttons_new & SCE_CTRL_UP)
        input_scroll(true);
    else if (buttons_new & SCE_CTRL_DOWN)
        input_scroll(false);

    g_buttons_last = data.buttons;
}

int main(int argc, char *argv[]) {
    display_init();

    while (true) {
        display_clear();

        display_set_scale(2.0f);
        display_printf(ANCHOR_CX2(13, 2.0f), ANCHOR_TY(5, 0), "PSVbench v1.0");
        display_set_scale(1.0f);
        display_printf(ANCHOR_CX(11), ANCHOR_TY2(5, 1, 2.0f), "by Electry");

        check_input();

        draw_view();

        display_sync();
        if (is_benchmark_running()) {
            sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND);
            sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_OLED_OFF);
            sceDisplayWaitVblankStartMulti(5);
        }
    }

    sceKernelExitProcess(0);
    return 0;
}
