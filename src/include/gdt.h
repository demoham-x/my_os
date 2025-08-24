/**
 * @file gdt.h
 *
 * @brief Headr file for Global descriptor Table (GDT)
 */
#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

/******************************************* Includes */

/******************************************* Typedefs/structures */
/* GDT Access Byte Flags */
/** @defgroup GDT_ACCESS GDT Access Byte Flags
 * @brief Flags for the access byte in GDT entries
 * @{
 */
#define GDT_ACCESS_PRESENT      0x80    /**< Segment is present in memory */
#define GDT_ACCESS_PRIVILEGE_0  0x00    /**< Ring 0 (kernel) privilege */
#define GDT_ACCESS_PRIVILEGE_3  0x60    /**< Ring 3 (user) privilege */
#define GDT_ACCESS_EXECUTABLE   0x08    /**< Segment contains executable code */
#define GDT_ACCESS_RW           0x02    /**< Segment is readable/writable */
#define GDT_ACCESS_DESCRIPTOR  0x10 
/** @} */

/* GDT Granularity Flags */
/** @defgroup GDT_FLAGS GDT Granularity and Size Flags
 * @brief Flags for the granularity byte in GDT entries
 * @{
 */
#define GDT_FLAG_GRANULARITY    0x80    /**< Limit is in 4KB blocks */
#define GDT_FLAG_32BIT          0x40    /**< 32-bit protected mode segment */
#define GDT_FLAG_16BIT          0x00    /**< 16-bit segment */
/** @} */

/* Standard GDT Access Patterns */
/** @defgroup GDT_SEGMENTS Standard GDT Segment Configurations
 * @brief Pre-configured access bytes for common segment types
 * @{
 */
#define GDT_KERNEL_CODE_ACCESS  (GDT_ACCESS_PRESENT | GDT_ACCESS_PRIVILEGE_0 | \
                                GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW | GDT_ACCESS_DESCRIPTOR)
#define GDT_KERNEL_DATA_ACCESS  (GDT_ACCESS_PRESENT | GDT_ACCESS_PRIVILEGE_0 | \
                                GDT_ACCESS_RW | GDT_ACCESS_DESCRIPTOR)
#define GDT_USER_CODE_ACCESS    (GDT_ACCESS_PRESENT | GDT_ACCESS_PRIVILEGE_3 | \
                                GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW | GDT_ACCESS_DESCRIPTOR)
#define GDT_USER_DATA_ACCESS    (GDT_ACCESS_PRESENT | GDT_ACCESS_PRIVILEGE_3 | \
                                GDT_ACCESS_RW | GDT_ACCESS_DESCRIPTOR)
/** @} */

/* Standard GDT Granularity Patterns */
/** @defgroup GDT_GRANULARITY Standard GDT Granularity Configurations
 * @brief Pre-configured granularity bytes for common segment types
 * @{
 */
#define GDT_STANDARD_GRANULARITY (GDT_FLAG_GRANULARITY | GDT_FLAG_32BIT)
/** @} */

/* DT Selector Offsets */
/** @defgroup GDT_SELECTORS GDT Selector Offsets
 * @brief Byte offsets for each GDT entry (used in assembly)
 * @{
 */
#define GDT_NULL_SELECTOR       0x00    /**< Null segment selector */
#define GDT_KERNEL_CODE_SELECTOR 0x08   /**< Kernel code segment selector */
#define GDT_KERNEL_DATA_SELECTOR 0x10   /**< Kernel data segment selector */
#define GDT_USER_CODE_SELECTOR   0x18   /**< User code segment selector */
#define GDT_USER_DATA_SELECTOR   0x20   /**< User data segment selector */
/** @} */

#define GDT_ENTRY_COUNT         5       /**< Total number of GDT entries */

/**
 * @struct gdt_entry
 * @brief Structure representing a single GDT entry (segment descriptor)
 *
 * This structure represents the x86 segment descriptor format. The fields
 * are arranged to match the hardware layout expected by the CPU.
 */
typedef struct _GDT_ENTRY
{
    unsigned short limit_low;     /**< Lower 16 bits of segment limit */
    unsigned short base_low;      /**< Lower 16 bits of base address */
    unsigned char  base_middle;   /**< Middle 8 bits of base address */
    unsigned char  access;        /**< Access byte (privilege, type, etc.) */
    unsigned char  granularity;   /**< Flags and upper 4 bits of limit */
    unsigned char  base_high;     /**< Upper 8 bits of base address */
} __attribute__((packed)) GDT_ENTRY;

/**
 * @struct gdt_ptr
 * @brief GDT pointer structure for the LGDT instruction
 *
 * This structure contains the size and address of the GDT table.
 * It's used by the LGDT instruction to load the GDT into the CPU.
 */
typedef struct _GDT
{
    unsigned short size;      /**< Size of GDT in bytes minus 1 - FIRST */
    unsigned int address;     /**< Linear address of the GDT - SECOND */
} __attribute__((packed)) GDT;



/******************************************* Defines */

/******************************************* Macros */

/******************************************* Protoytes */
/**
 * @name gdt_install
 *
 * @brief Sets up the GDT and loads it using the lgdt instruction
 */
void gdt_install(void);

/**
 * @name gdt_flush
 *
 * @brief Assembly function to load the new GDT and update segment registers
 *
 * @param gdt_ptr Pointer to the GDT point structure.
 */
void gdt_flush(GDT * gdt_ptr);
#endif /* INCLUDE_GDT_H */