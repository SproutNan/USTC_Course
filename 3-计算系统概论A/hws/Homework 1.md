# Homework 1

PB20111686    Ruixuan Huang

## Problem. 1

We have an assumption that the output signal can be copied as two input signals.

![image-20210926194447034](C:\Users\Sprout\AppData\Roaming\Typora\typora-user-images\image-20210926194447034.png)

## Problem. 2

**a.** We're required to find such an smallest $n$, which meets the conditions below.
$$
2^n\ge400
$$
Considering $n$ is an integer, so
$$
n=\lceil\log_2400\rceil=9
$$
**b.** Nine bits can represent $512$ students, so let $s$ be what we want
$$
s=2^9-400=112
$$

## Problem. 3

**a.** 00010110         **b.** 11111101         **c.** 11111000         **d.** 00000001

## Problem. 4

**a.** 01 + 1011 = 0001 + 1011 = 1100

**b.** 11 + 01010101 = 11111111 + 01010101 = 01010100

**c.** 0101 + 110 = 0101 + 1110 = 0011

**d.** 01 + 10 = 11

## Problem. 5

**a.** (01010101)~B~ = (85)~D~

**b.** (10001101)~B~ = (-115)~D~ 

**c.** (10000000)~B~ = (-128)~D~ 

**d.** (11111111)~B~ = (-1)~D~

## Problem. 6

$$
(0.3)_D=(0.01001100110...)_B=1.001100110...×2^{-2}
$$

It's a postive number, so the sign bit will be $0$. The exponent part is unsigned integer $125$'s 8-bit binary form, $01111101$. And the fraction part is $001100110...$.

**Answer:** $0\ 01111101\ 001100110...$

## Problem. 7

**Sign :** Negative

**Exponent :** 10000010 = 130, represents 3.

**Fraction :** 

$\displaystyle{}1010100110...=(\frac12)^{1}+(\frac12)^{3}+(\frac12)^{5}+(\frac12)^{8}+(\frac12)^{9}=0.662109375$

**Answer :** 
$$
(-1)\times1.662109375\times2^3=-13.296875
$$

## Problem. 8

$$
\begin{array}{r}
\text{x90A} \\
+\quad \text{x4123} \\
\hline \text{x4A2D}
\end{array}
$$

## Problem. 9

**a.** xABCD **OR** x9876 = xBBFF

**b.** x1234 **XOR** x1234 = x0000

**c.** xFEED **AND** (**NOT**(xBEEF)) = x4000

## Problem. 10

|  X   |  Y   |  Z   | Q~1~ | Q~2~ |
| :--: | :--: | :--: | :--: | :--: |
|  0   |  0   |  0   |  0   |  1   |
|  0   |  0   |  1   |  0   |  1   |
|  0   |  1   |  0   |  0   |  1   |
|  0   |  1   |  1   |  0   |  1   |
|  1   |  0   |  0   |  1   |  1   |
|  1   |  0   |  1   |  1   |  1   |
|  1   |  1   |  0   |  1   |  1   |
|  1   |  1   |  1   |  0   |  0   |

## Problem. 11

**a.** (25675)~D~ = (644B)~H~

**b.** $(675.625)_D = (1010100011.101)_B = 1.010100011101 × 2^9$

​    **IEEE 754 format :** 0  10001000  01010001110100000000000

​    **Hexadecimal representation :** 4428E800

**c.** (Hello)~ASCII~ = (48 65 6C 6C 6F)~H~