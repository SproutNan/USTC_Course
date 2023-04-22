# Homework 3

PB20111686    Ruixuan Huang

## Problem. 1

**(a)** The states number in total is
$$
N=99^2*4^2*99^2*4^2*50*51*2*900=1.129\times10^{17}
$$
The minimum number of bits we need to use to store the states is
$$
b=\lceil\log N\rceil=57
$$
**(b)** Let $b_i$ be the number of bits we need to build the circuit of element $i$, then
$$
\begin{align}
b_1&=\lceil\log99^2\rceil=14\\
b_2&=\lceil\log4^2\rceil=4\\
b_3&=\lceil\log99^2\rceil=14\\
b_4&=\lceil\log4^2\rceil=4\\
b_5&=\lceil\log(50*51)\rceil=12\\
b_6&=\lceil\log2\rceil=1\\
b_7&=\lceil\log900\rceil=10\\
\end{align}
$$
The total is
$$
b'=\sum b_i=59
$$
**(c)** Although part b has 2 more bits to use, it separates the functions of each bit, making the whole state clearer in practical application.

## Problem. 2

**(a)** The complete finite state mechine diagram is as follows. $H/0$ means the input is $H$ and the following output is $0$.

<img src="C:\Users\Sprout\AppData\Roaming\Typora\typora-user-images\image-20211102164403201.png" alt="image-20211102164403201" style="zoom:25%;" />

**(b)** 2, define $S=Q_1Q_0$.

## Problem. 3

Let $N$ be the number of bytes of memory the computer have. It should be the address space times the addressibility, using byte as unit.
$$
N=2^8\times8=2048
$$

## Problem. 4

**(a)** $A[1:0]=10,WE=0$.

**(b)** $N=\lceil\log60\rceil=6$. Because the number of bits of each location is unchanged, the addressability remains unchanged after the change was made.

**(c)** $n=2^N-60=4$.

## Problem. 5

**(a)** 2.

**(b)** 16 bit.

**(c)** $n=2^2*16/8=8\text{ bytes}$.

**(d)** The complement is as follows.

|  WE  | A[1:0] | Di[15:0] | D[15:0] | Read/Write |
| :--: | :----: | :------: | :-----: | :--------: |
|  0   |   01   |  xFADE   |  x4567  |    Read    |
|  1   |   10   |  xDEAD   |  xDEAD  |   Write    |
|  0   |   00   |  xBEEF   |  x0123  |    Read    |
|  1   |   11   |  xFEED   |  xFEED  |   Write    |

## Problem. 6

**(a)** $n_{op}=\lceil\log255\rceil=8$.

**(b)** $n_{dr}=\lceil\log120\rceil=7$.

**(c)** $n_{un}=32-8-3\times7=3$.

## Problem. 7

<img src="C:\Users\Sprout\AppData\Roaming\Typora\typora-user-images\image-20211102232350312.png" alt="image-20211102232350312" style="zoom: 50%;" />

## Problem. 8

<img src="C:\Users\Sprout\AppData\Roaming\Typora\typora-user-images\image-20211103155221197.png" alt="image-20211103155221197" style="zoom:50%;" />

**Function** : 3 bit's subtraction counter.

