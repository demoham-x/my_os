
#include "io.h"
#include "fb.h"
#include "serial_port.h"
#include "gdt.h"

/* Frame buffer write test */
/*#define TEST_2 */
/* Serial buffer write test */
#define TEST_3
/* GDT test */
#define TEST_4

/* The C function */
int sum_of_three(int arg1, int arg2, int arg3)
{
    return arg1 + arg2 + arg3;
}

void init_serial_com1(void)
{
    unsigned char line_config;
    unsigned char buffer_config;
    unsigned char modem_config;

    /* Configure baud rate to 57600 (115200 / 2) */
    serial_configure_baud_rate(SERIAL_COM1_BASE, 2);

    /* Configure line: 8 data bits, no parity, 1 stop bit, DLAB disabled */
    line_config = PACK_SERIAL_PORT_CONFIG(0, 0, 0, 0, 3);  /* 0x03 */
    serial_configure_line(SERIAL_COM1_BASE, line_config);

    /* Configure FIFO: Enable, clear both buffers, 14-byte trigger, 16-byte buffer */
    buffer_config = PACK_SERIAL_BUFFER_CONFIG(3, 0, 0, 1, 1, 1);  /* 0xC7 */
    serial_configure_buffer(SERIAL_COM1_BASE, buffer_config);

    /* Configure modem: DTR + RTS enabled, AO2 for interrupts */
    modem_config = PACK_SERIAL_MODEM_CONFIG(0, 0, 1, 0, 1, 1);  /* 0x0B */
    serial_configure_modem(SERIAL_COM1_BASE, modem_config);
}

int main()
{
#ifdef TEST_1
    /* Print 'H' at top left (row 0, col 0), white on black */
    fb_write_cell(PACK_FRAMEBUF_LOCATION(0, 0), 'H', FB_WHITE, FB_BLACK);
    fb_move_cursor(PACK_CURSOR_LOCATION(0, 0));

    /* Print 'i' at row 0, col 1, green on black */
    fb_write_cell(PACK_FRAMEBUF_LOCATION(0, 1), 'i', FB_GREEN, FB_BLACK);
    fb_move_cursor(PACK_CURSOR_LOCATION(0, 1));

    /* Print '!' at row 4, col 10, light red on blue */
    fb_write_cell(PACK_FRAMEBUF_LOCATION(4, 10), '!', FB_LIGHT_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(4, 10));
    /* printing "MY OS" */
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 2), 'M', FB_LIGHT_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 2));
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 3), 'Y', FB_LIGHT_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 3));
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 4), ' ', FB_LIGHT_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 4));
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 5), 'O', FB_LIGHT_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 5));
    fb_write_cell(PACK_FRAMEBUF_LOCATION(5, 6), 'S', FB_LIGHT_RED, FB_BLUE);
    fb_move_cursor(PACK_CURSOR_LOCATION(5, 6));
#endif /* TEST_1 */

#ifdef TEST_2
    char message[] = "Hello world!";
    fb_write(message, sizeof(message), DEFAULT_FG_COLOR, DEFAULT_BG_COLOUR);

    fb_write("\r\n", 2, DEFAULT_FG_COLOR, FB_BLUE);

    char os_message[] = "my os!";
    fb_write(os_message, sizeof(os_message), FB_CYAN, FB_LIGHT_RED);
#endif /* TEST_2 */

#ifdef TEST_3
    init_serial_com1();
    char serial_message[] = "Hello serial port!\n";
    serial_write(SERIAL_COM1_BASE, serial_message, sizeof(serial_message) - 1);
    serial_write(SERIAL_COM1_BASE, "\r\n", 2);

    char os_serial_message[7] = "my os!\n";
    serial_write(SERIAL_COM1_BASE, os_serial_message, sizeof(os_serial_message) - 1);
#endif /* TEST 3 */

#ifdef TEST_4
    fb_write("Before GDT install\n", 19, DEFAULT_FG_COLOR, DEFAULT_BG_COLOUR);
    gdt_install();
    fb_write("After GDT install\n", 18, DEFAULT_FG_COLOR, DEFAULT_BG_COLOUR);
#endif /* TEST_4 */
    while (1) { asm volatile ("hlt");}

    return 0;
}