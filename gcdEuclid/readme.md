Project: *GCD Euclidian Algorithm*
Author: Delwys Glokpor (dnglokpor)
Language: ARMAssembly
Date: 2/8/21
Description:
Uses Euclid algorithm to compute the gcd of two numbers. Processing is done by using registers R0 and R1. no IO nor print to terminal operations. Values of the two numbers must be set manually in the *"gcd.s"*. Said file must then be assembled and linked. Executable should be run. Result is returned as exit code thus unix command `echo $?` will reveal it.
Repository Structure:
- "gcd.s": file: assembly source code
- "gcd.o": file: assembled object file
- "gcdx": file: linked executable program
- "gcddx": file: (optional) debug executable file
