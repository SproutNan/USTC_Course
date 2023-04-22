# 数理逻辑作业 Week10

PB20111686 黄瑞轩

## P93 T3

### 3° $\forall x_1(\neg R_1^1(x_1)→\neg R_1^1(c_1))$

只需证明 $\not \vDash p$，为此只需找一个解释域 $M$ 与解释 $\phi\in\Phi_M$，使得 $|p|(\phi)=0$。

取 $M:\N=\{0,1,2,...\},\overline{c_1}=0,\overline{R_1^1}:$ 是 $0$，取 $\phi:\phi(x_1)=1$。

设 $p=\forall x_1 q$，则 $|q|(\phi)=0$，由于 $\phi$ 也是 $\phi$ 的变通，所以 $|p|(\phi)=0$。

所以 $p$ 不是有效式。

### 4° $\forall x_1R_1^2(x_1,x_1)→\exist x_2\forall x_1R_1^2(x_1,x_2)$

设 $p=r→s,r=\forall x_1 r_0$。

设 $s=\neg s_0,s_0=\forall x_2 (\neg\forall x_1R_1^2(x_1,x_2)),s_0=\forall x_2(\neg s_1),s_1=\forall x_1R_1^2(x_1,x_2)$。

取 $M:\N,\overline{R_1^2}:$ 相等，取 $\phi:\phi(x_1)=1,\phi(x_2)=2$。

对于 $\phi$ 的任意 $x_1$ 变通 $\phi'$，都有 $|r_0|(\phi')=1$，因此 $|r|(\phi)=1$。

因为存在 $\phi$ 的 $x_1$ 变通使得 $|R_1^2(x_1,x_2)|(\phi'')=0$，所以 $|s_1|(\phi)=0$，所以 $|\neg s_1|(\phi)=1$。

因为对于 $\phi$ 的任何 $x_2$ 变通 $\phi'$，都有 $|\neg s_1|(\phi')=1$，所以 $|s_0|(\phi)=1$，所以 $|s|(\phi)=0$。

所以 $|p|(\phi)=|r|(\phi)→|s|(\phi)=0$，不是有效式。

## P94 T4

### （a）

对项 $t$ 在 $K$ 的项集 $T$ 中层次 $n$ 做归纳。

（1）当 $n=0$ 时，$t\in X\cup C_K$，由于对任意 $x_i\in X$，$\phi^+(x_i)=\phi(x_i)$，故 $\phi(t)=\phi^+(t)$；

（2）设当 $n<k$ 时命题成立；

（3）当 $n=k$ 时，设 $t=f_{i}^{k}\left(t_{1}, \ldots, t_{k}\right)$，这里 $t \in T_{k}, t_{1}, \ldots, t_{k} \in \bigcup_{i=0}^{k-1} T_{i}$，故
$$
\phi(t)=\phi\left(f_{i}^{k}\left(t_{1}, \ldots, t_{k}\right)\right)=\overline{f_{i}^{n}}\left(\phi\left(t_{1}\right), \ldots, \phi\left(t_{k}\right)\right)=\overline{f_{i}^{n}}\left(\phi^+\left(t_{1}\right), \ldots, \phi^+\left(t_{k}\right)\right)=\phi^+(t)
$$
故结论成立。

### （b）

对公式 $p$ 在 $K$ 的公式集 $K(Y)$ 中的层次 $n$ 做归纳。

（1）当 $n=0$ 时，$p(x)$ 是原子公式，设 $p(x)=R_i^m(t_1(x),...,t_m(x))$，于是
$$
\begin{aligned}
|R_i^m(t_1(x),...,t_m(x))|(\phi^+)=1\Leftrightarrow & (\phi^+(t_1(x)),...,\phi^+(t_m(x)))\in\overline{R_i^m}\\
\Leftrightarrow& (\phi(t_1(x)),...,\phi(t_m(x)))\in\overline{R_i^m}\\
\Leftrightarrow& |R_i^m(t_1(x),...,t_m(x))|(\phi)=1
\end{aligned}
$$
为假的情况同理，故 $|p|(\phi^+)=|p|(\phi)$。

（2）设当 $n<k$ 时命题成立；

（3）当 $n=k$ 时，分以下四种情况讨论。

- 若 $p(x)=\neg q(x)$，则 $|p|(\phi^+)=\neg|q|(\phi^+)=\neg|q|(\phi)=|p|(\phi)$；
- 若 $p(x)=q(x)→r(x)$，则 $|p|(\phi^+)=|q|(\phi^+)→|r|(\phi^+)=|q|(\phi)→|r|(\phi)=|p|(\phi)$；
- 若 $p(x)=\forall y q(x)$，若 $\phi'$ 是 $\phi$ 的任意 $y$ 变通， $\phi^+{}'$ 是 $\phi'$ 在 $K^+$ 的扩张，则 $\phi^+{}'$ 是 $\phi^+$ 的 $y$ 变通，则

$$
\begin{aligned}
|p|(\phi^+)=1\Leftrightarrow & 对任意\phi^+的y变通\phi^+{}'，都有|p|(\phi^+{}')=1\\
\Leftrightarrow& 对任意\phi的y变通\phi{}'，都有|q|(\phi{}')=1\\
\Leftrightarrow& |p|(\phi)=1
\end{aligned}
$$

## P98 T2

反证法，如果 $\vdash \exist x_2R_1^2(x_1,x_2)→\exist x_2R_1^2(x_2,x_2)$ 成立，由可靠性定理，则 $\vDash \exist x_2R_1^2(x_1,x_2)→\exist x_2R_1^2(x_2,x_2)$ 成立。

给一个解释域 $M:\N,\overline{R_1^2}是<$，并且给 $\Phi_M$ 上的一个项解释 $\phi:\phi(x_1)=1,\phi(x_2)=2$，此时公式前件为真，后件为假，整个式子语义上是假的，故矛盾，所以原式不成立。