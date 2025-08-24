/**
 * @file serial_port.c
 *
 * @brief Implementation of serial port I/O functions
 */

/******************************************* Includes */
#include "io.h"
#include "os_common.h"
#include "serial_port.h"

/******************************************* Defines */

/******************************************* Macros */

/******************************************* Functions */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    /* Enable DLAB */
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    /* Send higher byte */
    outb(SERIAL_DATA_PORT(com), GET_BYTE_FROM_VAL(divisor, 1));
    /* Send lower byte */
    outb(SERIAL_DATA_PORT(com), GET_BYTE_FROM_VAL(divisor, 0));
}

void serial_configure_line(unsigned short com, unsigned char config)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), config);
}

void serial_configure_buffer(unsigned short com, unsigned char config)
{
    outb(SERIAL_FIFO_COMMAND_PORT(com), config);
}

void serial_configure_modem(unsigned short com, unsigned char config)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), config);
}

int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* checking 5th bit  of line status port */
    return (inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20U);
}

void serial_write(unsigned short com, char * buf, unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++)
    {
        /* wait until FIFO queu is empty */
        while (!serial_is_transmit_fifo_empty(com)) {};

        /* send data */
        outb(SERIAL_DATA_PORT(com), buf[i]);
    }
}