# 随机过程B Week 3

黄瑞轩 PB20111686

## Ch2 T9

$N_1(t)$是一个泊松过程，证明如下：

（i）$N_1(0)=0$；

（ii）因为$N(t)$具有独立增量，由题可知，$N_1(t)$也具有独立增量；

（iii）当$t>s>0,0<m<n$时，$N_1(t)-N_1(s)$的分布：
$$
\begin{aligned}
P(N_1(t)-N_1(s)=m)&=\sum_{n=m}^\infty P(N_1(t)-N_1(s)=m\mid N(t)-N(s)=n)P(N(t)-N(s)=n)\\
&=\sum_{n=m}^\infty C_m^np^m(1-p)^{n-m}\frac{\lambda^n(\Delta t)^n}{n!}e^{-\lambda(\Delta t)}\\
&=\frac{(p\lambda\Delta t)^m}{m!}e^{-\lambda\Delta t}\sum_{n=0}^\infty\frac{[(1-p)\lambda\Delta t]^n}{n!}\\
&=\frac{(p\lambda\Delta t)^m}{m!}e^{-p\lambda\Delta t}
\end{aligned}
$$
这说明$N_1(t)-N_1(s)\sim\mathcal{P}(p\lambda\Delta t)$。

综上，$N_1(t)$是强度为$p\lambda$的泊松过程。

若记$N_2(t)=N(t)-N_1(t)$，则与上面完全类似，只是概率$p$用$1-p$替代，则$N_2(t)$是强度为$(1-p)\lambda$的泊松过程。
$$
\begin{aligned}
P(N_1(t)=m,N_2(t)=n)&=P(N_1(t)=m,N(t)=m+n)\\
&=P(N_1(t)=m|N(t)=m+n)P(N(t)=m+n)\\
&=C_{m+n}^mp^m(1-p)^n\frac{(\lambda t)^{m+n}}{(m+n)!}e^{-\lambda t}\\
&=\frac{p^m(1-p)^n(\lambda t)^{m+n}}{m!n!}e^{-\lambda t}\\
&=P(N_1(t)=m)P(N_2(t)=n)
\end{aligned}
$$
所以$N_1(t)$和$N_2(t)$是独立的。

## Ch2 T11

按题意，有
$$
\{T>t\}=\left\{\sum_{k=1}^{N(t)}Y_k\le\alpha\right\}
$$
则
$$
\begin{aligned}
P\left(\sum_{k=1}^nY_k\le\alpha\right)&=\sum_{n=1}^\infty P\left(\sum_{k=1}^{N(t)}Y_k\le\alpha\mid N(t)=n\right)P(N(t)=n)\\
&=\sum_{n=1}^\infty P\left(\sum_{k=1}^{n}Y_k\le\alpha\right)P(N(t)=n)\\
&=\sum_{n=1}^\infty \int_0^\alpha\mu e^{-\mu y}\frac{(\mu y)^{n-1}}{(n-1)!}dy\frac{(\lambda t)^n}{n!}e^{-\lambda t}dt\\

\end{aligned}
$$
这里第二步使用独立性，第三步使用命题2.2结论。

则
$$
\begin{aligned}
ET&=\int_0^\infty P(T>t)dt\\
&=\int_0^\infty \sum_{n=1}^\infty \int_0^\alpha\mu e^{-\mu y}\frac{(\mu y)^{n-1}}{(n-1)!}dy\frac{(\lambda t)^n}{n!}e^{-\lambda t}dt\\
&=\sum_{n=1}^{\infty} \int_{0}^{+\infty} \int_{0}^{\alpha} \mu e^{-\mu y} \frac{(\mu y)^{n-1}}{(n-1) !} d y \frac{(\lambda t)^{n}}{n !} e^{-\lambda t} d t \\
&=\sum_{n=1}^{\infty} \frac{\mu^{n} \lambda^{n}}{n !(n-1) !} \int_{0}^{+\infty} \int_{0}^{\alpha} e^{-\mu y} y^{n-1} d y e^{-\lambda t} t^{n} d t \\
&=\sum_{n=1}^{\infty} \frac{(\mu \lambda)^{n}}{n !(n-1) !} \int_{0}^{\alpha} e^{-\mu y} y^{n-1} d y \int_{0}^{+\infty} e^{-\lambda t} t^{n} d t \\
&=\sum_{n=1}^{\infty} \frac{(\mu \lambda)^{n}}{n !(n-1) !} \int_{0}^{\alpha} e^{-\mu y} y^{n-1} d y \int_{0}^{+\infty} e^{-s} s^{n} \frac{1}{\lambda^{n+1}} d s \\
&=\sum_{n=1}^{\infty} \frac{\mu^{n}}{\lambda \cdot n !(n-1) !} \int_{0}^{\alpha} e^{-\mu y} y^{n-1} d y \cdot \Gamma(n+1) \\
&=\sum_{n=1}^{\infty} \frac{\mu^{n}}{\lambda(n-1) !} \int_{0}^{\alpha} e^{-\mu y} y^{n-1} d y \\
&=\int_{0}^{\alpha} \mu \frac{e^{-\mu y}}{\lambda} \sum_{n=1}^{\infty} \frac{\mu^{n-1} y^{n-1}}{(n-1) !} d y \\
&=\int_{0}^{\alpha} \frac{\mu e^{-\mu y}}{\lambda} e^{\mu y} d y=\frac{\mu \alpha}{\lambda}
\end{aligned}
$$
解释：强度$\lambda$越大，表示冲击来的越频繁，寿命越短；$\mu$越大，说明每次冲击的损害越小，寿命越长；$\alpha$越小，说明限度越小，寿命越小。

## Ch2 T12

结论：在一般情况下，$X_1,X_2,...$不独立、不同分布，分析如下。

任取指标$1\le s<r$，考察$X_s,X_r$独立性。设$s$发生的时间是$W_s$，由$W_s\le w_s\Leftrightarrow N(w_s)\ge s$和$W_s=\sum_{i=1}^sX_i$，先考察$W_s,W_r$的联合分布$F_{(W_s,W_r)}(w_s,w_r)$。

记$\displaystyle\Lambda(t)=\int_0^t\lambda(u)du$，则
$$
\begin{aligned}
P(W_s\le w_s,W_r\le w_r)&=P(N(w_s)\ge s,N(w_r)\ge r)\\
&=\sum_{k=s+1}^\infty\sum_{l=s}^k P(N(w_s)=l,N(w_r)=k),0\le w_s<w_r\\
&=\sum_{k=s+1}^\infty\sum_{l=s}^k P(N(w_s)=l,N(w_r)-N(w_s)=k-l)\\
&=\sum_{k=s+1}^\infty\sum_{l=s}^k \frac{\Lambda^l(w_s)(\Lambda(w_r)-\Lambda(w_s))^{k-l}}{l!(k-l)!}\exp(-\Lambda(w_r))
\end{aligned}
$$
很难看出这是$g_1(w_s)g_2(w_r)$的形式，为此，取$s=1,r=2$，则
$$
\begin{aligned}
P(W_1\le w_1,W_2\le w_2)&=\sum_{k=2}^\infty\sum_{l=1}^k \frac{\Lambda^l(w_1)(\Lambda(w_2)-\Lambda(w_1))^{k-l}}{l!(k-l)!}\exp(-\Lambda(w_2))\\
&=\exp(-\Lambda(w_2))\sum_{k=2}^\infty\frac{\Lambda^k(w_2)-(\Lambda(w_2)-\Lambda(w_1))^k}{k!}\\
&=\exp(-\Lambda(w_2))\left(\sum_{k=0}^\infty\frac{\Lambda^k(w_2)-(\Lambda(w_2)-\Lambda(w_1))^k}{k!}-\Lambda(w_1)\right)\\
&=\exp(-\Lambda(w_2))(\exp(\Lambda(w_2))-\exp(\Lambda(w_2)-\Lambda(w_1))-\Lambda(w_1))\\
&=1-\exp(-\Lambda(w_1))-\Lambda(w_1)\exp(-\Lambda(w_2))
\end{aligned}
$$
即
$$
F_{(W_1,W_2)}(w_1,w_2)=1-\exp(-\Lambda(w_1))-\Lambda(w_1)\exp(-\Lambda(w_2))
$$
故联合密度
$$
\begin{aligned}
f_{\left(W_{1}, W_{2}\right)}\left(w_{1}, w_{2}\right) &=\frac{\partial^{2} F_{\left(W_{1}, W_{2}\right)}\left(w_{1}, w_{2}\right)}{\partial w_{1} \partial w_{2}} =\lambda\left(w_{1}\right) \lambda\left(w_{2}\right)\exp(-\Lambda(w_2))
\end{aligned}
$$
由于$W_{1}=X_{1}, W_{2}=X_{1}+X_{2}$，从$\left(w_{1}, w_{2}\right)$变换到$\left(x_{1}, x_{2}\right)$的Jacobi行列式
$$
\frac{\partial(w_{1}, w_{2})}{\partial(x_{1}, x_{2})}=1
$$
故联合密度
$$
f_{(X_{1}, X_{2})}(x_{1}, x_{2})=\lambda(x_{1}) \lambda(x_{1}+x_{2})\exp(-\Lambda(x_1+x_2)), x_{1}, x_{2}>0
$$
一般情况下不能将上式拆分为$h_1(x_1)h_2(x_2)$的形式，所以$X_1,X_2$不独立。

密度函数
$$
f_{X_1}(x_1)=\int_0^\infty\lambda(x_1)\lambda(x_1+x_2)\exp(-\Lambda(x_1+x_2))dx_2=\lambda(x_1)[\exp(-\Lambda(x_1))-\exp(-\Lambda(\infty))]
$$

$$
\int_0^\infty f_{X_1}(x_1)dx_1=1=1-(\Lambda(\infty)+1)\exp(-\Lambda(\infty))
$$

则$\exp(-\Lambda(\infty))=0$，则$f_{X_1}(x_1)=\lambda(x_1)\displaystyle\exp\left(-\int_0^{x_1}\lambda(u)du\right),x_1>0$，$f_{X_2}(x_2)=\displaystyle\int_0^\infty\lambda(x_{1}) \lambda(x_{1}+x_{2})\exp(-\Lambda(x_1+x_2))dx_1,x_2>0$。

所以$X_1,X_2$不同分布，结论得证，并顺便求出了这两个随机变量的分布（密度函数）。

## Ch2 T13

因为$\lambda(t)>0$，则$m(t)$是严格的增函数，且$m(0)=0$，故$l(0)=0$。

（i）$N_1(0)=N(l(0))=N(0)=0$；

（ii）任取指标$0\le a<b\le c<d$，因为$m(t)$是严格增函数，所以$l(t)$也是严格增函数，所以
$$
0\le l(a)<l(b)\le l(c)<l(d)
$$
又因为
$$
\begin{aligned}
&N_{1}(b)-N_{1}(a)=N(l(b))-N(l(a)) \\
&N_{1}(d)-N_{1}(c)=N(l(d))-N(l(c))
\end{aligned}
$$
且$N(t)$是独立增量过程，所以$N_{1}(b)-N_{1}(a)$与$N_{1}(d)-N_{1}(c)$独立，所以$N_1(t)$是独立增量过程。

（iii）对$\forall t>0,s\ge 0$，计算
$$
\begin{aligned}
P(N_1(s+t)-N_1(s)=k)&=P(N(l(s+t))-N(l(s))=k)\\
&=\frac{\left(m(l(s+t))-m(l(s))\right)^k}{k!}\exp(-(m(l(s+t))-m(l(s))))\\
&=\frac{t^k}{k!}\exp(-t)
\end{aligned}
$$
所以$N_1(t)$是强度为1的泊松过程。

## Ch3 T2

$$
\begin{aligned}
&P\left\{X_{0}=0, X_{1}=1, X_{2}=2\right\} \\
&=P\left\{x_{2}=2 \mid X_{1}=1, X_{0}=0\right\} P\left\{x_{1}=1, X_{0}=0\right\} \\
&=P\left\{x_{2}=2 \mid X_{1}=1\right\} P\left\{x_{1}=1 \mid X_{0}=0\right\} P\left\{X_{0}=0\right\}\\
&=0×0.2×0.3=0
\end{aligned}
$$

## Ch3 T4

记A罐中球数为$k,0\le k\le N$。一次实验有四种情况：B→A，A→A，A→B，B→B，故$p_{ij}=P(X_{n+1}=j|X_n=i)$中$|j-i|\le 1$。

取值的三种情况：

（1）当$i=0$，$j$只能取$0,1$；

（2）当$i=N$，$j$只能取$N,N-1$；

（3）当$0<i<N$，$j$可取$i-1,i,i+1$。

对于（1）（2）两种情况：
$$
\begin{aligned}
&p_{00}=P\left(X_{n+1}=0 \mid X_{n}=0\right)=q \\
&p_{01}=P\left(X_{n+1}=1\mid X_{n}=0\right)=p \\
&p_{N N}=P\left(X_{n+1}=N \mid X_{n}=N\right)=p \\
&p_{N(N-1)}=P\left(X_{n+1}=N-1 \mid X_{n}=N\right)=q
\end{aligned}
$$
对于（3）情况：
$$
\begin{aligned}
&p_{i(i-1)}=P\left(X_{n+1}=i-1 \mid X_{n}=i\right)=\frac{i}{N}·q \\
&p_{i i}=P\left(X_{n+1}=i \mid X_{n}=i\right)=\frac{i}{N} \cdot p+\frac{N-i}{N} \cdot q \\
&p_{i(i+1)}=P\left(X_{n+1}=i+1 \mid X_{n}=i\right)=\frac{N-i}{N} p
\end{aligned}
$$
除了上面定义过的$p_{ij}$，其余元素均为0。所要求的矩阵就是$N+1$阶方阵$P_{ij}=(p_{ij})$。









































































