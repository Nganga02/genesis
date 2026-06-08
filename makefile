ASM = nasm
CC = gcc
BOOTSTRAP_FILE = asm/bootloader.asm 
INIT_KERNEL_FILES = asm/starter.asm
GDT_FILE = asm/gdt.asm
KERNEL_FILE = src/main.c 
KERNEL_FLAGS = -Wall -m32 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-pie
KERNEL_OBJECT = -o $(OBJ)kernel.elf
OBJ = obj/

build: $(BOOTSTRAP_FILE) $(KERNEL_FILES)
	@mkdir -p obj
	$(ASM) -f bin $(BOOTSTRAP_FILE) -o $(OBJ)bootstrap.o
	$(ASM) -I asm/ -f elf32 $(INIT_KERNEL_FILES) -o $(OBJ)starter.o
	$(ASM) -f elf32 $(GDT_FILE) -o $(OBJ)gdt.o
	$(CC) $(KERNEL_FLAGS) $(KERNEL_FILE) $(KERNEL_OBJECT)

	ld -melf_i386 -Tlinker.ld $(OBJ)starter.o $(OBJ)gdt.o $(OBJ)kernel.elf -o 539kernel.elf

	objcopy -O binary 539kernel.elf 539kernel.bin
	dd if=$(OBJ)/bootstrap.o of=kernel.img
	dd seek=1 conv=sync if=539kernel.bin of=kernel.img bs=512 count=5
	dd seek=6 conv=sync if=/dev/zero of=kernel.img bs=512 count=2046
	qemu-system-x86_64 -s kernel.img


clean:
	rm -f $(OBJ)*.o 