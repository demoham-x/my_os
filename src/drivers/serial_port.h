/**
 * @file serial_port.h
 *
 * @brief Header file for serial port I/O functions
 */
#ifndef INCLUDE_SERIAL_PORT_H
#define INCLUDE_SERIAL_PORT_H
/******************************************* Includes */
#include "io.h"

/******************************************* Defines */
/* All the I/O ports are calculated relative to the data port. This is because
* all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
* order, but they start at different values.
*/

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
* Tells the serial port to expect first the highest 8 bits on the data port,
* then the lowest 8 bits will follow
*/
#define SERIAL_LINE_ENABLE_DLAB         0x80

/******************************************* Macros */

/**
 * @name Pack serial port Configuration val
 *
 * @brief Configuration byte for the serial port
 *
 * @par
 * Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
 * Content: | d | b | prty  | s | dl  |
 *
 * d    -  Enables (d = 1) or disables (d = 0) DLAB
 * b    - If break control is enabled (b = 1) or disabled (b = 0)
 * prty	- The number of parity bits to use
 * s    - The number of stop bits to use (s = 0 equals 1, s = 1 equals 1.5 or 2)
 * dl   - Describes the length of the data bits
 *      (dl = 0 equals 5, dl = 1 equals 6, dl = 2 equals 7, dl = 3 equals 8)
 */
#define PACK_SERIAL_PORT_CONFIG(d, b, prty, s, dl)  \
        (((d & 0x01U) << 7U) |                      \
         ((b & 0x01U) << 6U) |                      \
         ((prty & 0x07U) << 3U) |                   \
         ((s & 0x01U) << 2U) |                      \
         (dl & 0x03U))

/**
 * @name Pack buffer configuration val
 *
 * @brief Configration byte for the FIFO buffer
 *
 * @par
 * Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
 * Content: | lvl | bs | r | dma | clt | clr | e |
 *
 * lvl - How many bytes should be stored in the FIFO buffers
 * bs  - If the buffers should be 16 or 64 bytes large
 * r   - Reserved for future use
 * dma - How the serial port data should be accessed
 * clt - Clear the transmission FIFO buffer
 * clr - Clear the receiver FIFO buffer
 * e   - If the FIFO buffer should be enabled or not
 */
#define PACK_SERIAL_BUFFER_CONFIG(lvl, bs, dma, clt, clr, e) \
        ((((lvl) & 0x03U) << 6U) |                           \
         (((bs) & 0x01U) << 5U)  |                           \
         (((dma) & 0x01U) << 3U) |                           \
         (((clt) & 0x01U) << 2U) |                           \
         (((clr) & 0x01U) << 1U) |                           \
         ((e) & 0x01U))

/**
 * @name Pack modem control configuration val
 *
 * @brief Configuration byte for the Modem Control Register
 *
 * @par
 * Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
 * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
 *
 * r   - Reserved
 * af  - Autoflow control enabled
 * lb  - Loopback mode (used for debugging serial ports)
 * ao2 - Auxiliary output 2, used for receiving interrupts
 * ao1 - Auxiliary output 1
 * rts - Ready To Transmit
 * dtr - Data Terminal Ready
 */
#define PACK_SERIAL_MODEM_CONFIG(af, lb, ao2, ao1, rts, dtr) \
        ((((af) & 0x01U) << 5U)  |                           \
         (((lb) & 0x01U) << 4U)  |                           \
         (((ao2) & 0x01U) << 3U) |                           \
         (((ao1) & 0x01U) << 2U) |                           \
         (((rts) & 0x01U) << 1U) |                           \
         ((dtr) & 0x01U))


/******************************************* Protoytes */
/**
 * @name serial_configure_baud_rate:
 *
 *  @brief Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor);

/**
 * @name serial_configure_line:
 *
 *  @brief Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 *  @param config the configuration byte(@ref PACK_SERIAL_PORT_CONFIG)
 *  default config:PACK_SERIAL_PORT_CONFIG(0, 0, 0, 0, 8U) : 0x03U
 */
void serial_configure_line(unsigned short com, unsigned char config);

/**
 * @name serial_configure_buffer:
 *
 *  @brief Configures the buffer of the given serial port.
 *
 * @param com  The serial port to configure
 * @param config the configuration byte (@ref PACK_SERIAL_BUFFER_CONFIG)
 * default config:PACK_SERIAL_BUFFER_CONFIG(0x03U, 0, 0, 1U, 1U, 1U): 0xC0U
 * Enables FIFO, Clear both receiver and transmission FIFO queues,
 * Use 14 bytes as size of queue.
 */
void serial_configure_buffer(unsigned short com, unsigned char config);

/**
 * @name serial_configure_modem:
 *
 * @brief Configures the modem control register of the given serial port.
 *
 * @param com    The serial port to configure
 * @param config the configuration byte (@ref PACK_SERIAL_MODEM_CONFIG)
 * default config: PACK_SERIAL_MODEM_CONFIG(0, 0, 0, 0, 1U, 1U): 0x03
 * Sets DTR=1, RTS=1, AO2=0 (for interrupts), other bits cleared.
 * Basic handshaking enabled with interrupt support.
 */
void serial_configure_modem(unsigned short com, unsigned char config);

/**
 * @name serial_is_transmit_fifo_empty:
 * @brief Checks whether the transmit FIFO queue is empty or not for the given COM
 * port.
 *
 * @param  com The COM port
 * @return 0 if the transmit FIFO queue is not empty
 *         1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com);

/**
 * @name Serial_write
 *
 * @brief writes a string to the given seral port
 *
 * @param com the com port to write
 * @param buf the string to write
 * @param len the length of the string
 */
void serial_write(unsigned short com, char * buf, unsigned int len);
#endif /* INCLUDE_SERIAL_PORT_H */