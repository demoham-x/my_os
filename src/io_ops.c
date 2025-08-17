/**
 * @file io.c
 *
 * @brief Implemnetaion of framebuffer I/O functions
 */

#include "io.h"

unsigned int current_row = 5;
unsigned int current_col = 0;

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
    volatile char *fb = (char *) FB_ADDR;

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

/**
 * @name scroll_scree
 *
 * @brief Scrolls the framebuffer up by one line
 */
void scroll_screen(void)
{
    unsigned int i;
    volatile char *fb = (char *) FB_ADDR;

    /* copy lines from 1 - FB_HEIGHT to 0 - (FB_HEIGHT - 1) */
    for (i = 0; i < ((FB_HEIGHT - 1) * FB_WIDTH * 2); i++)
    {
        fb[i] = fb[i + (FB_WIDTH * 2)];
    }
}

/**
 * @name clear_line
 *
 * @brief Clears the specifed row in the frame buffer
 */
void clear_line(unsigned int row)
{
    unsigned int i;

    /* Fill given line with ' ' */
    for (i = 0; i < (FB_WIDTH); i++)
    {
        fb_write_cell(PACK_FRAMEBUF_LOCATION(row, i), ' ', FB_WHITE, FB_BLACK);
    }
}

/**
 * @name fb_write
 *
 * @brief writes a string to the framebuffer
 *
 * @note The write function writes the contents
 * of the buffer buf of length len to the screen.
 * The write function should automatically advance
 * the cursor after a character has been written
 * and scroll the screen if necessary.
 *
 * @param buf the string to write
 * @param len the Length of the string
 * @param fg the foreground colour
 * @param bg the background colour
 */
void fb_write(char * buf, unsigned int len, unsigned char fg, unsigned char bg)
{
    unsigned int i;

    for (i = 0; i < len; i++)
    {
        if ((buf[i] == '\n') || (buf[i] == '\r') || (buf[i] == '\0'))
        {
            /* Check if next character has '\r' or '\n' */
            if((buf[i] == '\r') && (i + 1 < len) && (buf[i + 1] == '\n'))
            {
                continue;
            }
            if((buf[i] == '\n') && (i + 1 < len) && (buf[i + 1] == '\r'))
            {
                continue;
            }
            current_col = 0;
            current_row++;
        }
        else
        {
            /* Write character */
            fb_write_cell(PACK_FRAMEBUF_LOCATION(current_row, current_col), buf[i], fg, bg);
            /* Move cursor */
            fb_move_cursor(PACK_CURSOR_LOCATION(current_row, current_col));

            /* move to next column */
            current_col++;

            /* If we reach end of the column move to next row */
            if (current_col >= FB_WIDTH) {
                current_col = 0;
                current_row++;
            }
        }

        /* Check if we are in last row */
        if (current_row >= FB_HEIGHT)
        {
            /* Move current row to previous row */
            current_row = FB_HEIGHT - 1;
            /* Move rows one step above */
            scroll_screen();
            /* Clear the last line */
            clear_line(FB_HEIGHT-1);
        }
    }
}