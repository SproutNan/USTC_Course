# 数理逻辑作业 Week9

PB20111686 黄瑞轩

## P84 T2

对项 $t$ 在项集 $T$ 中层次 $n$ 做归纳。

（1）当 $n=0$ 时，$t\in X\cup C$，故 $\varphi(x)=\psi(x)\Rightarrow \varphi(t)=\psi(t)$；

（2）设当 $n<k$ 时命题成立；

（3）当 $n=k$ 时，设 $t=f_{i}^{k}\left(t_{1}, \ldots, t_{k}\right)$，这里 $t \in T_{k}, t_{1}, \ldots, t_{k} \in \bigcup_{i=0}^{k-1} T_{i}$，故
$$
\varphi(t)=\varphi\left(f_{i}^{k}\left(t_{1}, \ldots, t_{k}\right)\right)=\overline{f_{i}^{n}}\left(\varphi\left(t_{1}\right), \ldots, \varphi\left(t_{k}\right)\right)=\overline{f_{i}^{n}}\left(\psi\left(t_{1}\right), \ldots, \psi\left(t_{k}\right)\right)=\psi(t)
$$
故结论成立。

## P84 T3

用归纳法。

（1）对 $\forall\tau(x)\in T_0$，若 $\tau(x)=x$，则 $\varphi^{\prime}(\tau(x))=\varphi^{\prime}(x)=\varphi(t)=\varphi(\tau(t))$；若 $\tau(x)\neq x$，则 $\tau(x)=\tau(t), \varphi^{\prime}(\tau(x))=\varphi(\tau(x))=\varphi(\tau(t))$。

（2）假定对 $\forall\tau(x)\in T_n(n<k)$，结论都成立。

（3）对 $\tau(x)\in T_k$，设 $\tau(x)=f_{i}^{n}\left(\tau_{1}(x), \cdots, \tau_{n}(x)\right)$，这里 $\tau_m(x)\in\bigcup_{j=0}^kT_j,1\le m\le n$，且 $\tau(t)=f_i^n(\tau_1(t),...,\tau_n(t))$，则
$$
\begin{aligned}
\varphi^{\prime}(\tau(x)) &=\overline{f_{i}^{n}}\left(\varphi^{\prime}\left(\tau_{1}(x)\right), \cdots, \varphi^{\prime}\left(\tau_{n}(x)\right)\right)=\overline{f_{i}^{n}}\left(\varphi\left(\tau_{1}(t)\right), \cdots, \varphi\left(\tau_{n}(t)\right)\right) \\
&=\varphi\left(f_{i}^{n}\left(\tau_{1}(t), \cdots, \tau_{n}(t)\right)\right)=\varphi(\tau(t))
\end{aligned}
$$
取 $\tau(x)=u(x)$，原命题即得证。

## P87 T1

### 3° $\neg R_{1}^{2}\left(f_{2}^{2}\left(x_{1}, x_{2}\right), f_{2}^{2}\left(x_{2}, x_{3}\right)\right)$

令 $q=R_{1}^{2}\left(f_{2}^{2}\left(x_{1}, x_{2}\right), f_{2}^{2}\left(x_{2}, x_{3}\right)\right)$，$q$ 的语义解释为 $$x_{1} \times x_{2}=x_{2} \times x_{3}$$，要找 $|p|(\varphi)=1,|p|(\psi)=0$ 只需找 $|q|(\varphi)=0,|q|(\psi)=1$，因此可取
$$
\begin{gathered}
\varphi: \varphi\left(x_{1}\right)=1, \varphi\left(x_{2}\right)=2, \varphi\left(x_{3}\right)=3 \\
\psi: \psi\left(x_{1}\right)=1, \psi\left(x_{2}\right)=1, \psi\left(x_{3}\right)=1
\end{gathered}
$$

### 4° $\forall x_{1} R_{1}^{2}\left(f_{2}^{2}\left(x_{1}, x_{2}\right), x_{3}\right)$

令 $q=R_{1}^{2}\left(f_{2}^{2}\left(x_{1}, x_{2}\right), x_{3}\right)$，$q$ 的语义解释为 $x_{1} \times x_{2}=x_{3}$，若要对 $\varphi$ 任意的 $x$ 变通都有 $|q|(\varphi')=1$，可以取
$$
\varphi: \varphi\left(x_{2}\right)=\varphi\left(x_{3}\right)=0
$$
反之，可以取
$$
\psi: \psi\left(x_{2}\right)=1, \psi\left(x_{3}\right)=4
$$
涉及 $x_1$ 的指派是无关紧要的。

### 5° $\forall x_{1} R_{1}^{2}\left(f_{2}^{2}\left(x_{1}, c_{1}\right), x_{1}\right) \rightarrow R_{1}^{2}\left(x_{1}, x_{2}\right)$

令 $q=\forall x_1R_{1}^{2}\left(f_{2}^{2}\left(x_{1}, c_{1}\right), x_{1}\right),q_0=R_{1}^{2}\left(f_{2}^{2}\left(x_{1}, c_{1}\right), x_{1}\right),r=R_1^2(x_1,x_2)$，欲使 $|p|(\varphi)=1$ 只需 $|q|(\varphi)→|r|(\varphi)=1$。

$q_0$ 的语义解释为 $0=x_1$，显然不能对任何的 $\varphi$ 的 $x_1$ 变通都有 $|q_0|(\varphi')=1$ 成立，所以对于任何 $\varphi$，$|q|(\varphi)=0$，因此可以取
$$
\varphi:\varphi(x_2)=1
$$
涉及 $x_1$ 的指派是无关紧要的，并且没有符合条件的 $\psi$。

## P87 T2

### 3° 

设原公式为 $p=\neg q$，则 $q$ 的语义解释为 $x_{1}<\left(x_{1}-\left(x_{1}-x_{2}\right)\right) \equiv x_{1}<x_{2}$，要找 $|p|(\varphi)=1,|p|(\psi)=0$ 只需找 $|q|(\varphi)=0,|q|(\psi)=1$，因此可取
$$
\begin{aligned}
&\varphi: \varphi\left(x_{1}\right)=2, \varphi\left(x_{2}\right)=1 \\
&\psi: \psi\left(x_{1}\right)=1, \psi\left(x_{2}\right)=2
\end{aligned}
$$

### 4°

设原公式为 $p=\forall x_1 q$，则 $q$ 的语义解释为 $x_1-x_2<x_3$，显然不能对任何的 $\varphi$ 的 $x_1$ 变通都有 $|q|(\varphi')=1$ 成立，所以对于任何 $\varphi$，$|p|(\varphi)=0$，因此可以取
$$
\psi:\psi(x_2)=1,\psi(x_3)=2
$$
涉及 $x_1$ 的指派是无关紧要的，并且没有符合条件的 $\varphi$。

### 5°

设原公式为 $p=q→r=\forall x_1q_0→r$，欲使 $|p|(\varphi)=1$ 只需 $|q|(\varphi)→|r|(\varphi)=1$。

$q_0$ 的语义解释为 $x_1<x_1$，显然不能对任何的 $\varphi$ 的 $x_1$ 变通都有 $|q_0|(\varphi')=1$ 成立，所以对于任何 $\varphi$，$|q|(\varphi)=0$，因此可以取
$$
\varphi:\varphi(x_2)=1
$$
涉及 $x_1$ 的指派是无关紧要的，并且没有符合条件的 $\psi$。

## P91 T2

### 3°

设原公式为 $p=\forall x_1\forall x_2\forall x_3q$，$q$ 的语义解释为 $$
\left(x_{1}<x_{2}\right) \rightarrow\left(x_{1}-x_{3}<x_{2}-x_{3}\right)
$$。

对于任意的 $\phi$ 的 $x_3$ 变通，都有 $|q|(\phi)=1$，令 $r=\forall x_3q$；

对于任意的 $\phi$ 的 $x_2$ 变通，都有 $|r|(\phi)=1$，令 $s=\forall x_2r$；

对于任意的 $\phi$ 的 $x_1$ 变通，都有 $|s|(\phi)=1$，所以 $|p|_\Z=1$。

### 4°

设原公式为 $p=\forall x_1\exist x_2q$，$q$ 的语义解释为 $0<2x_2$。

存在 $\phi$ 的 $x_3$ 变通 $\phi'$，使得 $|q|(\phi')=1$，令 $r=\exist x_2q$，则 $|r|(\phi)=1$；

对于任意的 $\phi$ 的 $x_1$ 变通，都有 $|r|(\phi)=1$，所以 $|p|_\Z=1$。