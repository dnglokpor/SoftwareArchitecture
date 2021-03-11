   .arch armv6
   .file   "InsertSort.c"
   .text
   .align   2
   .syntax unified
   .arm
   .type   insertSort, %function
insertSort:
   str   fp, [sp, #-4]!
   sub   sp, sp, #12
   mov   r3, #1
   str   r3, [fp, #-12]
   b   .L2
.L6:
   ldr   r2, [fp, #-12]
   str   r2, [fp, #-4]
   mov   r2, r2, lsl #2
   add   r2, r1, r2
   ldr   r2, [r2]
   str   r2, [fp, #-4]
   b   .L3
.L5:
   ldr   r3, [fp, #-4]
   mov   r3, r3, lsl #2
   add   r3, r0, r3
   sub   r2, r3, #4
   ldr   r2, [r2]
   str   r2, [r3]
   ldr   r3, [fp, #-4]
   sub   r3, r3, #1
   str   r3, [fp, #-4]
.L3:
   ldr   r3, [fp, #-4]
   cmp   r3, #0
   ble   .L4
   ldr   r3, [fp, #-4]
   sub   r3, r3, #1
   mov   r3, r3, lsl #2
   add   r3, r0, r3
   ldr   r2, [r3]
   ldr   r3, [fp, #-8]
   cmp   r2, r3
   ble   .L5
.L4:
   ldr   r3, [fp, #-4]
   mov   r3, r3, lsl #2
   add   r3, r0, r3
   ldr   r2, [fp, #-8]
   str   r2, [r3]
   ldr   r3, [fp, #-12]
   add   r3, r3, #1
   str   r3, [fp, #-12]
.L2:
   ldr   r2, [fp, #-12]
   mov   r3, r1
   cmp   r2, r3
   blt   .L6
   ldr   fp, [sp], #4
   bx   lr
   .size   insertSort, .-insertSort
   .section   .rodata
   .align   2
.LC0:
   .ascii   "%d\000"
   .align   2
.LC1:
   .ascii   "%d \000"
   .text
   .align   2
   .global   main
   .syntax unified
   .arm
   .fpu vfp
   .type   main, %function
main:
   push   {fp, lr}
   add   fp, sp, #4
   sub   sp, sp, #44
   mov   r3, #0
   str   r3, [fp, #-8]
   b   .L8
.L9:
   sub   r2, fp, #44
   ldr   r1, [fp, #-8]
   add   r1, r2, r1, lsl #2
   ldr   r0, .L13
   bl   __isoc99_scanf
   ldr   r3, [fp, #-8]
   add   r3, r3, #1
   str   r3, [fp, #-8]
.L8:
   ldr   r3, [fp, #-8]
   cmp   r3, #9
   ble   .L9
   sub   r0, fp, #48
   mov   r1, #10
   bl   insertSort
   mov   r3, #0
   str   r3, [fp, #-8]
   b   .L10
.L11:
   ldr   r3, [fp, #-8]
   add   r3, fp, r3, lsl #2
   ldr   r1, [r3, #-40]
   ldr   r0, .L13+4
   bl   printf
   ldr   r3, [fp, #-4]
   add   r3, r3, #1
   str   r3, [fp, #-4]
.L10:
   ldr   r3, [fp, #-8]
   cmp   r3, #9
   ble   .L11
   mov   r0, #10
   bl   putchar
   mov   r0, #0
   mov   sp, fp
   pop   {fp, lr}
.L14:
   .align   2
.L13:
   .word   .LC0
   .word   .LC1
   .size   main, .-main
   .ident   "GCC: (Raspbian 6.3.0-18+rpi1) 6.3.0 20170516"
   .section   .note.GNU-stack,"",%progbits
