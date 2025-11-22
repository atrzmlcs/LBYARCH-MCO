default rel

section .data
    CONVERSION_VAL dq 3.6

section .text
bits 64
default rel
global compute_acc

; RCX - Size of Y
; RDX - Address of 2D Array matrix
; R8  - Address of 1D Array Result
; Note: Do not use XMM0 - XMM3
; Formula: A = (Vf - Vi) / T
; In matrix: Vi = [0] Vf = [1] T = [2]

compute_acc:
    MOV R9, RCX
    MOV RCX, 0
    MOVSD XMM7, [CONVERSION_VAL]

; Loop from last to first
LOOP_MATRIX:
    ;MOV R9, [RCX * ROW_SIZE] ;Current row offset
    CMP RCX, R9 ; i < Y
    JZ END_LOOP
    
    ; 24 - Size per row
    ; 8  - Size per element
    MOV R10, RCX
    IMUL R10, 24
    LEA R11, [RDX + R10]
    
    MOVSD XMM4, [R11 + 0] ; Vi
    MOVSD XMM5, [R11 + 8] ; Vf
    MOVSD XMM6, [R11 + 16] ; T
    
    SUBSD XMM5, XMM4 ; (Vf - Vi)
    DIVSD XMM5, XMM7 ; Convert KM/H to m/s
    DIVSD XMM5, XMM6 ; / T
    CVTSD2SI R13, XMM5 ; A in int
    
    ; 4 - Size of int
    MOV [R8 + RCX * 4], R13D
    
    INC RCX
    JMP LOOP_MATRIX

END_LOOP:
    RET