# 随机过程B Week 1

黄瑞轩 PB20111686

## Ch1 T2

按期望定义和期望的线性性，
$$
E[X(t)]=E\left[\frac{1}{n} \sum_{k=1}^{n} I\left(t, U_{k}\right)\right]=E\left[I\left(t, U_{1}\right)\right]
$$
由于$U_1\sim \mathcal{U}(0,1)$，所以
$$
E\left[I\left(t, U_{1}\right)\right]=1 * P\left[I\left(t, U_{1}\right)=1\right]+0 * P\left[I\left(t, U_{1}\right)=0\right]=t
$$
于是在任意两个时刻$t_1,t_2(t_1,t_2>0)$，有
$$
E[X(t_1)]=t_1,E[X(t_2)]=t_2
$$
按协方差函数定义，
$$
\begin{aligned}
\operatorname{Cov}\left(X\left(t_{1}\right), X\left(t_{2}\right)\right) &=\frac{1}{n^{2}}\operatorname{Cov}\left(\sum_{k=1}^{n} I\left(t_{1}, U_{k}\right), \sum_{k=1}^{n}\left(t_{2}, U_{k}\right)\right)\\
&=\frac{1}{n^{2}} \sum_{k=1}^{n} \sum_{m=1}^{n} \operatorname{Cov}\left(I\left(t_{1}, U_{k}\right), I\left(t_{2}, U_{m}\right)\right) \\
&=\frac1n\operatorname{Cov}\left(I\left(t_{1}, U_{1}\right), I\left(t_{2}, U_{1}\right)\right) \\
&=\frac1n[E\left(I\left(t_{1}, U_{1}\right) I\left(t_{2}, U_{1}\right)\right)-t_{1} t_{2}] \\
&=\frac1n[P\left(I\left(t_{1}, U_{1}\right) I\left(t_{2}, U_{1}\right)=1\right)-t_{1} t_{2}] \\
&=\frac1n[P\left(u_{1} \leq t_{1}, u_{1} \leq t_{2}\right)-t_{1} t_{2} ]\\
&=\frac1n[\min \left\{t_{1}, t_{2}\right\}-t_{1} t_{2}]
\end{aligned}
$$

## Ch1 T4

按期望定义和条件，
$$
E[X(t)]=E[X(t)-X(0)]=\lambda t
$$
按协方差函数定义，在任意两个时刻$t_1,t_2(t_1,t_2>0)$，有
$$
\operatorname{Cov}\left(X\left(t_{1}\right), X\left(t_{2}\right)\right)=E\left(X\left(t_{1}\right) X\left(t_{2}\right)\right)-\lambda^{2} t_{1} t_{2}
$$
计算等号后的第一项，记$m=\min(t_1,t_2),M=\max(t_1,t_2)$，按条件$(\text{iii})$有
$$
\begin{aligned}
E(X(t_1)X(t_2))&=E\{[X(m)-X(0)][X(M)-X(m)+X(m)-X(0)]\}\\
&=\lambda m·\lambda(M-m)+E[X(m)-X(0)]^2\\
&=\lambda^2(t_1t_2-m^2)+\text{Var}[X(m)-X(0)]+\{E[X(m)-X(0)]\}^2\\
&=\lambda^2(t_1t_2-m^2)+\lambda m+\lambda^2m^2\\
&=\lambda^2t_1t_2+\lambda m
\end{aligned}
$$
所以，
$$
\operatorname{Cov}\left(X\left(t_{1}\right), X\left(t_{2}\right)\right)=\lambda\min(t_1,t_2)
$$
由于均值函数不是常数，此过程不是宽平稳的。

## Ch1 T5

按期望线性性，有
$$
\begin{aligned}
E(Y(t))&=E(X(t+1)-X(t)) \\
&=E(X(t+1)-X(0))-E(X(t)-X(0)) \\
&=\lambda
\end{aligned}
$$
计算协方差函数，在任意两个时刻$t_1,t_2(t_1,t_2>0)$，利用上题结论及$\displaystyle\min(x,y)=\frac{x+y-|x-y|}{2}$
$$
\begin{aligned}
\text{Cov}\left(Y\left(t_{1}\right), Y\left(t_{2}\right)\right)&=E\left(Y\left(t_{1}\right) Y\left(t_{2}\right)\right)-\lambda^{2}\\
&=E(X(t_1+1)X(t_2+1))-E(X(t_1+1)X(t_2))-E(X(t_1)X(t_2+1))+E(X(t_1)X(t_2))-\lambda^{2}\\
&=\lambda[2\min(t_1,t_2)-\min(t_1+1,t_2)-\min(t_1,t_2+1)]\\
&=\lambda\left(\frac{|t_1-t_2+1|+|t_1-t_2-1|}{2}-|t_1-t_2|\right)\\
&\sim f(t_1-t_2)
\end{aligned}
$$
由协方差函数知二阶矩存在，所以此过程是宽平稳的。

## Ch1 T9

由几何概型，
$$
P(X^2+Y^2\ge\frac34|X>Y)=\frac{P(X^2+Y^2\ge\frac34,X>Y)}{P(X>Y)}=\frac{\frac12(1-\frac34)\pi}{\frac12\pi}=\frac14
$$

## Ch1 T14

$$
\begin{aligned}
P\left(X_{1}+X_{2}=n\right) &=\sum_{k=0}^{\infty} P\left(X_{1}=k, X_{2}=n-k\right) \\
&=\sum_{k=0}^{\infty} P\left(X_{1}=k\right) P\left(X_{2}=n-k\right) \\
&=\sum_{k=0}^{\infty} \frac{\lambda_{1}^{k}}{k !} e^{-\lambda_{1}} \frac{\lambda_{2}^{n-k}}{(n-k) !} e^{-\lambda_{2}} \\
&=\frac{e^{-\lambda_{1}-\lambda_{2}}}{n !} \sum_{k=0}^{\infty} C_{n}^{k} \lambda_{1}^{k} \lambda_{2}^{n-k} \\
&=\frac{e^{-\left(\lambda_{1}+\lambda_{2}\right)}}{n !}\left(\lambda_{1}+\lambda _2\right)^{n}
\end{aligned}
$$

这说明
$$
X_1+X_2\sim\mathcal{P}(\lambda_1+\lambda_2)
$$
条件概率
$$
\begin{aligned}
P\left(X_{1}=k \mid X_{1}+X_{2}=n\right) &=\frac{P\left(X_{1}=k, X_{2}=n-k\right)}{P\left(X_{1}+X_{2}=n\right)} \\
&=\frac{\frac{\lambda_{1}^{k}}{k !} e^{-\lambda_1} \frac{\lambda_{2}^{n-k}}{(n-k) !} e^{-\lambda_2}}{\frac{\left(\lambda_{1}+\lambda_{2}\right)^{n}}{n !} e^{-\left(\lambda_1+\lambda_{2}\right)}} \\
&=C_{n}^{k} \frac{\lambda_{1}^{k} \lambda_{2}^{n-k}}{\left(\lambda_{1}+\lambda_{2}\right)^{n}}
\end{aligned}
$$
这里$k=0,1,...,n$。

## Ch1 T15

$X_1,X_2,...\text{ i.i.d.}$，则$\sum_{i=1}^nX_i\sim\Gamma(n,\lambda)$，即
$$
f_{Y|N}(y|n)=\frac{y^{n-1}\lambda^n e^{-\lambda y}}{(n-1)!},y\ge0
$$
故而
$$
\begin{aligned}
f_Y(y)&=\sum_{i=1}^nf(y|n)P(N=n)\\
&=\sum_{i=1}^n\frac{y^{n-1}\lambda^n e^{-\lambda y}}{(n-1)!}\beta(1-\beta)^n\\
&=\lambda\beta e^{-\lambda\beta y}
\end{aligned}
$$
即
$$
Y\sim\mathcal{E(\lambda\beta)}
$$

## Ch2 T2

$$
\begin{aligned}
E(N(s+t)N(t))&=E\{[N(t)-N(0)][N(s+t)-N(t)+N(t)-N(0)]\}\\
&=\lambda t·\lambda(s+t-t)+E[N(t)-N(0)]^2\\
&=\lambda^2st+\text{Var}[N(t)-N(0)]+\{E[N(t)-N(0)]\}^2\\
&=\lambda^2st+\lambda t+\lambda^2t^2
\end{aligned}
$$

## Ch2 T4

由Poisson过程定义条件(iii)可解。

（i）$N(1)=N(1)-N(0)\sim\mathcal{P}(2)$，则$P(N(1)\le2)=P(N(1)=0,1,2)=5/e^2$；

（ii）$N(1)=N(1)-N(0)\sim\mathcal{P}(2),N(2)=N(2)-N(1)\sim\mathcal{P}(2)$，则
$$
\begin{aligned}
P(N(1)=1,N(2)=3)&=P(N(1)-N(0)=1,N(2)-N(1)=2)\\
&=P(N(1)-N(0)=1)P(N(2)-N(1)=2)\\
&=\frac{2}{1}e^{-2}\frac{2^2}{2!}e^{-2}=\frac{4}{e^4}
\end{aligned}
$$
（iii）$P(N(1)\ge2|N(1)\ge1)=\displaystyle\frac{P(N(1)\ge2,N(1)\ge1)}{P(N(1)\ge1)}=\frac{1-P(N(1)=0,1)}{1-P(N(1)=0)}=\frac{e^2-3}{e^2-1}$