#include <hal/cpu.h>
#include <limine.h>
#include <hal/term.h>
#include <hal/font.h>

__attribute__((section(".requests"), used))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
};

// TODO: locking

struct terminfo {
    uint32_t *fb;
    uint8_t *font;
    uint8_t font_width, font_height;

    size_t width, height;
    size_t columns, rows;
    struct {
        size_t x, y;
    } cursor;

    uint32_t bg_col, fg_col;
    bool flip;
};

// static uint32_t fgcol = 0xa9b1d6;
static uint32_t fgcol = 0x000000;
static struct terminfo info;
static struct limine_framebuffer *fb = NULL;

static void clr(void) {
    for (size_t x = 0; x < info.width; x++) {
        for (size_t y = 0; y < info.height; y++) {
            info.fb[y * (fb->pitch / 4) + x] = info.bg_col;
        }
    }
}

void term_init(void) {
    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count == 0) {
        halt();
    }
    fb =
        framebuffer_request.response->framebuffers[0];


    info = (struct terminfo) {
        .fb = fb->address,
        .font = font_small.data,
        .font_width = font_small.width,
        .font_height = font_small.height,
        .width = fb->width,
        .height = fb->height,
        .columns = fb->width / font_small.width,
        .rows = fb->height / font_small.height,
        .cursor = {
            .x = 0,
            .y = 0,
        },
        .bg_col = 0xeaebe6,
        .fg_col = fgcol,
        .flip = true,
    };


    clr();
}

static void draw_char(uint32_t x, uint32_t y, char c) {
    uint8_t *glyph = &info.font[(uint8_t)c * info.font_height];
    for (uint16_t row = 0; row < info.font_height; ++row) {
        uint8_t rowdata = glyph[row];
        for (uint16_t col = 0; col < info.font_width; ++col) {
            uint32_t pixelx = info.flip ? (x + info.font_width - 1 - col) : x + col;
            uint32_t pixely = y + row;
            if (rowdata & (1 << (info.font_width - 1 - col))) {
                info.fb[pixely * (fb->pitch / (fb->bpp / 8)) + pixelx] = info.fg_col;
            } else {
                info.fb[pixely * (fb->pitch / (fb->bpp / 8)) + pixelx] = info.bg_col;
            }
        }
    }
}

void term_write(const char *str) {
    while (*str) {
        if (*str == '\n') {
            info.cursor.x = 0;
            info.cursor.y += info.font_height;
            if (info.cursor.y >= info.height) {
                info.cursor.y = 0;
            }
        } else {
            draw_char(info.cursor.x, info.cursor.y, *str);
            info.cursor.x += info.font_width;
            if (info.cursor.x > info.width) {
                info.cursor.x = 0;
                info.cursor.y += info.font_height;
                if (info.cursor.y >= info.height) {
                    clr();
                    info.cursor.y = 0;
                }
            }
        }
        str++;
    }
}
