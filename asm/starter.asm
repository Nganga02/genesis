bits 16
extern kernel_main
extern interrupt_handler
extern scheduler
extern run_next_process
extern next_process
extern screen_init
extern printi
extern println

extern page_directory

global load_page_directory
global enable_paging
global load_context
global jump_next_process

global dev_write_byte
global dev_write_word
global dev_read

BOOT_INFO equ 0x8000

start:
    mov ax, cs
    mov ds, ax 
    
    call load_gdt
    call init_video_mode
    call get_memory_map
    call enter_protected_mode
    call setup_interrupts
    call load_task_register

    call 08h:start_kernel

load_gdt:
    cli

    lgdt [gdtr-start]

    ret


init_video_mode:
    mov ah, 0h 
    mov al, 03h
    int 10h


    mov ah, 01h
    mov cx, 2000h
    int 10h

    ret

enter_protected_mode:
    mov eax, cr0

    or eax, 1
    mov cr0, eax

    ret



setup_interrupts:
    call remap_pic
    call load_idt
    ret

remap_pic:
    mov al, 11h

    send_init_cmd_to_pic_master:
        out 0x20, al 

    send_init_cmd_to_pic_slave:
        out 0xa0, al 

        ;--------------;

    make_irq_start_from_intr_32_in_pic_master:
        mov al, 0x20
        out 0x21, al


    make_irq_start_from_intr_40_in_pic_slave:
        mov al, 0x28
        out 0xa1, al

        ;---------------;

    tell_pic_master_where_pic_slave_is_connected:
        mov al, 0x04
        out 0x21, al
    

    tell_pic_slave_where_pic_master_is_connected:
        mov al, 0x02
        out 0xa1, al

        ;---------------:

    mov al, 01h

    tell_pic_master_the_arch_is_x86:
        out 0x21, al

    tell_pic_slave_the_arch_is_x86:
        out 0xa1, al

        ;---------------;

        mov al, 0h
    make_pic_master_enables_all_irqs:
        out 0x21, al

    make_pic_slave_enables_all_irqs:
        out 0xa1, al
    
    ret

load_idt:
    lidt[idtr - start]
    ret

load_task_register:
    mov ax, 40d
    ltr ax

    ret


MEMORY_MAP_BUFFER equ 0x5000

RANGE_BUFFER      equ 0x7000



SMAP equ 0x534d4150

range_count dd 0

get_memory_map:
    xor ebx,ebx

    mov ax,0
    mov es,ax

    mov di,MEMORY_MAP_BUFFER

    mov dword [range_count],0

.e820_loop:
    mov eax,0xE820
    mov edx,0x534D4150
    mov ecx,24

    int 0x15
    jc .done

    cmp eax,0x534D4150
    jne .done

    ; usable memory only
    cmp dword [es:di + 16],1
    jne .next

    ; base low
    mov eax,[es:di]

    ; length low
    mov edx,[es:di + 8]

    test edx,edx
    jz .next

    ; end = base + length - 1
    add edx,eax
    dec edx

    mov esi,[range_count]
    imul esi,8

    mov [RANGE_BUFFER + esi],eax
    mov [RANGE_BUFFER + esi + 4],edx

    inc dword [range_count]

.next:
    add di,24

    test ebx,ebx
    jne .e820_loop

.done:
    ret

    
bits 32
load_page_directory:
    mov eax, [page_directory]
    mov cr3, eax

    mov word [0xb8000], 0x0f41 ; A

    ret

p_eax equ 0
p_ebx equ 4
p_ecx equ 8
p_edx equ 12 
p_esi equ 24
p_edi equ 28

load_context:
    push ebp
    mov ebp, esp
    mov ebx, [next_process]
    mov eax, [ebx]
    mov ecx, [ebx + p_ecx]
    mov edx, [ebx + p_edx]
    mov esi, [ebx + p_esi]
    mov edi, [ebx + p_edi]
    mov ebx, [ebx + p_ebx]
    pop ebp
    ret

jump_next_process:
    sti
    jmp [next_process + 36]


enable_paging:
    mov eax, cr0
    or eax, 0x80000000 ;enabling bit 31 to enable paging
    mov cr0, eax

    mov word [0xb8002], 0x0f42 ; B
    ret

dev_write_byte:
    push edx
    push eax

    xor edx, edx
    xor eax, eax

    mov dx, [esp + 12]
    mov al, [esp + 16]

    out dx, al

    pop eax
    pop edx

    ret

dev_write_word:
    push edx
    push eax

    xor edx, edx
    xor eax, eax

    mov dx, [esp + 12]
    mov ax, [esp + 16]

    out dx, ax

    pop eax
    pop edx

    ret

dev_read:
    push edx

    xor eax, eax
    xor edx, edx

    mov dx, [esp + 8]

    in ax, dx

    pop edx

    ret


start_kernel:
    mov esp, 0x90000
    and esp, 0xfffffff0
    mov ebp, esp
    mov eax, 10h
    mov ds, eax
    mov ss, eax
    mov es, eax

    xor eax, eax
    mov fs, eax
    mov gs, eax


    mov eax, MEMORY_MAP_BUFFER
    mov DWORD[BOOT_INFO], eax
    xor eax, eax
    mov eax, [range_count]
    mov DWORD[BOOT_INFO + 4], eax

    sti 
    call screen_init
    mov eax, DWORD [MEMORY_MAP_BUFFER]
    call println
    push eax

    call printi
    call println

    mov eax, DWORD [range_count]
    call println
    push eax
    call printi
    call println
    pop eax
    push BOOT_INFO


    call kernel_main


gdt:
    null_descriptor                 : dw 0,0,0,0
    kernel_code_descriptor          : dw 0xffff, 0x0000, 0x9a00, 0x00cf
    kernel_data_descriptor          : dw 0xffff, 0x0000, 0x9200, 0x00cf
    user_space_code_descriptor      : dw 0xffff, 0x0000, 0xfa00, 0x00cf
    user_space_data_descriptor      : dw 0xffff, 0x0000, 0xf200, 0x00cf
    tss_descriptor                  : dw tss + 3, tss, 0x8900, 0x0000

gdtr:
    gdt_size_in_bytes               : dw gdtr-gdt
    gdt_base_address                : dd gdt




isr_0:
    cli
    push 0
    jmp isr_basic

isr_1:
    cli
    push 1
    jmp isr_basic

isr_2:
    cli
    push 2
    jmp isr_basic

isr_3:
    cli
    push 3
    jmp isr_basic

isr_4:
    cli
    push 4
    jmp isr_basic

isr_5:
    cli
    push 5
    jmp isr_basic

isr_6:
    cli
    push 6
    jmp isr_basic

isr_7:
    cli
    push 7
    jmp isr_basic

isr_8:
    cli
    push 8
    jmp isr_basic

isr_9:
    cli
    push 9
    jmp isr_basic

isr_10:
    cli
    push 10
    jmp isr_basic

isr_11:
    cli
    push 11
    jmp isr_basic

isr_12:
    cli
    push 12
    jmp isr_basic

isr_13:
    cli
    ;push dword [esp]
    push 13
    jmp isr_basic

isr_14:
    cli
    push 14
    jmp isr_basic

isr_15:
    cli
    push 15
    jmp isr_basic

isr_16:
    cli
    push 16
    jmp isr_basic

isr_17:
    cli
    push 17
    jmp isr_basic

isr_18:
    cli
    push 18
    jmp isr_basic

isr_19:
    cli
    push 19
    jmp isr_basic

isr_20:
    cli
    push 20
    jmp isr_basic

isr_21:
    cli
    push 21
    jmp isr_basic

isr_22:
    cli
    push 22
    jmp isr_basic

isr_23:
    cli
    push 23
    jmp isr_basic

isr_24:
    cli
    push 24
    jmp isr_basic

isr_25:
    cli
    push 25
    jmp isr_basic

isr_26:
    cli
    push 26
    jmp isr_basic

isr_27:
    cli
    push 27
    jmp isr_basic

isr_28:
    cli
    push 28
    jmp isr_basic

isr_29:
    cli
    push 29
    jmp isr_basic

isr_30:
    cli
    push 30
    jmp isr_basic

isr_31:
    cli
    push 31
    jmp isr_basic

;----------------------------;
new_line dd 402

isr_32:;handles the timer interrupt;
    cli
    pushad

    mov eax, [esp + 32]
    push eax
    
    call scheduler

   
    mov al, 0x20
    out 0x20, al


    add esp, 0x28
    push run_next_process

    iret

isr_33:
    cli
    push 33
    jmp irq_basic

isr_34:
    cli
    push 34
    jmp irq_basic

isr_35:
    cli
    push 35
    jmp irq_basic

isr_36:
    cli
    push 36
    jmp irq_basic

isr_37:
    cli
    push 37
    jmp irq_basic

isr_38:
    cli
    push 38
    jmp irq_basic

isr_39:
    cli
    push 39
    jmp irq_basic

isr_40:
    cli
    push 40
    jmp irq_basic

isr_41:
    cli
    push 41
    jmp irq_basic

isr_42:
    cli
    push 42
    jmp irq_basic

isr_43:
    cli
    push 43
    jmp irq_basic

isr_44:
    cli
    push 44
    jmp irq_basic

isr_45:
    cli
    push 45
    jmp irq_basic

isr_46:
    cli
    push 46
    jmp irq_basic

isr_47:
    cli
    push 47
    jmp irq_basic

isr_48:
    cli
    push 48
    jmp irq_basic

isr_basic:
    call interrupt_handler

    add esp, 4
    sti
    iret
    
irq_basic:
    call interrupt_handler

    ;-giving the pic commands-;

    mov al, 0x20
    out 0x20, al

    cmp byte [esp], 0x28
    jnge irq_basic_end

    out 0xa0, al

    irq_basic_end:
        add esp, 4

        sti
        iret



idt:
    dw isr_0, 8, 0x8e00, 0x0000
    dw isr_1, 8, 0x8e00, 0x0000
    dw isr_2, 8, 0x8e00, 0x0000
    dw isr_3, 8, 0x8e00, 0x0000
    dw isr_4, 8, 0x8e00, 0x0000
    dw isr_5, 8, 0x8e00, 0x0000
    dw isr_6, 8, 0x8e00, 0x0000
    dw isr_7, 8, 0x8e00, 0x0000
    dw isr_8, 8, 0x8e00, 0x0000
    dw isr_9, 8, 0x8e00, 0x0000
    dw isr_10, 8, 0x8e00, 0x0000
    dw isr_11, 8, 0x8e00, 0x0000
    dw isr_12, 8, 0x8e00, 0x0000
    dw isr_13, 8, 0x8e00, 0x0000
    dw isr_14, 8, 0x8e00, 0x0000
    dw isr_15, 8, 0x8e00, 0x0000
    dw isr_16, 8, 0x8e00, 0x0000
    dw isr_17, 8, 0x8e00, 0x0000
    dw isr_18, 8, 0x8e00, 0x0000
    dw isr_19, 8, 0x8e00, 0x0000
    dw isr_20, 8, 0x8e00, 0x0000
    dw isr_21, 8, 0x8e00, 0x0000
    dw isr_22, 8, 0x8e00, 0x0000
    dw isr_23, 8, 0x8e00, 0x0000
    dw isr_24, 8, 0x8e00, 0x0000
    dw isr_25, 8, 0x8e00, 0x0000
    dw isr_26, 8, 0x8e00, 0x0000
    dw isr_27, 8, 0x8e00, 0x0000
    dw isr_28, 8, 0x8e00, 0x0000
    dw isr_29, 8, 0x8e00, 0x0000
    dw isr_30, 8, 0x8e00, 0x0000
    dw isr_31, 8, 0x8e00, 0x0000
    dw isr_32, 8, 0x8e00, 0x0000
    dw isr_33, 8, 0x8e00, 0x0000
    dw isr_34, 8, 0x8e00, 0x0000
    dw isr_35, 8, 0x8e00, 0x0000
    dw isr_36, 8, 0x8e00, 0x0000
    dw isr_37, 8, 0x8e00, 0x0000
    dw isr_38, 8, 0x8e00, 0x0000
    dw isr_39, 8, 0x8e00, 0x0000
    dw isr_40, 8, 0x8e00, 0x0000
    dw isr_41, 8, 0x8e00, 0x0000
    dw isr_42, 8, 0x8e00, 0x0000
    dw isr_43, 8, 0x8e00, 0x0000
    dw isr_44, 8, 0x8e00, 0x0000
    dw isr_45, 8, 0x8e00, 0x0000
    dw isr_46, 8, 0x8e00, 0x0000
    dw isr_47, 8, 0x8e00, 0x0000
    dw isr_48, 8, 0x8e00, 0x0000
    
idtr:
    idt_size_in_bytes   : dw idtr-idt
    idt_base_address    : dd idt



tss:
    dd 0
