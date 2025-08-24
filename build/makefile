
# Architecture
ARCH = x86_32
# Compiler
CC = gcc
# Assembler
AS = nasm
# LINKER
LD = ld
# Emulator (bochs or qemu-system-i386)
EMU = bochs
#object extension
obj = o

# Root directory of the project
ROOT = ..
# Source Dir
SRC = $(ROOT)/src
# Architecture main dir
ARCH_MAIN_DIR = $(SRC)/arch
# Directories
ARCH_DIR    = $(ARCH_MAIN_DIR)/$(ARCH)
DRIVER_DIR  = $(SRC)/drivers
KERNEL_DIR  = $(SRC)/kernel
INCLUDE_DIR = $(SRC)/include
# Kernel out Dir
KERNEL_OUT_DIR = $(ROOT)/iso/boot
# Kernal file
KERNEL = kernel.elf

# Compiler flags
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
# Linker flags
# -T specifies the linker script, -melf_i386 specifies the output format
# -melf_i386 is used for 32-bit x86 architecture
LDFLAGS = -T $(ARCH_DIR)/link.ld -melf_i386
# Assembler flags
# -f elf specifies the output format for the assembler
ASFLAGS = -f elf

# ISO file
ISO=os.iso

# C objects
C_OBJS = \
	kmain.$(obj) \
	io_ops.$(obj) \
	fb.$(obj) \
	serial_port.$(obj) \
	gdt_c.$(obj)

# Assembly objects
S_OBJS = \
	loader.$(obj) \
	io.$(obj) \
	gdt.$(obj)

# All objects
OBJECTS = $(C_OBJS) $(S_OBJS)

# Src file paths
VPATH = \
	$(ROOT)/src \
	$(ARCH_DIR) \
	$(DRIVER_DIR) \
	$(KERNEL_DIR) \

# Include directories
INCLUDES_DIRS = \
	-I$(INCLUDE_DIR) \
	-I$(DRIVER_DIR)

# All build
all: $(ISO)

# Kernel build
$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(KERNEL_OUT_DIR)/$(KERNEL)

# Generate ISO
$(ISO): $(KERNEL)
	genisoimage -R \
	            -b boot/grub/stage2_eltorito \
	            -no-emul-boot \
	            -boot-load-size 4 \
	            -A os \
	            -input-charset utf8 \
	            -quiet \
	            -boot-info-table \
	            -o $(ISO) \
	            $(ROOT)/iso

# Run ISO
run: $(ISO)
ifeq ($(EMU), bochs)
	$(EMU) -f $(ROOT)/bochs/bochsrc.txt -q
else ifeq ($(EMU), qemu-system-i386)
#		$(EMU) -cdrom os.iso -monitor stdio -d cpu,in_asm,int -D detailed.log
	$(EMU) -cdrom $(ISO) -monitor stdio
else
	$(error Unsupported emulator: $(EMU))
endif


# C object compilation
# %.c files are compiled to %.o files
$(C_OBJS): %.$(obj): %.c
	$(CC) $(CFLAGS) $(INCLUDES_DIRS) $< -o $@

# Assembly object compilation
# %.s files are assembled to %.o files
$(S_OBJS): %.$(obj): %.s
	$(AS) $(ASFLAGS) $(INCLUDES_DIRS) $< -o $@

# Clean up build artifacts
# Removes all object files and the kernel binary
clean:
	rm -rf *.$(obj) $(KERNEL_OUT_DIR)/$(KERNEL) $(ISO)

.PHONY: all clean run
