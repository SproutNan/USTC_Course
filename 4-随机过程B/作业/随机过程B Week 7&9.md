# 随机过程作业 Week7&9

黄瑞轩 PB20111686

## Ch3 T20

(a)

设 $X_n$ 为第 $n$ 分钟红细胞数，状态空间 $S=\{0,1,...\}$

因为 $P(X_n|X_{n-1},X_{n-2},...,X_0)=P(X_n|X_{n-1})$，所以 $\{X_n\}$ 是M.C.

$n+1$ 分钟没有白细胞，说明每一代个体都新生2个红细胞，
$$
P=\prod_{i=0}^{n+1}\left(\frac{1}{4}\right)^{2^{i}}=\left(\frac{1}{4}\right) \sum_{i=0}^{n+1} 2^{i}=\left(\frac{1}{4}\right)^{\frac{1-2^{n+1}}{1-2}}=\left(\frac{1}{4}\right)^{2^{n+1}-1}
$$
(b)

记 $X_n$ 为第 $n$ 代后裔的大小，$X_1=0,1,2.$
$$
\phi(s)=\frac{1}{4}(s+1)^{2}, \pi=\inf \{s \mid \phi(s)=s\}=1
$$

## Ch3 T21

记 $X_n$ 为第 $n$ 代后裔的大小，$Z_i$ 为第 $n$ 代第 $i$ 个个体后代的个数。

显然 $Z_i\sim i.i.d.\{p_0=q,p_1=p\},X_{n+1}=\sum_{i=1}^{X_n}Z_i$，故
$$
E X_{n+1}=E X_{n} E Z_{i}=E X_{n-1}\left(E Z_{i}\right)^{2}=\ldots=E X_{1}\left(E Z_{i}\right)^{n}=p^{n+1}
$$

$$
\operatorname{Var}\left(x_{n+1}\right)=p q \cdot p^{n} \frac{\left(1-p^{n+1}\right)}{1-p} I(p \neq 1)=p^{n+1}\left(1-p^{n+1}\right)
$$

$$
\phi(s)=q+p s, \pi=\inf \{s \mid \phi(s)=s\}
$$

当 $p<1$ 时，$\pi=1$；当 $p=1$ 时，$\pi=0$；

> 当 $p_{0}=\frac{1}{4}, p_{1}=\frac{1}{2}, p_{2}=\frac{1}{4} \text { 时, } E Z_{i}=1, E X_{n+1}=1$；
>
> 此时 $\mu=1, \sigma^{2}=E\left(Z_{i}-1\right)^{2}=0 \times \frac{1}{2}+1 \times \frac{1}{2}=\frac{1}{2}$
>
> 故 $\operatorname{Var}\left(X_{n+1}\right)=\frac{1}{2}(n+1), \phi(s)=\frac{1}{2}(1+s)^{2}$，第20题讨论了，此时 $\pi=1$。

> 当 $p_{0}=\frac{1}{8}, p_{1}=\frac{1}{2}, p_{2}=\frac{1}{4}, p_{3}=\frac{1}{8}, E Z_{i}=\frac{11}{8}$
>
> 此时 $\mu=\frac{11}{8}, \sigma^{2}=E\left(z_{i}-\frac{11}{8}\right)^{2}=\frac{47}{64}$
>
> 故 $E X_{n+1}=\left(\frac{11}{8}\right)^{n+1}, \quad \mathrm{Var}\left(x_{n+1}\right)=\frac{47}{64}\left(\frac{11}{8}\right)^{n} \frac{8}{3}\left[\left(\frac{11}{8}\right)^{n+1}-1\right]=\frac{47}{24}\left(\frac{11}{8}\right)^{n}\left[\left(\frac{11}{8}\right)^{n+1}-1\right]$
>
> $\phi(s)=\frac{1}{8}+\frac{1}{2} s+\frac{1}{4} s^{2}+\frac{1}{8} s^{3}, \quad \pi=\inf \{s \mid \phi(s)=s\}=\frac{\sqrt{13}-3}{2}$。

## Ch3 T22

$$
\begin{aligned}
&\left\{X_{3} \mid X_{0}=1\right\}=\{0,1\} \\
&P\left(X_{3}=1 \mid X_{0}=1\right)=P\left(X_{3}=1, X_{2}=1, X_{1}=1 \mid X_{0}=1\right)=p^{3} \\
&P\left(X_{3}=0 \mid X_{0}=1\right)=1-p^{3}
\end{aligned}
$$

## Ch4 T1

(a)
$$
\begin{aligned}
&E X(t)=\int_{0}^{2 \pi} \sin u t \cdot \frac{1}{2 \pi} d u=\left.\frac{1}{2 x} \frac{1}{t}(-\cos u t)\right|_{0} ^{2 \pi}=0 \\
&R_{X}(t, s)=E(X(t)-0)(X(s)-0)=E X(t) X(s)
\end{aligned}
$$

- 当 $t\neq s$

$$
\begin{aligned}
R_X(t,s)&=E \sin Ut \sin Us\\&=\frac{1}{2} E[\cos (t-s) u-\cos (t+s) u]\\&=\frac{1}{4 \pi}\left[\left.\frac{1}{t-s} \sin (t-s) u\right|_{0} ^{2 \pi}-\left.\frac{1}{t+s} \sin (t+s) u\right|_{0} ^{2 \pi}\right]=0
\end{aligned}
$$

- 当 $t=s$

$$
E X^{2}(t)=E \sin ^{2} U t=\int_{0}^{2 \pi} \sin ^{2} u t d u=\frac{1}{2}
$$

故 $R_X(t,s)=f(t-s)$，故 $X(t)$ 宽平稳。

考虑 $(X(3),X(2))$ 的分布：$P(X(3)<x_3,X(2)<x_2)=P(\sin 3U<x_3,\sin2U < x_2)$；

再考虑 $(X(2),X(1))$ 的分布：$P(X(2)<x_2,X(1)<x_1)=P(\sin 2U<x_2,\sin U < x_1)$；

二者分布显然不相同，故 $X(t)$ 非严平稳。

(b)

$EX(t)=\frac1{2\pi t}(1-\cos2\pi t)$，不是常数，故非宽平稳。

$X(t+\frac\pi2)$ 的分布：$P(X(t+\frac\pi2)<x)=P(\sin(ut+\frac\pi2u)<x)$

$X(t)$ 的分布：$P(X(t)<x)=P(\sin ut<x)$

二者分布不相同，故 $X(t)$ 非严平稳。

## Ch4 T3

$$
\begin{aligned}
E X_{n} &=\sum_{k=1}^{N} \sigma_{k} \sqrt{2} E \cos \left(a_{k} n-U_{k}\right)=\sum_{k=1}^{N} \sigma_{k} \sqrt{2} \int_{0}^{2 \pi} \cos \left(a_{k} n-u_{k}\right) d u_{k} \\
&=\left.\sum_{k=1}^{N} \sigma_{k} \sqrt{2}\left[-\sin \left(a_{k} n-u_{k}\right)\right]\right|_{0} ^{2 \pi}=0
\end{aligned}
$$

$R_X(n,m)=EX_nX_m=2E(\sum_{k=1}^N\sigma_k\cos(a_kn-U_k))(\sum_{l=1}^N\sigma_l\cos(a_lm-U_l))$

对 $k\neq l$，$\cos(a_kn-U_k)\cos(a_lm-U_l)=\frac12[\cos(a_kn+a_lm-U_k-U_l)+\cos(a_kn-a_lm-U_k+U_l)]$
$$
E\cos(a_kn-U_k)\cos(a_lm-U_l)=\\\frac{1}{2}\frac{1}{4\pi^2}\int_{0}^{2\pi}\int_{0}^{2\pi}\frac12[\cos(a_kn+a_lm-u_k-u_l)+\cos(a_kn-a_lm-u_k+u_l)]du_kdu_l=0
$$
故 $E\left(\sum_{k=1}^{N} \sigma_{k} \cos \left(a_{k} n-U_{k}\right)\right)\left(\sum_{l=1}^{N} \sigma_{l} \cos \left(a_{l} m-U_{l}\right)\right)=E\sum_{k=1}^{N} \sigma_{k}^{2} \cos \left(a_{k} n-U_{k}\right) \cos \left(a_{k} m-U_{k}\right)$

则 $E \sum_{k=1}^{N} \sigma_{k}^{2} \cos \left(a_{k} n-U_{k}\right) \cos \left(a_{k} m-U_{k}\right)=\sum_{k=1}^{N} \sigma_{k}^{2} E_{2} \cos \left(a_{k} n-U_{k}\right) \cos \left(a_{k} m-U_{k}\right)$

$\begin{aligned}
&=\sum_{k=1}^{N} \sigma_{k}^{2} \cdot \frac{1}{2 \pi} \int_{0}^{2 \pi}\left(\cos \left(a_{k}(n+m)-2 u_{k}\right)+\cos \left(a_{k}(n-m)\right)\right) d u_{k} . \\
&=\sum_{k=1}^{N} \sigma_{k}^{2} \cos \left(a_{k}(n-m)\right)=f(n-m)
\end{aligned}$

所以 $R_X(n,m)=2f(n-m)$，故是宽平稳的。

## Ch4 T4

$E Z(t)=\sum_{k=1}^{n} E A_{k} \cdot e^{j \omega_{k} t}=$ 常数

因为 $\omega_j$ 是给定数，不能指派，所以只能所有 $EA_k=0$，此时 $EZ(t)=0$

$R_{Z}(t, s) =E Z(t) Z^{*}(s)=E\left(\sum_{k=1}^{n} A_{k} e^{j \omega_{k} t}\right)\left(\sum_{l=1}^{n} A_{l} e^{-j \omega_{l} s}\right) \\
=E \sum_{k=1}^{n} \sum_{l=1}^{n} A_{k} A_{l} e^{j \omega_{k} t-j \omega_{l} s}=f(t-s)$

因为 $\omega_j$ 是给定数，不能指派，所以只能所有 $EA_kA_l=0$，此时 $R_Z(t,s)=0$

## Ch4 T5

$$
\begin{aligned}
&E X_{n}=p+p-1=2 p-1 \quad E X_{n}^{2}=1\\
&E S_{n}=\frac{1}{\sqrt{n}} \cdot n(2 p-1)=\sqrt{n}(2 p-1) \text {. }\\
&R_{s}(n, m)=E\left(S_{n}-\sqrt{n}(2 p-1)\right)\left(S_{m}-\sqrt{m}(2 p-1)\right)=E S_{n} S_{m}-\sqrt{m n}(2 p-1)^{2}\\
&E S_{n} S_{m}=\frac{1}{\sqrt{m n}} E\left(\sum_{k=1}^{m} X_{k}\right)\left(\sum_{l=1}^{n} X_{l}\right)\\
&不妨设m\le n, 则\left(\sum_{k=1}^{m} X_{k}\right)\left(\sum_{l=1}^{n} X_{l}\right)=\sum_{k=1}^{m} X_{k}^{2}+\left[\left(\sum_{k=1}^{m} X_{k}\right)\left(\sum_{l=1}^{n} X_{l}\right)-\sum_{k=1}^{m} X_{k}^{2}\right]\\
&E S_{n} S_{m}=E\left\{\sum_{k=1}^{m} X_{k}^{2}+\left[\left(\sum_{k=1}^{m} X_{k}\right)\left(\sum_{l=1}^{n} X_{l}\right)-\sum_{k=1}^{m} X_{k}^{2}\right]\right\} \frac{1}{\sqrt{m n}}
=\left(m+(m n-m)(2 p-1)^{2}\right) \frac{1}{\sqrt{m n}}\\
&\text { 故 } R_{s}(n, m)=\sqrt{\frac{m}{n}}-\sqrt{\frac{m}{n}}(2 p-1)^{2}\\
&r_{s}(n, m)=E S_{n} S_{m}=\sqrt{\frac{m}{n}}+\left(\sqrt{m n}-\sqrt{\frac{m}{n}}\right)(2 p-1)^{2}
\end{aligned}
$$

若平稳，则 $ES_n=\sqrt{n}(2p-1)$ 为常数，则 $p=1/2$；

此时 $R_S(n,m)=\sqrt\frac mn\neq f(m-n)$，所以 $S_n$ 不可能平稳。