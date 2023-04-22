# 第三章作业

## 单周期作业

## 

## 

### COD5 4.1

#### 4.1.1

```assembly
RegWrite = 1
ALUSrc = 0
ALU operation = 0000
MemWrite = 0
MemRead = 0
MemtoReg = 0
```

#### 4.1.2

指令储存器、寄存器堆、ALU、多路选择器（、控制单元）

#### 4.1.3

不产生输出的单元：无

输出不被用到的单元：数据存储器、立即数产生器

### COD5 4.7

#### 4.7.1

PC read -> I-Mem -> Register File -> Mux -> ALU -> Mux -> Register Setup

30 + 250 + 150 + 25 + 200 + 25 + 20 = 700 (ps)

#### 4.7.2

PC read -> I-Mem -> Register File -> ALU -> D-Mem -> Mux -> Register Setup

30 + 250 + 150 + 200 + 250 + 25 + 20 = 925 (ps)

#### 4.7.3

PC read -> I-Mem -> Register File -> ALU -> D-Mem

30 + 250 + 150 + 200 + 250 = 880 (ps)

#### 4.7.4

PC read -> I-Mem -> Register File -> ALU -> Single Gate -> Mux -> Register Setup

30 + 250 + 150 + 25 + 200 + 5 + 25 + 20 = 705 (ps)

#### 4.7.5

##### 不访问 D-Mem 的指令

PC read -> I-Mem -> Register File -> ALU -> Mux -> Register Setup

30 + 250 + 150 + 200 + 25 + 20 = 675 (ps)

##### 需要访问 D-Mem 的指令

PC read -> I-Mem -> Register File -> ALU -> D-Mem -> Mux -> Register Setup

30 + 250 + 150 + 200 + 250 + 25 + 20 = 925 (ps)

#### 4.7.6

最小时钟周期 = 最长的延迟 = 925ps

### 思考题

单周期处理器在一个周期内完成指令所有的微操作，思考：

#### 寻址方式如何实现

1. 立即数寻址：控制单元发出相应的ALUSrc信号，控制寄存器文件与ALU之间的多路复用器选择ImmGen产生的输出
2. 寄存器寻址：控制单元发出相应的ALUSrc信号，控制寄存器文件与ALU之间的多路复用器选择寄存器产生的输出
3. 基址寻址：控制单元发出相应的ALUSrc信号，控制寄存器文件与ALU之间的多路复用器选择ImmGen产生的输出
4. PC相对寻址：控制单元发出相应的Branch信号，控制图4-20右上方的Mux选择ImmGen和PC相加产生的输出

#### 周期宽度如何确定

根据各器件延迟，算出关键路径（最大）延迟，周期宽度的最小值就是延迟的最大值，为了确保稳妥，周期宽度最好比延迟最大值大一些。

#### 能否“在一个clk内完成”

如果考虑各器件延迟，数据通路关键路径延迟小于一个时钟周期，则可以在一个clk内完成

#### 能否将两个adder合二为一

可以，一个操作数为PC，另一个操作数通过Mux和控制信号来选择是+4还是PC相对跳转

#### 能否将两个memory合二为一

可以，指令存储器没有写操作，不过为了避免混乱，使用双端口的Memory

### 附加题

布尔表达式：

```assembly
is_R_type	= Op4 * Op5
is_ld		= ~(Op4 + Op5)
is_sd		= Op4 ^ Op5 ^ Op6
is_beq		= Op5 * Op6

ALUSrc		= is_R_type + is_sd
			= Op4 * Op5 + Op4 ^ Op5 ^ Op6
			= Op4 * Op5 + ~Op4 * ~Op5 * Op6 + ~Op4 * Op5 * ~Op6 + Op4 * ~Op5 * ~Op6
```

其他控制信号类似产生，PLA图片略。

## 多周期作业

## 

### 每一类指令的指令周期各含多少个时钟周期？

1. R型指令：4个时钟周期
2. sd指令：4个时钟周期
3. B型指令：3个时钟周期
4. J型指令：3个时钟周期
5. U型指令：3个时钟周期
6. lw指令：5个时钟周期
7. 其他I型指令：4个时钟周期

### 分别分析R/I/S/B-type指令的多周期设计方案中每个周期所用到的功能部件

|      |              取指              |               译码               |          执行          |           访存            |     写回      |
| :--: | :----------------------------: | :------------------------------: | :--------------------: | :-----------------------: | :-----------: |
|  R   | PC寄存器、指令存储器、Mux、ALU |          Mux、寄存器堆           |        ALU、Mux        |       Mux、寄存器堆       |               |
|  I   | PC寄存器、指令存储器、Mux、ALU |          Mux、寄存器堆           | ALU、Mux、立即数产生器 | Mux、寄存器堆或数据存储器 | Mux、寄存器堆 |
|  S   | PC寄存器、指令存储器、Mux、ALU |          Mux、寄存器堆           | ALU、Mux、立即数产生器 |      Mux、数据存储器      |               |
|  B   | PC寄存器、指令存储器、Mux、ALU | Mux、寄存器堆、立即数产生器、ALU |   ALU、Mux、PC寄存器   |                           |               |

