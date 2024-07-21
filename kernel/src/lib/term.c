#include <sys/cpu.h>
#include <limine.h>
#include <lib/term.h>
#include <flanterm.h>
#include <backends/fb.h>
#include <lib/terminus.h>

__attribute__((section(".requests"), used))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
};

static struct flanterm_context *context = NULL;
// TODO: locking

// flanterm uses a gray foreground color by default. We want to set it to white
static uint32_t fg_col = 0xFFFFFF;

void term_init(void)
{
    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count == 0) {
            halt();
    }
    struct limine_framebuffer *fb =
        framebuffer_request.response->framebuffers[0];

    context = flanterm_fb_init(NULL, NULL,
        fb->address,
        fb->width, fb->height,
        fb->pitch,
        fb->red_mask_size, fb->red_mask_shift,
        fb->green_mask_size, fb->green_mask_shift,
        fb->blue_mask_size, fb->blue_mask_shift,
        NULL, NULL, NULL,
        NULL, &fg_col, NULL, NULL,
        font.data, font.width, font.height, 0, 1, 1, 0);

    if (context == NULL) {
        halt();
    }
}

void term_write(const char *str, size_t length)
{
    flanterm_write(context, str, length);
}
