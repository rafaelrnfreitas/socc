.global main

main:
	movq $0, %rax
	cmpq $0, %rax
	sete %al
	movzbq %al, %rax
	ret
