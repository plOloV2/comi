.global base
.section .text

base:
    cmp w0, w2
    bgt swap

continue:
    mov w11, #0x0
    cmp w0, #0x0
    beq return

    mov w9, w0
    mov w10, #0x2
    sub w11, w1, w10

loop:
    cmp w9, #0x1
    ble return
    sub w9, w9, #0x1
    add w10, w10, #0x1
    sub w12, w1, w10
    add w11, w11, w12
    b loop

swap:
    mov w11, w0
    mov w0, w2
    mov w2, w11
    b continue

return:
    mov w0, w11
    sub w0, w0, #0x1
    add w0, w0, w2
    ret
