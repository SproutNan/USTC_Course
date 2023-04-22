# 随机过程B Week 10&11

黄瑞轩 PB20111686

## Ch4 T12

因为 $\int_{-\infty}^\infty R(\tau)d\tau<\infty$，则均值遍历性成立，则
$$
E\bar X=E\left(\frac1T\int_0^T X(s)ds\right)=\frac1T\int_0^TEX(s)ds=m
$$

$$
\begin{aligned}
\mathrm{Var}(\bar X)&=E(\bar X^2)-[E(\bar X)]^2\\
&= \frac{1}{4T^2}\iint_{[-T,T]\times[-T,T]}E[(X(t)-m)(X(s)-m)]dtds\\
&=\frac{1}{4T^2}\iint_{[-T,T]\times[-T,T]}R(t-s)dtds\\
&(令\tau=t-s,u=t+s)\\
&=\frac{1}{4T^2}\frac12\iint_DR(\tau)d\tau du\\
&=\frac{1}{4T^2}\int^{2T}_{2T}R(\tau)(2T-|\tau|)d\tau\\
&=\frac{1}{T}\int_{0}^{2T}R(\tau)(1-\frac\tau{2T})d\tau\\
&=\frac1T\int_{0}^{2T}ae^{-b\tau}(1-\tau/2T)d\tau\\
&=\frac{2a}{bT}[1-\frac{1}{bT}(1-e^{-bt})]
\end{aligned}
$$

## Ch4 T16

$EX_0=\int_0^12x^2dx=\frac23$，给定 $X_0$ 时，$EX_1=E[E(X_1|X_0)]=E(1-\frac{X_0}{2})=\frac23$；

不妨用归纳法，假设 $EX_n=2/3$，同上面的计算过程，可知 $EX_{n+1}=2/3$，所以对于任意 $n$，$EX_n$ 是常数。

$EX_0^2=\int_0^12x^3dx=1/2$，所以 $DX_0=EX_0^2-(EX_0)^2=1/18$。假设 $EX_n^2=1/2$，则 $EX_{n+1}^2=E[E(X_{n+1}^2|X_n)]=E[\int_{1-X_n}^1\frac1{X_n}x_{n+1}^2dx_{n+1}]=E[X_n^2/12+(2-X_n)^2/4]=1/2$，所以对于任意 $n$，$EX_n^2$ 是常数。

$R_X(n+t,n)=E(X_{n+t}-2/3)(X_n-2/3)=E(X_{n+t}X_n)-4/9$

$E(X_{n+t}X_n)=E[E(X_{n+t}X_n|X_{n+t-1},...,X_0)]=E[X_nE(X_{n+t}|X_{n+t-1},...,X_0)]$

$=E[X_n(1-\frac12X_{n+t-1})]=\frac23-\frac12E(X_{n+t-1}X_n)=...=\frac23\sum_{i=0}^{t-1}(-\frac12)^i+\frac12(-\frac12)^t=\frac49+\frac1{18}(-\frac12)^t$

所以 $R_X(n+t,n)=\frac 1{18}(-\frac12)^t=R(t)$。所以 $\{X_n\}$ 是平稳的。

又因为 $R(t)→0(t→\infty)$，知均值遍历性成立。

## Ch4 T17

$EX_n=\sum_{k=0}^\infty\alpha^kE\varepsilon_{n-k}=0$，

$R_X(n+t,n)=EX_{n+t}X_n=E(\sum_{k=0}^\infty\alpha^k\varepsilon_{{n+t}-k})(\sum_{l=0}^\infty\alpha^l\varepsilon_{n-l})=E(\sum_\infty\alpha^a\varepsilon_b\varepsilon_c)$

对于 $b\neq c$，由白噪声性质，有 $E\varepsilon_b\varepsilon_c=0$，所以

$R_X(n+t,n)=E(\sum_{k=0}^\infty\alpha^{2k+t}\varepsilon_{n-k}^2)=\frac{\sigma^2\alpha^t}{1-\alpha^2}=R(t)$，所以 $\{X_n\}$ 是平稳的。

又因为 $R(t)→0(t→\infty)$，知均值遍历性成立。