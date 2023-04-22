# 编译原理实验-GCC/Clang工具链使用

<center>黄瑞轩 PB20111686</center>

## 第1关

### 问题1-1

在命令行下执行 `gcc -DNEG -E sample.c -o sample.i` 后的输出结果与之前的区别是

```
root@evassh-12142603:/data/workspace/myshixun# diff sample.i sample_DNEG.i
14c14
<     int a = 4;
---
>     int a = -4;
```

后者使用 -DNEG，相当于定义这个 NEG 宏，默认这个宏的内容是 1，所以在 `sample_DNEG.i` 中，原本用 4 替换的位置变成了 -4。

### 问题1-2

通过 diff 命令可以给出两个代码的不同之处：

```
root@evassh-12142603:/data/workspace/myshixun# diff sample-32.s sample.s
8,15c8,14
<       pushl   %ebp
<       .cfi_def_cfa_offset 8
<       .cfi_offset 5, -8
<       movl    %esp, %ebp
<       .cfi_def_cfa_register 5
<       subl    $16, %esp
<       movl    $4, -4(%ebp)
<       cmpl    $0, -4(%ebp)
---
>       pushq   %rbp
>       .cfi_def_cfa_offset 16
>       .cfi_offset 6, -16
>       movq    %rsp, %rbp
>       .cfi_def_cfa_register 6
>       movl    $4, -4(%rbp)
>       cmpl    $0, -4(%rbp)
17c16
<       addl    $4, -4(%ebp)
---
>       addl    $4, -4(%rbp)
20c19
<       sall    $2, -4(%ebp)
---
>       sall    $2, -4(%rbp)
23,25c22,23
<       leave
<       .cfi_restore 5
<       .cfi_def_cfa 4, 4
---
>       popq    %rbp
>       .cfi_def_cfa 7, 8
```

1. `pushl` 和 `pushq` 的区别

   二者都是压栈操作，区别在于 `pushl` 是 32 位操作，压入双字； `pushq` 是 64 位操作，压入两个双字。

2. `%esp`、`%ebp` 与 `$rsp`、`$rbp` 的区别

   这些都是寄存器，`%e` 开头的是 32 位的寄存器，`%r` 开头的是 64 位的寄存器。相应的前者是相应的后者的低 32 位。

3. 最后一处，`leave` 相当于执行了 `popq %ebp`，即释放栈帧，没有本质上的区别

**产生原因**：分别采用了 32 位和 64 位编译，产生了适配不同位机器的汇编代码。

### 问题1-3

gcc 和 clang 生成的目标文件反汇编区别：

```
root@evassh-12142603:/data/workspace/myshixun# diff dump.txt 1-clang-ver/dump.txt 
10,18c10,23
<    4: c7 45 fc 04 00 00 00    movl   $0x4,-0x4(%rbp)
<    b: 83 7d fc 00             cmpl   $0x0,-0x4(%rbp)
<    f: 74 06                   je     17 <main+0x17>
<   11: 83 45 fc 04             addl   $0x4,-0x4(%rbp)
<   15: eb 04                   jmp    1b <main+0x1b>
<   17: c1 65 fc 02             shll   $0x2,-0x4(%rbp)
<   1b: b8 00 00 00 00          mov    $0x0,%eax
<   20: 5d                      pop    %rbp
<   21: c3                      retq   
---
>    4: c7 45 fc 00 00 00 00    movl   $0x0,-0x4(%rbp)
>    b: c7 45 f8 04 00 00 00    movl   $0x4,-0x8(%rbp)
>   12: 83 7d f8 00             cmpl   $0x0,-0x8(%rbp)
>   16: 0f 84 0e 00 00 00       je     2a <main+0x2a>
>   1c: 8b 45 f8                mov    -0x8(%rbp),%eax
>   1f: 83 c0 04                add    $0x4,%eax
>   22: 89 45 f8                mov    %eax,-0x8(%rbp)
>   25: e9 09 00 00 00          jmpq   33 <main+0x33>
>   2a: 8b 45 f8                mov    -0x8(%rbp),%eax
>   2d: c1 e0 02                shl    $0x2,%eax
>   30: 89 45 f8                mov    %eax,-0x8(%rbp)
>   33: 31 c0                   xor    %eax,%eax
>   35: 5d                      pop    %rbp
>   36: c3                      retq  
```

后者是 clang 生成的，生成的代码行数更多，但主要是对具体语句的实现有差别，整体的框架差不多。

其他步骤的输出都相同。

## 第3关

1. 使用各编译选项后代码所作的变换：

   （1）使用 -E 后，源代码 include 的头文件被展开。由于没有任何宏等，代码主体未被预处理器替换。

   （2）使用 -S 后，源代码被翻译成汇编代码，其他选项为默认。

   （3）在 -S 选项的基础上加上 -m32 或者 -m64 选项之后，生成的汇编将强制使用 32 位或 64 位指令，生成的汇编代码与（2）生成的均有差异，说明 -S 选项并不默认指定位数。因为（2）出现了 r 开头的寄存器，说明平台是两种位数的代码都支持。

   （4）使用 -c 后，生成的是二进制机器码（*.o 文件）。

   （5）使用 -o 主要是指定输出文件的文件名。

2. 使用 clang 编译器来看 -m32 和 -m64 的区别：

   ```shell
   $ clang -S -m32 sort.c -o sort_32.s
   $ clang -S -m64 sort.c -o sort_64.s   
   ```
   
   （1）64 位汇编码中出现一些 `.cfi_` 开头的指令。其用处为：在某些体系结构上，必须使用调用帧信息指令来管理异常处理。`cfi` 指令用于调试，它允许调试器展开堆栈，调试器可以通过使用堆栈指针 (%rsp) 并注册 %rbp 来展开此堆栈。
   
   ```assembly
   # 64位汇编
   >       .cfi_def_cfa_offset 16
   >       .cfi_offset %rbp, -16
   >       movq    %rsp, %rbp
   >       .cfi_def_cfa_register %rbp
   ```
   
   （2）不同位数的汇编码使用了不同长度的寄存器和配套指令，比如寄存器的 r 和 e（如 %rsp 和 %esp），以及指令的 l 和 q（如 pushl 和 pushq）。并且由于位数不同，在寻址偏移常数上也有所改变。
   
   ```assembly
   # 前半部分32位，后半部分64位
   27,28c27,28
   <       movl    -12(%ebp), %eax
   <       cmpl    -8(%ebp), %eax
   ---
   >       movl    -24(%rbp), %eax
   >       cmpl    -20(%rbp), %eax
   40,45c40,47
   <       movl    12(%ebp), %eax
   <       movl    -16(%ebp), %ecx
   <       movl    (%eax,%ecx,4), %eax
   <       movl    12(%ebp), %ecx
   <       movl    -16(%ebp), %edx
   <       cmpl    4(%ecx,%edx,4), %eax
   ---
   >       movq    -16(%rbp), %rax
   >       movslq  -28(%rbp), %rcx
   >       movl    (%rax,%rcx,4), %edx
   >       movq    -16(%rbp), %rax
   >       movl    -28(%rbp), %esi
   >       addl    $1, %esi
   >       movslq  %esi, %rcx
   >       cmpl    (%rax,%rcx,4), %edx
   ```
   
3. 使用 -m32 选项来看 gcc 和 clang 生成汇编码的不同：

   ```shell
   $ gcc -S -m32 sort.c -o sort_gcc.s
   $ clang -S -m32 sort.c -o sort_clang.s
   ```
   
   （1）具体实现不同。如 `sort()` 函数，gcc 版本在 L5 循环中使用了多种指令来完成，但 clang 版本在 LBB0_3（对应 L5）中几乎全部用 movl 指令实现。

   ```assembly
   # gcc版本
   .L5:
           movl    -12(%ebp), %eax
           leal    0(,%eax,4), %edx
           movl    12(%ebp), %eax
           addl    %edx, %eax
           movl    (%eax), %edx
           movl    -12(%ebp), %eax
           addl    $1, %eax
           leal    0(,%eax,4), %ecx
           movl    12(%ebp), %eax
           addl    %ecx, %eax
           movl    (%eax), %eax
           cmpl    %eax, %edx
           jle     .L4
           movl    -12(%ebp), %eax
           leal    0(,%eax,4), %edx
           movl    12(%ebp), %eax
           addl    %edx, %eax
           movl    (%eax), %eax
           movl    %eax, -4(%ebp)
           movl    -12(%ebp), %eax
           leal    0(,%eax,4), %edx
           movl    12(%ebp), %eax
           addl    %eax, %edx
           movl    -12(%ebp), %eax
           addl    $1, %eax
           leal    0(,%eax,4), %ecx
           movl    12(%ebp), %eax
           addl    %ecx, %eax
           movl    (%eax), %eax
           movl    %eax, (%edx)
           movl    -12(%ebp), %eax
           addl    $1, %eax
           leal    0(,%eax,4), %edx
           movl    12(%ebp), %eax
           addl    %eax, %edx
           movl    -4(%ebp), %eax
           movl    %eax, (%edx)
   ```
   
   ```assembly
   # clang版本
   .LBB0_3:                                #   Parent Loop BB0_1 Depth=1
                                           # =>  This Inner Loop Header: Depth=2
           movl    -16(%ebp), %eax
           movl    8(%ebp), %ecx
           subl    $1, %ecx
           cmpl    %ecx, %eax
           jge     .LBB0_8
   # %bb.4:                                #   in Loop: Header=BB0_3 Depth=2
           movl    12(%ebp), %eax
           movl    -16(%ebp), %ecx
           movl    (%eax,%ecx,4), %eax
           movl    12(%ebp), %ecx
           movl    -16(%ebp), %edx
           cmpl    4(%ecx,%edx,4), %eax
           jle     .LBB0_6
   # %bb.5:                                #   in Loop: Header=BB0_3 Depth=2
           movl    12(%ebp), %eax
           movl    -16(%ebp), %ecx
           movl    (%eax,%ecx,4), %eax
           movl    %eax, -20(%ebp)
           movl    12(%ebp), %eax
           movl    -16(%ebp), %ecx
           movl    4(%eax,%ecx,4), %eax
           movl    12(%ebp), %ecx
           movl    -16(%ebp), %edx
           movl    %eax, (%ecx,%edx,4)
           movl    -20(%ebp), %eax
           movl    12(%ebp), %ecx
           movl    -16(%ebp), %edx
           movl    %eax, 4(%ecx,%edx,4)
   ```
   
   （2）gcc 版本的汇编码中将 `"%d"` 之类的字符串当成普通字符串；在 clang 中被当成一种对象（Object）处理，包含内容和长度。
   
   ```assembly
   # gcc版本
   .LC0:
           .string "%d"
   .LC1:
           .string "%d "
   ```
   
   ```assembly
   # clang版本
           .type   .L.str,@object          # @.str
           .section        .rodata.str1.1,"aMS",@progbits,1
   .L.str:
           .asciz  "%d"
           .size   .L.str, 3
   
           .type   .L.str.1,@object        # @.str.1
   .L.str.1:
           .asciz  "%d "
           .size   .L.str.1, 4
   ```
   
   （3）gcc 版本即使在 32 位版本中也会添加 `.cfi` 指令。
   
   ```assembly
   # gcc版本
   sort:
   .LFB2:
           .cfi_startproc
           pushl   %ebp
           .cfi_def_cfa_offset 8
           .cfi_offset 5, -8
           movl    %esp, %ebp
           .cfi_def_cfa_register 5
           subl    $16, %esp
           # ……
   ```
   
   ```assembly
   # clang版本
   sort:                                   # @sort
   # %bb.0:
           pushl   %ebp
           movl    %esp, %ebp
           pushl   %esi
           subl    $20, %esp
           # ……
   ```
   
   （4）clang 生成的汇编码会附带很多注释，如循环深度、函数开始与结束标记和数据结构类型等。
   
   ```assembly
   # clang版本
   .LBB0_6:                                #   in Loop: Header=BB0_3 Depth=2
           jmp     .LBB0_7
   .LBB0_7:                                #   in Loop: Header=BB0_3 Depth=2
           movl    -16(%ebp), %eax
           addl    $1, %eax
           movl    %eax, -16(%ebp)
           jmp     .LBB0_3
   .LBB0_8:                                #   in Loop: Header=BB0_1 Depth=1
           jmp     .LBB0_9
           
   # ……
   
   .Lfunc_end0:
           .size   sort, .Lfunc_end0-sort
                                           # -- End function
           .globl  main                    # -- Begin function main
           .p2align        4, 0x90
           .type   main,@function
   main:                                   # @main
   # %bb.0:
           pushl   %ebp
   
   # ……
   ```
   
4. 对 clang 生成的 32 位汇编代码详细注释如下：

   ```assembly
           .text
           .file   "sort.c"
           .globl  sort                    # -- Begin function sort
           .p2align        4, 0x90
           .type   sort,@function
   sort:                                   # @sort
   # %bb.0:
           pushl   %ebp					# 保存基址寄存器ebp
           movl    %esp, %ebp				# 把栈顶寄存器的值存入ebp
           pushl   %esi					# 保存源地址指针esi
           subl    $20, %esp				# 在栈顶分配20字节的空间
           movl    12(%ebp), %eax			# 从栈上取出各参数保存到寄存器
           movl    8(%ebp), %ecx				# eax = int* base
           movl    8(%ebp), %edx				# edx = ecx = esi = int number
           movl    8(%ebp), %esi
           addl    $1, %esi				# esi += 1
           imull   %esi, %edx				# edx *= esi
           movl    %eax, -24(%ebp)         # -24(ebp) = eax(base)
           movl    %edx, %eax				# eax = edx = number * (number + 1)
           cltd							# 将eax寄存器的值符号扩展32位到edx寄存器
           movl    $2, %esi				# esi = 2
           idivl   %esi					# eax /= esi -> eax = n * (n + 1) / 2
           movl    %eax, -8(%ebp)			# -8(ebp) = eax = n * (n + 1)
           movl    $0, -12(%ebp)			# -12(ebp) = 0
   .LBB0_1:                                # =>This Loop Header: Depth=1
                                           #     Child Loop BB0_3 Depth 2
           movl    -12(%ebp), %eax			# eax = 0(int j)
           cmpl    -8(%ebp), %eax			# 比较 eax(j) 和 n * (n + 1) / 2
           jge     .LBB0_10				# if j >= n * (n + 1) / 2, jump .LBB0_10(跳出循环)
   # %bb.2:                                #   in Loop: Header=BB0_1 Depth=1
           movl    $0, -16(%ebp)			# -16(ebp) = 0
   .LBB0_3:                                #   Parent Loop BB0_1 Depth=1
                                           # =>  This Inner Loop Header: Depth=2
           movl    -16(%ebp), %eax			# eax = -16(ebp) = 0(int i)
           movl    8(%ebp), %ecx			# ecx = number
           subl    $1, %ecx				# ecx -= 1
           cmpl    %ecx, %eax				# 比较 eax(i) 和 number - 1
           jge     .LBB0_8					# if i >= number - 1, jump .LBB0_8(跳出循环)
   # %bb.4:                                #   in Loop: Header=BB0_3 Depth=2
           movl    12(%ebp), %eax			# eax = 12(ebp) (int* base)
           movl    -16(%ebp), %ecx			# ecx = 0
           movl    (%eax,%ecx,4), %eax		# eax = [base + 4*ecx](base[i])
           movl    12(%ebp), %ecx			# ecx = base
           movl    -16(%ebp), %edx			# edx = 0
           cmpl    4(%ecx,%edx,4), %eax	# 比较 [base + 4*ecx](base[i])和 [base + 4*ecx + 4](base[i+1]) 
           jle     .LBB0_6					# 如果小于，则跳过swap操作
   # %bb.5:                                #   in Loop: Header=BB0_3 Depth=2
           movl    12(%ebp), %eax			# 这里是 swap 操作，eax = base
           movl    -16(%ebp), %ecx			# ecx = i
           movl    (%eax,%ecx,4), %eax		# eax = base[i]
           movl    %eax, -20(%ebp)			# -20(ebp) = base[i] (前者是tmp变量)
           movl    12(%ebp), %eax			# eax = base
           movl    -16(%ebp), %ecx			# ecx = i
           movl    4(%eax,%ecx,4), %eax	# eax = base[i+1]
           movl    12(%ebp), %ecx			# ecx = base
           movl    -16(%ebp), %edx			# edx = i
           movl    %eax, (%ecx,%edx,4)		# base[i] = base[i+1]
           movl    -20(%ebp), %eax			# eax = tmp
           movl    12(%ebp), %ecx			# ecx = base
           movl    -16(%ebp), %edx			# edx = i
           movl    %eax, 4(%ecx,%edx,4)	# base[i+1] = tmp
   .LBB0_6:                                #   in Loop: Header=BB0_3 Depth=2
           jmp     .LBB0_7
   .LBB0_7:                                #   in Loop: Header=BB0_3 Depth=2
           movl    -16(%ebp), %eax			# eax = i
           addl    $1, %eax				# i++
           movl    %eax, -16(%ebp)			# -16(ebp) = 当时的i+1
           jmp     .LBB0_3					# 返回，继续循环
   .LBB0_8:                                #   in Loop: Header=BB0_1 Depth=1
           jmp     .LBB0_9
   .LBB0_9:                                #   in Loop: Header=BB0_1 Depth=1
           movl    -12(%ebp), %eax			# eax = j
           addl    $1, %eax				# j++
           movl    %eax, -12(%ebp)			# -12(ebp) = 当时的j+1
           jmp     .LBB0_1					# 返回，继续循环
   .LBB0_10:
           addl    $20, %esp				# 栈顶释放20字节的空间
           popl    %esi					# 弹出esi
           popl    %ebp					# 弹出ebp
           retl							# 函数返回
   .Lfunc_end0:
           .size   sort, .Lfunc_end0-sort
                                           # -- End function
           .globl  main                    # -- Begin function main
           .p2align        4, 0x90
           .type   main,@function
   main:                                   # @main
   # %bb.0:
           pushl   %ebp					# 保存基址寄存器ebp
           movl    %esp, %ebp				# 把栈顶寄存器的值存入ebp
           subl    $40, %esp				# 在栈顶分配40字节空间
           movl    $0, -4(%ebp)			# -4(ebp)=0
           leal    .L.str, %eax			# eax = "%d"(这里等号意思是首地址，下面不解释了)
           movl    %eax, (%esp)			# [esp]="%d"(入栈，传参数，下面不解释了)
           leal    -8(%ebp), %eax			# eax = -8(ebp) = number
           movl    %eax, 4(%esp)			# [esp+4] = number
           calll   __isoc99_scanf			# 调用scanf函数，下面是malloc分配空间
           movl    -8(%ebp), %ecx			# ecx = number
           shll    $2, %ecx				# ecx *= 4
           movl    %ecx, (%esp)			# [esp]=number*4
           movl    %eax, -24(%ebp)         # -24(ebp) = number
           calll   malloc					# 调用malloc函数
           movl    %eax, -12(%ebp)			# -12(ebp) 存数组基地址指针 base
           movl    $0, -16(%ebp)			# -16(ebp) = 0
   .LBB1_1:                                # =>This Inner Loop Header: Depth=1
           movl    -16(%ebp), %eax			# eax = 0（int i）
           cmpl    -8(%ebp), %eax			# 比较 i 和 number
           jge     .LBB1_4					# if i>=number, 跳出循环
   # %bb.2:                                #   in Loop: Header=BB1_1 Depth=1
           movl    -12(%ebp), %eax			# eax = base
           movl    -16(%ebp), %ecx			# ecx = i
           shll    $2, %ecx				# ecx = 4*i
           addl    %ecx, %eax				# eax = base+4*i
           leal    .L.str, %ecx			# ecx = "%d"
           movl    %ecx, (%esp)			# [esp] = "%d"
           movl    %eax, 4(%esp)			# [esp+4] = base+4*i
           calll   __isoc99_scanf			# 调用scanf函数，传递栈上两个参数（就上面两行）
   # %bb.3:                                #   in Loop: Header=BB1_1 Depth=1
           movl    -16(%ebp), %eax			# eax = i
           addl    $1, %eax				# eax = i+1
           movl    %eax, -16(%ebp)			# -16(ebp) = 原来的i+1
           jmp     .LBB1_1					# 返回，重新循环
   .LBB1_4:
           movl    -8(%ebp), %eax			# eax = number
           movl    -12(%ebp), %ecx			# ecx = base
           movl    %eax, (%esp)			# [esp] = number
           movl    %ecx, 4(%esp)			# [esp+4] = base
           calll   sort					# 调用sort函数
           movl    $0, -20(%ebp)			# -20(ebp) = 0(int i)
   .LBB1_5:                                # =>This Inner Loop Header: Depth=1
           movl    -20(%ebp), %eax			# eax = i
           cmpl    -8(%ebp), %eax			# 比较i和number
           jge     .LBB1_8					# 如果i>=number，退出循环
   # %bb.6:                                #   in Loop: Header=BB1_5 Depth=1
           movl    -12(%ebp), %eax			# eax = base
           movl    -20(%ebp), %ecx			# ecx = i
           movl    (%eax,%ecx,4), %eax		# eax = base[i]
           leal    .L.str.1, %ecx			# ecx = "%d "
           movl    %ecx, (%esp)			# [esp] = "%d "
           movl    %eax, 4(%esp)			# [esp+4] = base[i]
           calll   printf					# 调用printf函数
   # %bb.7:                                #   in Loop: Header=BB1_5 Depth=1
           movl    -20(%ebp), %eax			# eax = i
           addl    $1, %eax				# eax = i+1
           movl    %eax, -20(%ebp)			# -20(ebp) = 原来的i+1
           jmp     .LBB1_5					# 返回，重新循环
   .LBB1_8:
           movl    -12(%ebp), %eax			# eax = base
           movl    %eax, (%esp)			# [esp] = eax
           calll   free					# 调用free函数释放内存
           movl    -4(%ebp), %eax			# eax = 0（返回值）
           addl    $40, %esp				# 释放栈
           popl    %ebp					# ebp出栈
           retl							# main函数返回
   .Lfunc_end1:
           .size   main, .Lfunc_end1-main
                                           # -- End function
           .type   .L.str,@object          # @.str
           .section        .rodata.str1.1,"aMS",@progbits,1
   .L.str:
           .asciz  "%d"
           .size   .L.str, 3
   
           .type   .L.str.1,@object        # @.str.1
   .L.str.1:
           .asciz  "%d "
           .size   .L.str.1, 4
   
           .ident  "clang version 10.0.1 "
           .section        ".note.GNU-stack","",@progbits
           .addrsig
           .addrsig_sym sort
           .addrsig_sym __isoc99_scanf
           .addrsig_sym malloc
           .addrsig_sym printf
           .addrsig_sym free
   ```

   ```assembly
   root@evassh-12142603:/data/workspace/myshixun# gcc -E sample.c -o sample.i
   root@evassh-12142603:/data/workspace/myshixun# clang -E sample.c -o sample_.i
   root@evassh-12142603:/data/workspace/myshixun# diff sample.i sample_.i
   2,6c2,7
   < # 1 "<built-in>"
   < # 1 "<command-line>"
   < # 1 "/usr/include/stdc-predef.h" 1 3 4
   < # 1 "<command-line>" 2
   < # 1 "sample.c"
   ---
   > # 1 "<built-in>" 1
   > # 1 "<built-in>" 3
   > # 341 "<built-in>" 3
   > # 1 "<command line>" 1
   > # 1 "<built-in>" 2
   > # 1 "sample.c" 2
   ```

   
