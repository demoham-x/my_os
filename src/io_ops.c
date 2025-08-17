/**
 * @file io.c
 *
 * @brief Implemnetaion of framebuffer I/O functions
 */

#include "io.h"

/**
 * @name fb_write_cell:
 *
 *  @brief Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    /* Framebuffer address */
    char *fb = (char *) FB_ADDR;

    /* Ensure i is within bounds */
    if (i >= FB_SIZE)
    {
        /* Out of bounds, do nothing */
        return;
    }

    /* Each cell is 2 bytes: character + attributes */
    fb[i] = c;
    fb[i + 1] = PACK_FG_BG(fg, bg);
}

/**
 * @name Cursor Manipulation (More Advanced Output)
 * The visible cursor is positioned using hardware I/O ports.
 * Ports:
 * 0x3D4 – Command port (specifies which cursor byte you’re writing)
 * 0x3D5 – Data port (provides the byte value)
 * The cursor’s position (0-based, column + 80·row) is split into two bytes.
 */

/** @name fb_move_cursor:
 *  @brief Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}