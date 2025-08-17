
#include "io.h"

/* The C function */
int sum_of_three(int arg1, int arg2, int arg3)
{
    return arg1 + arg2 + arg3;
}

int main() {
    /* Print 'H' at top left (row 0, col 0), white on black */
    fb_write_cell(PACK_FRAMEBUF_LOCATION(0, 0), 'H', FB_WHITE, FB_BLACK);
    fb_move_cursor(PACK_CURSOR_LOCATION(0, 0));

    /* Print 'i' at row 0, col 1, green on black */
    fb_write_cell(PACK_FRAMEBUF_LOCATION(0, 1), 'i', FB_GREEN, FB_BLACK);
    fb_move_cursor(PACK_CURSOR_LOCATION(0, 1));

    /* Print '!' at row 4, col 10, light red on blue */
    fb_write_cell(PACK_FRAMEBUF_LOCATION(4, 10), '!', FB_Light_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(4, 10));

    /* printing "MY OS" */
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 2), 'M', FB_Light_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 2));
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 3), 'Y', FB_Light_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 3));
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 4), ' ', FB_Light_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 4));
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 5), 'O', FB_Light_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 5));
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 6), 'S', FB_Light_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 6));

    return 0;
}