# 数理逻辑作业（Week 1&2）

#### 黄瑞轩 PB20111686

## Week 1

### P13 T7

$$
\begin{array}{  c  c  c  c | c | c  c  c  c  }
 (\neg & p & \wedge & q) & \rightarrow & (\neg & q & \wedge & r) \\
\hline 1 & 0 & 0 & 0 & 1 & 1 & 0 & 0 & 0 \\
 1 & 0 & 0 & 0 & 1 & 1 & 0 & 1 & 1 \\
 1 & 0 & 1 & 1 & 0 & 0 & 1 & 0 & 0 \\
 1 & 0 & 1 & 1 & 0 & 0 & 1 & 0 & 1 \\
 0 & 1 & 0 & 0 & 1 & 1 & 0 & 0 & 0 \\
 0 & 1 & 0 & 0 & 1 & 1 & 0 & 1 & 1 \\
 0 & 1 & 0 & 1 & 1 & 0 & 1 & 0 & 0 \\
 0 & 1 & 0 & 1 & 1 & 0 & 1 & 0 & 1 \\

\end{array}
$$

### P13 T8

$$
\begin{array}{ccc|c|ccc}
(p & \rightarrow & q) & \rightarrow & (p & \rightarrow & r) \\
\hline 0 & 1 & 0 & 1 & 0 & 1 & 0 \\
0 & 1 & 0 & 1 & 0 & 1 & 1 \\
0 & 1 & 1 & 1 & 0 & 1 & 0 \\
0 & 1 & 1 & 1 & 0 & 1 & 1 \\
1 & 0 & 0 & 1 & 1 & 0 & 0 \\
1 & 0 & 0 & 1 & 1 & 1 & 1 \\
1 & 1 & 1 & 0 & 1 & 0 & 0 \\
1 & 1 & 1 & 1 & 1 & 1 & 1
\end{array}
$$

### P13 T9

$$
\begin{array}{cccccc|c|cccccc}
\neg & (p & \vee & (q & \wedge & r) & \leftrightarrow & ((p & \vee & q) & \wedge & (p & \vee & r)) \\
\hline 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 \\
1 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 1 \\
1 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 1 & 1 & 0 & 0 & 0 & 0 \\
0 & 0 & 1 & 1 & 1 & 1 & 0 & 0 & 1 & 1 & 1 & 0 & 1 & 1 \\
0 & 1 & 1 & 0 & 0 & 0 & 0 & 1 & 1 & 0 & 1 & 1 & 1 & 0 \\
0 & 1 & 1 & 0 & 0 & 1 & 0 & 1 & 1 & 0 & 1 & 1 & 1 & 1 \\
0 & 1 & 1 & 1 & 0 & 0 & 0 & 1 & 1 & 1 & 1 & 1 & 1 & 0 \\
0 & 1 & 1 & 1 & 1 & 1 & 0 & 1 & 1 & 1 & 1 & 1 & 1 & 1
\end{array}
$$

















### P15 T2

$$
\begin{aligned}
L_{0}&=X_{2}=\left\{x_{1}, x_{2}\right\} \\
L_{1}&=\left\{\neg x_{1}, \neg x_{2}, x_{1} \rightarrow x_{1}, x_{1} \rightarrow x_{2}, x_{2} \rightarrow x_{1}, x_{2} \rightarrow x_{2}\right\} \\
L_{2}&=\left\{\neg\left(\neg x_{1}\right), \neg\left(\neg x_{2}\right), \neg\left(x_{1} \rightarrow x_{1}\right), \neg\left(x_{1} \rightarrow x_{2}\right), \neg\left(x_{2} \rightarrow x_{1}\right), \neg\left(x_{2} \rightarrow x_{2}\right),\right. \\
&x_{1} \rightarrow\left(\neg x_{1}\right), x_{1} \rightarrow\left(\neg x_{2}\right), x_{1} \rightarrow\left(x_{1} \rightarrow x_{1}\right), x_{1} \rightarrow\left(x_{1} \rightarrow x_{2}\right), x_{1} \rightarrow\left(x_{2} \rightarrow x_{1}\right), x_{1} \rightarrow\left(x_{2} \rightarrow x_{2}\right), \\
&x_{2} \rightarrow\left(\neg x_{1}\right), x_{2} \rightarrow\left(\neg x_{2}\right), x_{2} \rightarrow\left(x_{1} \rightarrow x_{1}\right), x_{2} \rightarrow\left(x_{1} \rightarrow x_{2}\right), x_{2} \rightarrow\left(x_{2} \rightarrow x_{1}\right), x_{2} \rightarrow\left(x_{2} \rightarrow x_{2}\right), \\
&\left(\neg x_{1}\right) \rightarrow x_{1},\left(\neg x_{2}\right) \rightarrow x_{1},\left(x_{1} \rightarrow x_{1}\right) \rightarrow x_{1},\left(x_{1} \rightarrow x_{2}\right) \rightarrow x_{1},\left(x_{2} \rightarrow x_{1}\right) \rightarrow x_{1},\left(x_{2} \rightarrow x_{2}\right) \rightarrow x_{1}, \\
&\left.\left(\neg x_{1}\right) \rightarrow x_{2},\left(\neg x_{2}\right) \rightarrow x_{2},\left(x_{1} \rightarrow x_{1}\right) \rightarrow x_{2},\left(x_{1} \rightarrow x_{2}\right) \rightarrow x_{2},\left(x_{2} \rightarrow x_{1}\right) \rightarrow x_{2},\left(x_{2} \rightarrow x_{2}\right) \rightarrow x_{2}\right\}
\end{aligned}
$$

## Week 2

注：以下某条语句用到$MP/HS$未注明的，默认是由其上的两条语句得到。

### P22 T2 1°

(1) $\left(\neg x_{1} \rightarrow \neg x_{2}\right) \rightarrow\left(x_{2} \rightarrow x_{1}\right)(L 3)$

(2) $\left(\left(\neg x_{1} \rightarrow \neg x_{2}\right) \rightarrow\left(x_{2} \rightarrow x_{1}\right)\right) \rightarrow\left(\left(x_{1} \rightarrow x_{2}\right) \rightarrow\left(\left(\neg x_{1} \rightarrow \neg x_{2}\right) \rightarrow\left(x_{2} \rightarrow x_{1}\right)\right)\right)(L 1)$

(3) $\left(x_{1} \rightarrow x_{2}\right) \rightarrow\left(\left(\neg x_{1} \rightarrow \neg x_{2}\right) \rightarrow\left(x_{2} \rightarrow x_{1}\right)\right)(M P)$

### P22 T2 2°

(1) $\left(x_{1} \rightarrow\left(x_{2} \rightarrow x_{3}\right)\right) \rightarrow\left(\left(x_{1} \rightarrow x_{2}\right) \rightarrow\left(x_{1} \rightarrow x_{3}\right)\right)(L 2)$

(2) $\left(\left(x_{1} \rightarrow\left(x_{2} \rightarrow x_{3}\right)\right) \rightarrow\left(\left(x_{1} \rightarrow x_{2}\right) \rightarrow\left(x_{1} \rightarrow x_{3}\right)\right)\right)$
      $\rightarrow\left(\left(x_{1} \rightarrow\left(x_{2} \rightarrow x_{3}\right)\right) \rightarrow\left(x_{1} \rightarrow x_{2}\right)\right) \rightarrow\left(\left(x_{1} \rightarrow\left(x_{2} \rightarrow x_{3}\right)\right) \rightarrow\left(x_{1} \rightarrow x_{3}\right)\right)(L 2)$

(3) $\left(\left(x_{1} \rightarrow\left(x_{2} \rightarrow x_{3}\right)\right) \rightarrow\left(x_{1} \rightarrow x_{2}\right)\right) \rightarrow\left(\left(x_{1} \rightarrow\left(x_{2} \rightarrow x_{3}\right)\right) \rightarrow\left(x_{1} \rightarrow x_{3}\right)\right)(M P)$

### P22 T3 2°

(1) $\neg \neg p \rightarrow(\neg \neg \neg \neg p \rightarrow \neg \neg p)(L 1)$

(2) $\neg \neg p$ (Assumption)

(3) $\neg \neg \neg \neg p \rightarrow \neg \neg p(M P)$

(4) $(\neg\neg\neg\neg p \rightarrow \neg \neg p$ ) $\rightarrow(\neg p \rightarrow \neg \neg \neg p)(L 3)$

(5) $\neg p \rightarrow \neg \neg \neg p(M P)$

(6) $(\neg p \rightarrow \neg \neg \neg p) \rightarrow(\neg \neg p \rightarrow p)(L 3)$

(7) $\neg \neg p \rightarrow p(M P)$

(8) $p(1,7, M P)$

### P22 T3 3°

(1) $(\neg(q \rightarrow r) \rightarrow \neg p) \rightarrow(p \rightarrow(q \rightarrow r))(L 3)$

(2) $\neg(q \rightarrow r) \rightarrow \neg p$ (Assumption)

(3) $p \rightarrow(q \rightarrow r)(M P)$

(4) $(p \rightarrow(q \rightarrow r)) \rightarrow((p \rightarrow q) \rightarrow(p \rightarrow r))(L 2)$

(5) $(p \rightarrow q) \rightarrow(p \rightarrow r)(M P)$

(6) $p \rightarrow q$ (Assumption)

(7) $p \rightarrow r(M P)$

### P22 T3 4°

(1) $(p \rightarrow(q \rightarrow r)) \rightarrow((p \rightarrow q) \rightarrow(p \rightarrow r))(L 2)$

(2) $p \rightarrow(q \rightarrow r)$ (Assumption)

(3) $(p \rightarrow q) \rightarrow(p \rightarrow r)(M P)$

(4) $((p \rightarrow q) \rightarrow(p \rightarrow r)) \rightarrow(q \rightarrow((p \rightarrow q) \rightarrow(p \rightarrow r)))(L 1)$

(5) $q \rightarrow((p \rightarrow q) \rightarrow(p \rightarrow r))(M P)$

(6) $(q \rightarrow((p \rightarrow q) \rightarrow(p \rightarrow r))) \rightarrow((q \rightarrow(p \rightarrow q)) \rightarrow(q \rightarrow(p \rightarrow r)))(L 2)$

(7) $(q \rightarrow(p \rightarrow q)) \rightarrow(q \rightarrow(p \rightarrow r))(M P)$

(8) $q \rightarrow(p \rightarrow q)(L 1)$

(9) $q \rightarrow(p \rightarrow r)(M P)$

### P25 T2 2°

由P22 T3 2°，知$\{\neg\neg p\}\vdash p$，即$\neg\neg p\rightarrow p$，则$\neg\neg\neg p→\neg p$。

由演绎定理知，$\vdash (q→p)→(\neg p→\neg q)\Leftrightarrow \{q→p\}\vdash \neg p→\neg q$。

(1) $(\neg\neg\neg p→\neg p)→(p→\neg\neg p)(L3)$

(2) $\neg\neg\neg p→\neg p$（已证明）

(3) $p→\neg\neg p(MP)$

(4) $\neg\neg q\rightarrow q$（已证明）

(5) $q→p$（Assumption）

(6) $\neg\neg q→p(HS)$

(7) $\neg\neg q→\neg\neg p(3,6,HS)$

(8) $(\neg\neg q→\neg\neg p)→(\neg p→\neg q)(L3)$

(9) $\neg p→\neg q(MP)$

### P25 T2 3°

由演绎定理知，$\vdash ((p→q)→p)→p\Leftrightarrow \{(p→q)→p\}\vdash p$。

(1) $(p→q)→p$ (Assumption)

(2) $\neg p→(p→q)$ （否定前件律）

(3) $\neg p→p(HS)$

(4) $(\neg p→p)→p$ （否定肯定律）

(5) $p(MP)$

### P28 T1 2°

由演绎定理知，$\vdash (\neg p→q)→(\neg q→p)\Leftrightarrow \{\neg p→q\}\vdash \neg q→p$。

(1) $\neg p→q$ (Assumption)

(2) $(\neg p→q)→(\neg q→\neg\neg p)$（P25 T2 2°）

(3) $\neg q→\neg\neg p(MP)$

(4) $\neg\neg p→p$ （双重否定律）

(5) $\neg q→p(HS)$ 

### P28 T1 3°

由演绎定理知，$\vdash \neg(p→q)→\neg q\Leftrightarrow \{\neg(p→q)\}\vdash \neg q$。

按归谬律，把$q$作为新假定。

(1) $\neg(p→q)$ (Assumption)

(2) $q→(p→q)(L1)$

(3) $q$ (Assumption)

(4) $p→q(MP)$

由(1, 4)用归谬律即得$\{\neg(p→q)\}\vdash \neg q$。









