; Simple kernel entry point
[bits 32]
[extern main]

call    main
jmp     $
