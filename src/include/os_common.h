/**
 * @file os_common.h
 *
 * @brief Common definitios for the OS
 */
#ifndef INCLUDE_OS_COMMON_H
#define INCLUDE_OS_COMMON_H
/******************************************* Includes */

/******************************************* Defines */

/******************************************* Macros */


/**
 * @name Macro to get byte from value
 */
#define GET_BYTE_FROM_VAL(val, byte_idx) \
        (((val) >> (8U * byte_idx)) & 0xFFU)

/******************************************* Protoytes */

#endif /* INCLUDE_OS_COMMON_H */