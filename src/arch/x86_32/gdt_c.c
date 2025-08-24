/**
 * @file gdt_c
 *
 * @brief C code for setting up the global descriptor table (GDT)
 */

/******************************************* Includes */
#include "gdt.h"
#include "serial_port.h"

/******************************************* Static global defines */
 /** @brief Array of GDT entries */
static GDT_ENTRY gdt_entries[GDT_ENTRY_COUNT];

/** @brief GDT pointer for LGDT instruction */
static GDT gdt_pointer;

/**
 * @brief Set up a single GDT entry
 * @param num Index of the GDT entry (0-4)
 * @param base Base address of the segment
 * @param limit Size limit of the segment
 * @param access Access byte (permissions, privilege level, etc.)
 * @param granularity Granularity and flags byte
 *
 * This helper function properly encodes a segment descriptor according to
 * the x86 GDT entry format, handling the split fields correctly.
 */
static void gdt_set_gate(unsigned int num, unsigned int base, unsigned int limit,
                        unsigned char access, unsigned char granularity)
{
    /* Encode base address (split across 3 fields) */
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    /* Encode limit (split across 2 fields) */
    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    /* Set granularity flags and access byte */
    gdt_entries[num].granularity |= granularity & 0xF0;
    gdt_entries[num].access       = access;
}

/**
 * @brief Initialize and install the Global Descriptor Table
 *
 * Creates a flat memory model GDT with 5 entries:
 * - Entry 0: Null segment (required by x86)
 * - Entry 1: Kernel code segment (0x00000000-0xFFFFFFFF, ring 0, executable)
 * - Entry 2: Kernel data segment (0x00000000-0xFFFFFFFF, ring 0, read/write)
 * - Entry 3: User code segment (0x00000000-0xFFFFFFFF, ring 3, executable)
 * - Entry 4: User data segment (0x00000000-0xFFFFFFFF, ring 3, read/write)
 *
 * All segments use 4GB limits with 4KB granularity for a flat memory model.
 */
void gdt_install(void)
{
    /* Set up the GDT pointer */
    gdt_pointer.size = (sizeof(GDT_ENTRY) * GDT_ENTRY_COUNT) - 1;
    gdt_pointer.address = (unsigned int)&gdt_entries;

    /* Entry 0: Null segment (required) */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* Entry 1: Kernel code segment*/
    /* Base: 0x00000000, Limit: 0xFFFFF (4GB with 4KB granularity)*/
    /* Access: Present, Ring 0, Executable, Readable*/
    gdt_set_gate(1, 0x00000000, 0xFFFFF,
                GDT_KERNEL_CODE_ACCESS, GDT_STANDARD_GRANULARITY);
    /* Entry 2: Kernel data segment */
    /* Base: 0x00000000, Limit: 0xFFFFF (4GB with 4KB granularity) */
    /* Access: Present, Ring 0, Writable */
    gdt_set_gate(2, 0x00000000, 0xFFFFF,
                GDT_KERNEL_DATA_ACCESS, GDT_STANDARD_GRANULARITY);

    /* Entry 3: User code segment */
    /* Base: 0x00000000, Limit: 0xFFFFF (4GB with 4KB granularity) */
    /* Access: Present, Ring 3, Executable, Readable */
    gdt_set_gate(3, 0x00000000, 0xFFFFF,
                GDT_USER_CODE_ACCESS, GDT_STANDARD_GRANULARITY);

    /* Entry 4: User data segment */
    /* Base: 0x00000000, Limit: 0xFFFFF (4GB with 4KB granularity) */
    /* Access: Present, Ring 3, Writable   */
    gdt_set_gate(4, 0x00000000, 0xFFFFF,
                GDT_USER_DATA_ACCESS, GDT_STANDARD_GRANULARITY);

    /* Load the new GDT and flush segment registers */
    gdt_flush(&gdt_pointer);
}