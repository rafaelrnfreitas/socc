.global main

main:
	movq $0, %rax
	cmpq $0, %rax
	jne _true_0
	movq $1, %rax
	cmpq $0, %rax
	jne _true_0
	movq $0, %rax
	jmp _end_0
_true_0:
	movq $1, %rax
_end_0:
	ret
