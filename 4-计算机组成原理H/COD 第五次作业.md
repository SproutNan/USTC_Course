# COD 第五次作业

PB20111686 黄瑞轩

## 4.16

### 4.16.4

$\delta(\text{MEM})=\delta(\text{Load})+\delta(\text{Store})=20\%+15\%=35\%$

### 4.16.5

$\delta(\text{RegWrite})=\delta(\text{ALU/Logic})+\delta(\text{Load})=45\%+20\%=65\%$

## 4.22

### 4.22.1

![image-20220507091636021](C:/Users/10258/AppData/Roaming/Typora/typora-user-images/image-20220507091636021.png)

stall 的原因：这里使用的是单端口且数据、指令 MEM 合一的结构，所以当同时需要 IF 和 MEM 时就需要 stall。

### 4.22.2

不能，因为每个周期都需要 Fetch 指令，所以当流水线向后运作的时候一定会出现 IF 和 MEM 重合的情况。

### 4.22.3

必须通过硬件来处理。NOP 也是指令（事实上就是 `addi	x0, x0, 0`），也需要取指令、译码，所以还是会出现 IF 和 MEM 重合的情况。

### 4.22.4

由于 IF 每周期都有，只需考虑 MEM 的出现即可。出现 MEM 就要延迟一个周期。

$\delta(\text{Stall})=\delta(\text{ld})+\delta(\text{sd})=36\%$。

## 4.25

### 4.25.1

![image-20220507093841118](C:/Users/10258/AppData/Roaming/Typora/typora-user-images/image-20220507093841118.png)

### 4.25.2

![image-20220507094109186](C:/Users/10258/AppData/Roaming/Typora/typora-user-images/image-20220507094109186.png)

没有这样的情况出现，要么被 stall，要么没有满负荷工作，要么进入无效的 MEM 或 WB 阶段。