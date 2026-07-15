#include "./rangeset.h"

struct boot_info_t{
    Range *range_set;
    int count;
};


// get_memory_map:
//     push edx
//     xor ebx, ebx
//     mov eax, 0
//     mov ax, ds
//     mov es, ax
//     mov edx, SMAP
//     mov ax, 0xe820
//     mov ecx, 24

//     mov di, MEMORY_MAP_BUFFER

//     mov dword [range_count],0

//     int 15h
//     jc  .exit ; means an unsupported function
//     mov edx, SMAP ;some BIOSes clear the register


//     cmp eax, SMAP
//     jne .exit


//     jmp .save_address

//     .e820_loop:
//         mov ax, 0xe820
//         mov edx, SMAP
//         mov ecx, 24

//         int 15h
//         jc .exit

//         cmp eax, SMAP
//         jne .exit
//     .save_address:
//         cmp dword [es:di + 16], 1
//         jne .next

//         ;base low address considering endianness
//         mov eax, [es:di]

//         ;length low nibble
//         mov edx, [es:di + 8]

//         test edx, edx
//         je .next

//         ; length = base_address + length_of_usable_ram - 1 
//         add edx, eax
//         dec edx

//         mov esi, [range_count]

//         imul esi, 8

//         mov [RANGE_BUFFER + esi], eax
//         mov [RANGE_BUFFER + esi + 4], edx

//         inc dword [range_count]
        
//     .next:
//         test ebx, ebx
//         je .return
        
//         add di, 24
        
//         jmp .e820_loop



//     .return: 
//         pop edx
//         ret


//     .exit:
//         stc 
//         pop edx
//         ret