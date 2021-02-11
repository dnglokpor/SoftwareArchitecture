@ CSCI 263 Assignment: GCD Euclidian Algorithm:
@ Prompt: ARM program that calculates the GCD of two numbers
@ stored in R0 and R1 and prints it as exit code.
@ Author: Delwys Glokpor
@ Spring 2021
.global _start
_start:
mov R0, #99 @ x = 99
mov R1, #55 @ y = 55
loopTop: @ main loop
cmp R0, R1
beq return @ while x != y
blt secondCase @ if x > y
sub R0, R0, R1 @ x = x - y
b loopTop @ early next iteration
secondCase:
sub R1, R1, R0 @ y = y - x
b loopTop @ late next iteration
return: @ loop end
mov R7, #1
swi 0 @ end of program - returns contents of R0.

