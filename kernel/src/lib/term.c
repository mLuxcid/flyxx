#include <sys/cpu.h>
#include <limine.h>
#include <lib/term.h>
#include <lib/terminus.h>

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
};

static uint32_t fg_col = 0xFFFFFF;
static struct terminfo terminfo;
static struct limine_framebuffer *fb = NULL;

void term_init(void) {
    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count == 0) {
            halt();
    }
    fb =
        framebuffer_request.response->framebuffers[0];


    terminfo = (struct terminfo) {
        .fb = fb->address,
        .font = font.data,
        .font_width = font.width,
        .font_height = font.height,
        .width = fb->width,
        .height = fb->height,
        .columns = fb->width / font.width,
        .rows = fb->height / font.height,
        .cursor = {
            .x = 0,
            .y = 0,
        },
        .bg_col = 0x000000,
        .fg_col = fg_col
    };
}

static void draw_char(uint32_t x, uint32_t y, char c) {
    uint8_t *glyph = &terminfo.font[(uint8_t)c * terminfo.font_height];
    for (uint16_t row = 0; row < terminfo.font_height; ++row) {
        uint8_t row_data = glyph[row];
        for (uint16_t col = 0; col < terminfo.font_width; ++col) {
            uint32_t pixelx = x + col;
            uint32_t pixely = y + row;
            if (row_data & (1 << (7 - col))) {
                terminfo.fb[pixely * (fb->pitch / (fb->bpp / 8)) + pixelx] = terminfo.fg_col;
            } else {
                terminfo.fb[pixely * (fb->pitch / (fb->bpp / 8)) + pixelx] = terminfo.bg_col;
            }
        }
    }
}

void term_write(const char *str) {
    while (*str) {
        if (*str == '\n') {
            terminfo.cursor.x = 0;
            terminfo.cursor.y += terminfo.font_height;
            if (terminfo.cursor.y >= terminfo.height) {
                terminfo.cursor.y = 0;
            }
        } else {
            draw_char(terminfo.cursor.x, terminfo.cursor.y, *str);
            terminfo.cursor.x += terminfo.font_width;
            if (terminfo.cursor.x > terminfo.width) {
                terminfo.cursor.x = 0;
                terminfo.cursor.y += terminfo.font_height;
                if (terminfo.cursor.y >= terminfo.height) {
                    terminfo.cursor.y = 0;
                }
            }
        }
        str++;
    }
}
