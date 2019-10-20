#include <psp2/display.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/threadmgr.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "display.h"
#include "display_font.h"

static SceUID g_display_mutex = -1;
static int g_display_offscreen_buf_i = 1;
static SceUID g_display_vram_uid = -1;
static rgba_t *g_display_vram_base[2] = {NULL, NULL};

static float g_display_text_scale = 1.0f;
static rgba_t g_display_text_color = {.rgba = {.r = 255, .g = 255, .b = 255, .a = 255}};
static rgba_t g_display_back_color = {.rgba = {.r =   0, .g =   0, .b =   0, .a = 255}};

void display_init() {
    int buffer_size = (((DISPLAY_PITCH * DISPLAY_HEIGHT * 4) + ((256 * 1024) - 1)) >> 18 << 18);

    g_display_mutex = sceKernelCreateMutex("display_mutex", 0, 0, NULL);
    g_display_vram_uid = sceKernelAllocMemBlock(
            "display_vram",
            SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
            2 * buffer_size,
            NULL);

    sceKernelGetMemBlockBase(g_display_vram_uid, (void **)&g_display_vram_base[0]);
    g_display_vram_base[1] = (rgba_t *)((uint32_t)g_display_vram_base[0] + buffer_size);
}

void display_clear() {
    memset(g_display_vram_base[g_display_offscreen_buf_i], 0, sizeof(rgba_t) * DISPLAY_PITCH * DISPLAY_HEIGHT);
}

void display_sync() {
    SceDisplayFrameBuf fb = {
        .size        = sizeof(SceDisplayFrameBuf),
        .base        = (void *)g_display_vram_base[g_display_offscreen_buf_i],
        .pitch       = DISPLAY_PITCH,
        .pixelformat = SCE_DISPLAY_PIXELFORMAT_A8B8G8R8,
        .width       = DISPLAY_WIDTH,
        .height      = DISPLAY_HEIGHT
    };

    // Flip
    g_display_offscreen_buf_i = (g_display_offscreen_buf_i == 1) ? 0 : 1;

    sceDisplaySetFrameBuf(&fb, SCE_DISPLAY_SETBUF_NEXTFRAME);
    sceDisplayWaitSetFrameBuf();
}

void display_set_scale(float scale) {
    g_display_text_scale = scale;
}

void display_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    g_display_text_color.rgba.r = r;
    g_display_text_color.rgba.g = g;
    g_display_text_color.rgba.b = b;
    g_display_text_color.rgba.a = a;
}

void display_prchar(const char character, int x_pos, int y_pos, float scale) {
    uint32_t font_char_offset = character * FONT_HEIGHT * FONT_BYTES_PER_ROW;

    for (int y = 0; y < FONT_HEIGHT * scale; y++) {
        int y_font = y / scale;

        uint32_t fb_displacement = ((y_pos + y) * DISPLAY_PITCH) + x_pos;
        if (fb_displacement >= DISPLAY_PITCH * DISPLAY_HEIGHT)
            return; // oob

        rgba_t *fb = &g_display_vram_base[g_display_offscreen_buf_i][fb_displacement];

        for (int x = 0; x < FONT_WIDTH * scale; x++) {
            if (x_pos + x >= DISPLAY_WIDTH)
                return; // oob

            int x_font = x / scale;
            uint32_t font_offset = font_char_offset + (y_font * FONT_BYTES_PER_ROW) + (x_font / 8);

            fb[x] = GET_BIT(FONT[font_offset], 7 - (x_font % 8)) ?
                        g_display_text_color : g_display_back_color;
        }
    }
}

void display_printf(int x_pos, int y_pos, const char *format, ...) {
    char buffer[256] = "";
    va_list va;

    va_start(va, format);
    vsnprintf(buffer, 256, format, va);
    va_end(va);

    size_t len = strlen(buffer);
    for (size_t i = 0; i < len; i++) {
        display_prchar(buffer[i], (i * FONT_WIDTH * g_display_text_scale) + x_pos, y_pos, g_display_text_scale);
    }
}
