[org 0x7c00]

SECTION .data
    gdt_start:

    ; GDT null descriptor 
    ; 8 bytes of 0
    gdt_null:
        dq  0x0

    gdt_code:
        ; Base: 0x0,    Limit: 0xffff
        ; Flags 1:      (present)1 (privilege)00 (type)1                
        ; -> 0b1001
        ; Type flags:   (code)1 (conforming)0 (readable)1 (accessed) 0  
        ; -> 0b1010
        ; Flags 2:      (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL) 0 
        ; -> 0b1100
        dw  0xffff      ; limit (0-15)
        dw  0x0000      ; base  (0-15)
        dw  0x0000      ; base  (16-23)
        db  0b10011010  ; 1st flags, type flags
        db  0b11001111  ; 2nd flags, limit (16-19)
        dw  0x0000      ; base  (24-31)

    gdt_data:
        ; See above for base, limit, flags 1&2
        ; Type flags:   (code)0 (expand down)0 (writable)1 (accessed)0 
        ; -> 0b0010
        dw  0xffff      ; limit (0-15)
        dw  0x0000      ; base  (0-15)
        dw  0x0000      ; base  (16-23)
        db  0b10010010  ; 1st flags, type flags
        db  0b11001111  ; 2nd flags, limit (16-19)
        dw  0x0000      ; base  (24-31)
    gdt_end:


    gdt_descriptor:
        dw  gdt_end - gdt_start - 1 ; size of GDT
        dd  gdt_start               ; start of GDT


SECTION .text

[bits 16]
KERNEL_OFFSET equ 0x1000

_start:
    mov     [BOOT_DRIVE], dl    ; store boot drive

    ; set up temporary stack
    mov     bp, 0x9000
    mov     sp, bp

    ; zero out segment registers just in case
    xor     ax, ax
    mov     ds, ax
    mov     es, ax

    ; welcome message
    mov     bx, STR_WELCOME
    call    print_str

    ; start reading disk
    mov     bx, STR_READDISK
    call    print_str

    mov     bx, KERNEL_OFFSET   ; load read data to KERNEL_OFFSET
    mov     dh, 15              ; read 15 sectors
    mov     dl, [BOOT_DRIVE]    ; from the drive we booted
    call    read_disk




read_disk:
    push    dx
    mov     ah, 0x2     ; read disk call
    mov     al, dh      ; # of sectors
    mov     ch, 0x0     ; Cylinder 0
    mov     dh, 0x0     ; Head 0 
    mov     cl, 0x2     ; Sector 2

    int     0x13
    jc      .error

    ; See if read sectors equals requested
    pop     dx
    cmp     al, dh
    jne     .error

    ret

    .error:
    mov     bx, STR_DISKERROR
    call    print_str
    hlt


; Simple print routine for real mode
; Prints a NUL terminated string pointed to by bx
print_str:
    push    ax
    mov     ah, 0x0e    ; print character call

    .loop:
    mov     al, [bx]
    test    al, al
    jz      .end

    int     0x10

    add     bx, 1
    jmp     .loop
    
    .end:
    pop     ax
    ret


; PROTECTED MODE BEGINS HERE
[bits 32]
BEGIN_PM:







; Variables
BOOT_DRIVE: db 0
    
STR_WELCOME: db "Hello! Booted up in real mode", 0xa, 0xd, 0x0
STR_READDISK: db "Reading disk...", 0xa, 0xd, 0x0
STR_DISKERROR: db "ERROR READING DISK", 0xa, 0xd, 0x0

; Padding and boot sector magic
times 510-($-$$) db 0
dw 0xaa55
