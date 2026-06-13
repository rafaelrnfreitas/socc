.global main

main:
	movq $64, %rax
	pushq %rax
	movq $2, %rax
	popq %rbx
	movq %rax, %rcx
	movq %rbx, %rax
	sarq %cl, %rax
	ret
