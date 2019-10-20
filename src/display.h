#ifndef DISPLAY_H
#define DISPLAY_H

#define FONT_WIDTH  12
#define FONT_HEIGHT 24
#define FONT_BYTES_PER_ROW (((FONT_WIDTH - 1) / 8) + 1)

#define DISPLAY_WIDTH  960
#define DISPLAY_HEIGHT 544
#define DISPLAY_PITCH  ((DISPLAY_WIDTH + 31) >> 5 << 5)

#define GET_BIT(bits, pos) (((bits) >> (pos)) & 0b1)

#define ANCHOR_LX(off, len)           (off + (len) * FONT_WIDTH)
#define ANCHOR_LX2(off, len, scale)   (off + (len) * FONT_WIDTH * (scale))
#define ANCHOR_RX(off, len)           (DISPLAY_WIDTH - (off) - (len) * FONT_WIDTH)
#define ANCHOR_RX2(off, len, scale)   (DISPLAY_WIDTH - (off) - (len) * FONT_WIDTH * (scale))
#define ANCHOR_TY(off, lines)         (off + (lines) * FONT_HEIGHT)
#define ANCHOR_TY2(off, lines, scale) (off + (lines) * FONT_HEIGHT * (scale))
#define ANCHOR_BY(off, lines)         (DISPLAY_HEIGHT - (off) - (lines) * FONT_HEIGHT)
#define ANCHOR_BY2(off, lines, scale) (DISPLAY_HEIGHT - (off) - (lines) * FONT_HEIGHT * (scale))
#define ANCHOR_CX(len)                (DISPLAY_WIDTH / 2 - ((len) * FONT_WIDTH) / 2)
#define ANCHOR_CX2(len, scale)        (DISPLAY_WIDTH / 2 - ((len) * FONT_WIDTH * (scale)) / 2)
#define ANCHOR_CY(lines)              (DISPLAY_HEIGHT / 2 - ((lines) * FONT_HEIGHT) / 2)
#define ANCHOR_CY2(lines, scale)      (DISPLAY_HEIGHT / 2 - ((lines) * FONT_HEIGHT * (scale)) / 2)

typedef union {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } rgba;
    uint32_t uint32;
} rgba_t;

void display_init();
void display_clear();
void display_sync();
void display_set_scale(float scale);
void display_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void display_prchar(const char character, int x_pos, int y_pos, float scale);
void display_printf(int x_pos, int y_pos, const char *format, ...);

#endif
