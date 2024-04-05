[bits 32]

; Will fallthru and enable if A20 is not enabled
check_A20:
    pushad
    mov     edi, 0x112345   ; odd address
    mov     esi, 0x012345   ; even address
    mov     [esi], esi
    mov     [edi], edi
    cmpsd 
    popad
    jne     .done

    .enable_A20:
    in      al, 0x92    ; Read Fast A20 gate
    test    al, 2       ; See if it is already set
    jnz     .done       ; yes, we're done
    or      al, 2       ; no, set it
    out     0x92, al

    .done:
    ret


