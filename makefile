ASM = nasm
CC = gcc
BOOTSTRAP_FILE = asm/bootloader.asm 
INIT_KERNEL_FILES = asm/starter.asm
GDT_FILE = asm/gdt.asm
PAGING_FILE = asm/paging.asm
KERNEL_FILE = src/main.c 
KERNEL_FLAGS = -Wall -m32 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-pie
KERNEL_OBJECT = -o $(OBJ)kernel.elf
OBJ = obj/


OBJECT_FILES = print/print.elf

build: $(BOOTSTRAP_FILE) $(KERNEL_FILES)
	@mkdir -p obj
	$(ASM) -I asm/ -f bin $(BOOTSTRAP_FILE) -o $(OBJ)bootstrap.o
	$(ASM) -f elf32 $(INIT_KERNEL_FILES) -o $(OBJ)starter.o
	$(CC) $(KERNEL_FLAGS) $(KERNEL_FILE) $(KERNEL_OBJECT)
	$(CC) $(KERNEL_FLAGS) src/print/print.c -o obj/print.elf
	$(CC) $(KERNEL_FLAGS) src/process/process.c -o obj/process.elf
	$(CC) $(KERNEL_FLAGS) src/scheduler/scheduler.c -o obj/scheduler.elf
	$(CC) $(KERNEL_FLAGS) src/heap/heap.c -o obj/heap.elf
	$(CC) $(KERNEL_FLAGS) src/paging/paging.c -o obj/paging.elf
	$(CC) $(KERNEL_FLAGS) src/ata/ata.c -o obj/ata.elf
	$(CC) $(KERNEL_FLAGS) src/string/string.c -o obj/string.elf
	$(CC) $(KERNEL_FLAGS) src/filesystem/filesystem.c -o obj/filesystem.elf
	

	ld -melf_i386 -Tlinker.ld $(OBJ)starter.o $(OBJ)kernel.elf \
		$(OBJ)print.elf $(OBJ)process.elf $(OBJ)scheduler.elf \
		$(OBJ)ata.elf $(OBJ)string.elf $(OBJ)filesystem.elf \
		$(OBJ)heap.elf $(OBJ)paging.elf -o 539kernel.elf

	objcopy -O binary 539kernel.elf 539kernel.bin

	dd if=$(OBJ)/bootstrap.o of=kernel.img
	dd seek=1 conv=sync if=539kernel.bin of=kernel.img bs=512 count=20
	dd seek=21 conv=sync if=/dev/zero of=kernel.img bs=512 count=2056
	qemu-system-i386 -drive format=raw,file=kernel.img -s



clean:
	rm -f $(OBJ)*.o 
	rm -f $(OBJ)*.elf
	rm -f kernel.img
	rm -f 539*