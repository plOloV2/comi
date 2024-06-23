.global Distance
.section .text

Distance:
    sub w0, w0, w2      //w0 -= w2
    sub w1, w1, w3      //w1 -= w3
    mul w1, w1, w1      //w1 *= w1
    madd w0, w0, w0, w1 //w0 = w1 + w0 * w0
    scvtf s0, w0        //conversion to float
    fsqrt s0, s0        //s0 = sqrt s0
    ret
