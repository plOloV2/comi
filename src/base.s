.global base                //calculates position in array of distances beetwen points 
.section .text              //takes 3 x int as inputs in this order: first point id, number of all points, socond point id

base:
    cmp w0, w2
    bgt swap                //if w0 > w2 go to swap:

continue:
    mov w11, #0x0           //w11 = 0
    cmp w0, #0x0
    beq return              //if w0 == w11 go to return:

    mov w9, w0              //w9 = w0
    mov w10, #0x2           //w10 = 2
    sub w11, w1, w10        //w11 = w1 - w10

loop:
    cmp w9, #0x1
    ble return              //if w9 <= 1 go to return:
    sub w9, w9, #0x1        //w9 -= 1
    add w10, w10, #0x1      //w10 += 1
    sub w12, w1, w10        //w12 = w1 - w10
    add w11, w11, w12       //w11 += w12
    b loop                  //go back to start of loop

swap:
    mov w11, w0             //w11 = w0
    mov w0, w2              //w0 = w2
    mov w2, w11             //w2 = w11
    b continue              //go to continue

return:
    mov w0, w11             //w0 = w11
    sub w0, w0, #0x1        //w0 -= 1
    add w0, w0, w2          //w0 += w2
    ret                     //return values
