.global Distance
.section .text

Distance:
    sub w0, w0, w2      //w0 -= w2
    sub w1, w1, w3      //w1 -= w3
    mul w1, w1, w1      //w1 *= w1
    madd w0, w0, w0, w1 //w0 = w1 + w0 * w0
    scvtf d0, w0        //conversion to double
    fsqrt d0, d0        //d0 = sqrt d0
    ret
