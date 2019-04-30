
# 2. Function prologue and epilogue

## Function prologue

A function prologue is a sequence of instructions at the start of a function. It often looks something like the following code fragment:
```nasm
	push 	ebp
	mov 	ebp, esp
	sub 	esp, X
```

What these instruction do: save the value in the `ebp` register, set the value of the `ebp` register to the value of the `esp` and then allocate space on the stack for local variables.

The value in the `ebp` stays the same over the period of the function execution and is to be used for local variables and argument access. For the same purpose one can use `esp`, but since it changes over time this approach is not too convenient.

## Function epilogue

The function epilogue frees the allocated space in the stack, returns the value in the `ebp` register back to its initial state and returns the control flow to the caller:
```nasm
	mov 	esp, ebp
	pop 	ebp
	ret 	0
```

Function prologues and epilogues are usually detected in disassemblers for function delimitation. 

## Reference

* [Reverse Engineering for Beginners By Dennis Yurichev](https://beginners.re/RE4B-EN.pdf)