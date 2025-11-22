default rel

section .data
    ; No constant here

section .text
    global compute_acc

compute_acc:
    push rbp
    mov rbp, rsp

    xor r9, r9

.loop:
    cmp r9, rdx
    jge .end

    lea r10, [r9 + r9*2]
    lea r10, [r10*8]

    movsd xmm0, [rcx + r10]
    movsd xmm1, [rcx + r10 + 8]
    movsd xmm2, [rcx + r10 + 16]

    subsd xmm1, xmm0

    ; Load 3.6 as double (alternative method)
    mov rax, 0x400ccccccccccccd
    movq xmm3, rax
    divsd xmm1, xmm3

    divsd xmm1, xmm2

    cvtsd2si eax, xmm1

    mov [r8 + r9*4], eax

    inc r9
    jmp .loop

.end:
    pop rbp
    ret