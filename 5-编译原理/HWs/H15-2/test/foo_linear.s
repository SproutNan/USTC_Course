	.text
	.def	 foo;
	.scl	2;
	.type	32;
	.endef
	.globl	foo
	.align	16, 0x90
foo:                                    # @foo
.Ltmp0:
.seh_proc foo
# BB#0:                                 # %entry
	subq	$56, %rsp
.Ltmp1:
	.seh_stackalloc 56
.Ltmp2:
	.seh_endprologue
	movl	$1, %eax
	movl	%ecx, 48(%rsp)
	cmpl	48(%rsp), %eax
	jne	.LBB0_2
# BB#1:                                 # %if.then
	movl	$1, 52(%rsp)
	jmp	.LBB0_3
.LBB0_2:                                # %if.end
	movl	$3, %eax
	movl	48(%rsp), %ecx
	movl	%eax, 44(%rsp)          # 4-byte Spill
	movl	%ecx, %eax
	cltd
	movl	44(%rsp), %ecx          # 4-byte Reload
	idivl	%ecx
	addl	$1, %edx
	movl	48(%rsp), %r8d
	subl	$1, %r8d
	movl	%r8d, %ecx
	movl	%edx, 40(%rsp)          # 4-byte Spill
	callq	foo
	movl	40(%rsp), %ecx          # 4-byte Reload
	imull	%eax, %ecx
	movl	%ecx, 52(%rsp)
.LBB0_3:                                # %return
	movl	52(%rsp), %eax
	addq	$56, %rsp
	retq
.Ltmp3:
	.seh_endproc

	.def	 main;
	.scl	2;
	.type	32;
	.endef
	.globl	main
	.align	16, 0x90
main:                                   # @main
.Ltmp4:
.seh_proc main
# BB#0:                                 # %entry
	pushq	%rbp
.Ltmp5:
	.seh_pushreg 5
	subq	$48, %rsp
.Ltmp6:
	.seh_stackalloc 48
	leaq	48(%rsp), %rbp
.Ltmp7:
	.seh_setframe 5, 48
.Ltmp8:
	.seh_endprologue
	callq	__main
	movl	$0, -4(%rbp)
	movl	$10, %ecx
	callq	foo
	nop
	addq	$48, %rsp
	popq	%rbp
	retq
.Ltmp9:
	.seh_endproc


