#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/**
 * @name The Framebuffer
 * @par The framebuffer is a block of memory mapped to video hardware.
 * Text output is done by writing bytes to specific addresses corresponding to onscreen positions.
 * Video memory starts at: 0x000B8000
 * The screen is 80 columns x 25 rows.
 *
 * @name How Screen Characters are Arranged
 * @par Each cell on the screen is 16 bits (2 bytes):
 * High byte (bits 8-15): ASCII character
 * Low byte (bits 0-7): Color attributes (background: bits 4-7, foreground: bits 0-3)
 */

/** Frame buffer width */
#define FB_WIDTH 80U
/** Frame buffer height */
#define FB_HEIGHT 25U
/** Frame buffer size */
#define FB_SIZE (FB_WIDTH * FB_HEIGHT * 2U)

/**
 */
/**
 * @name Macro to pack Foreground and Background
 *
 * @par Frame buffer structure:
 * Bit:     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
 * Content: | ASCII                 | FG      | BG      |
*/
#define PACK_FG_BG(fg, bg) \
        ((fg & 0x0FU) << 4U) | (bg & 0x0FU)

/** Frame buffer colours */
#define FB_BLACK            0U
#define FB_RED              4U
#define FB_DARK_GREY        8U
#define FB_Light_RED        12U
#define FB_BLUE             1U
#define FB_MAGENTA          5U
#define FB_LIGHT_BLUE       9U
#define FB_LIGHT_MAGENTA    13U
#define FB_GREEN            2U
#define FB_Brown            6U
#define FB_LIGHT_GREEN      10U
#define FB_LIGHT_BROWN      14U
#define FB_CYAN             3U
#define FB_LIGHT_GREY       7U
#define FB_LIGHT_CYAN       11U
#define FB_WHITE            15U

/** Frame buffer address */
#define FB_ADDR 0x000B8000

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/**
 * @name Cursor pack loaction
 *
 * @note row and col starts from 0
 * >0 means row zero, column zero;
 * >1 means row zero, column one;
 * >80 means row one, column zero and so on
 * here since it is char it is 2 bytes per cell
*/
#define PACK_CURSOR_LOCATION(row, col) \
        (((row * FB_WIDTH) + col))

/**
 * @name Frame buffer pack loaction
 *
 * @note row and col starts from 0
 * >0 means row zero, column zero;
 * >1 means row zero, column one;
 * >80 means row one, column zero and so on
 * here since it is char it is 2 bytes per cell
*/
#define PACK_FRAMEBUF_LOCATION(row, col) \
        (((row * FB_WIDTH) + col) * 2U)

 /**
  * @name fb_write_cell:
  *
 *  @brief Writes a character with the given foreground and background to position i and i +1
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

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
void fb_move_cursor(unsigned short pos);

/** outb:
 *  Sends the given data to the given I/O port. Defined in io.s
 *
 *  @param port The I/O port to send the data to
 *  @param data The data to send to the I/O port
 */
void outb(unsigned short port, unsigned char data);

#endif /* INCLUDE_IO_H */