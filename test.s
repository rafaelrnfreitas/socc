.global main

main:
	movq $8, %rax
	pushq %rax
	movq $2, %rax
	popq %rbx
	movq %rax, %rcx
	movq %rbx, %rax
	cqto
	idivq %rcx
	ret
