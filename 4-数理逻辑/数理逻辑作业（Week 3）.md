# 数理逻辑作业（Week 3）

黄瑞轩 PB20111686

## P30 2

### 命题2-2°

即证$\vdash \neg(p \rightarrow \neg q) \rightarrow q$。

(1) $\neg q \rightarrow(p \rightarrow \neg q)(L 1)$
(2) $(\neg q \rightarrow(p \rightarrow \neg q)) \rightarrow(\neg(p \rightarrow \neg q) \rightarrow \neg \neg q)$(换位率)
(3) $\neg(p \rightarrow \neg q) \rightarrow \neg \neg q(1,2,M P)$
(4) $\neg \neg q \rightarrow q$(双重否定律)
(5)$\neg(p \rightarrow \neg q) \rightarrow q(3,4,H S)$

### 命题2-3°

即证$\vdash \neg(p \rightarrow \neg q) \rightarrow \neg(q \rightarrow \neg p)$。

由演绎定理，即证$\{\neg(p \rightarrow \neg q)\} \vdash \neg(q \rightarrow \neg p)$。

如果把$q\rightarrow\neg p$作为新假定，得到如下证明：

(1) $\neg(p \rightarrow \neg q) \rightarrow q$(已证明)
(2) $q \rightarrow \neg p$(假定)
(3) $\neg(p \rightarrow \neg q) \rightarrow \neg p(1,2,H S)$
(4) $\neg(p \rightarrow \neg q) \rightarrow p$(按命题2-1°)

按归谬律，得证。

### 命题2-4°

即证$\vdash p \rightarrow \neg(p \rightarrow \neg p)$。

按演绎定理，即证$\{p\} \vdash \neg(p \rightarrow \neg p)$。

如果把$p\rightarrow \neg p$作为新假定，得到如下证明：

$(1)p$（假定）

$(2)p\rightarrow\neg p$（假定）

$(3)\neg p(1,2,MP)$

按归谬律，得证。

### 命题4-1°

记$r=\neg(p \wedge q), s=\neg p \vee \neg q$，即证$\vdash r \leftrightarrow s $，即证$\vdash(r \rightarrow s) \wedge(s \rightarrow r)$，即证$\vdash \neg((r \rightarrow s) \rightarrow \neg(s \rightarrow r))$。

下面先证明：$\vdash r\rightarrow s,\vdash s\rightarrow r$。

1° 按演绎定理，即证$\{\neg \neg(p \rightarrow \neg q)\} \vdash(\neg \neg p \rightarrow \neg q)$。

$\text {(1)} \neg \neg(p \rightarrow \neg q) \rightarrow(p \rightarrow \neg q)$（双重否定律）

$(2)\neg\neg(p→\neg q)$（假定）

$(3)p→\neg q$（1,2,MP）

$(4)\neg\neg p→p$（双重否定律）

$(5)\neg\neg p→\neg q$（3,4,HS）

2° 按演绎定理，即证$\{\neg \neg p \rightarrow \neg q\} \vdash\neg \neg(p \rightarrow \neg q)$。

$(1)\neg\neg p→\neg q$（假定）

$(2)p→\neg\neg p$（第二双重否定律）

$(3)p→\neg q$（1,2,HS）

$(4)(p→\neg q)→\neg\neg(p→\neg q)$（第二双重否定律）

$(5)\neg\neg(p→\neg q)$（3,4,MP）

下面给出原命题的证明：

如果把$(r \rightarrow s) \rightarrow \neg(s \rightarrow r)$作为新假定，则

$(1...5)r→s$

$(6...10)s→r$

$(11)(r \rightarrow s) \rightarrow \neg(s \rightarrow r)$（假定）

$(12)\neg(s \rightarrow r)$

按归谬律，得证。



