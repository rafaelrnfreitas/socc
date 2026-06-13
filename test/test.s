.global main

main:
	movq $1, %rax
	not %rax
	ret
