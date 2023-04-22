# 编译原理作业 HW8

<center>PB20111686 黄瑞轩</center>

## 4.12

拓广文法：

- $S'→S$
- $S→(L)$
- $S→a$
- $L→L,S$
- $L→S$

**语法制导定义**：

给 $S,L$ 继承属性 $i$ 表示左侧字符数，综合属性 $s$ 表示文法符号推出的字符串中最后一个字符在句子中是第几个字符。

|  产生式   |               语法制导定义                |
| :-------: | :---------------------------------------: |
|  $S'→S$   |                  $S.i=0$                  |
|  $S→(L)$  |        $L.i=S.i+1;\quad S.s=L.s+1$        |
|   $S→a$   |        $S.s=S.i+1\quad print(S.s)$        |
| $L→L_1,S$ | $L_1.i=L.i\quad S.i=L_1.s+1\quad L.s=S.s$ |
|   $L→S$   |          $L.s=S.s\quad S.i=L.i$           |

**翻译方案**：

- $S'→\{S.i=0;\}S$
- $S→(\{L.i=S.i+1;\}L)\{S.s=L.s+1;\}$
- $S→a\{S.s=S.i+1;print(S.s);\}$
- $L→\{L_1.i=L.i;\}L_1,\{S.i=L_1.s+1;\}S\{L.s=S.s;\}$
- $L→\{S.i=L.i;\}S\{L.s=S.s;\}$

消除左递归：

- $S'→S$
- $S→(L)$
- $S→a$
- $L→SL'$
- $L'→,SL'$
- $L'→\varepsilon$

消除左递归后的语法制导定义：

|      产生式      |                 语法制导定义                  |
| :--------------: | :-------------------------------------------: |
|      $S'→S$      |                   $S.i = 0$                   |
|     $S→(L)$      |       $L.i = S.i +1\quad S.s = L.s + 1$       |
|      $S→a$       |        $S.s = S.i + 1\quad print(S.s)$        |
|     $L→SL'$      |   $S.i=L.i\quad L'.i = S.s\quad L.s = L'.s$   |
|   $L'→,SL'_1$    | $L'_1.i=S.s\quad L'.s=L'_1.s\quad S.i=L'.i+1$ |
| $L'→\varepsilon$ |                  $L'.s=L'.i$                  |

**预测翻译器**：

```C
void S_prime() {
    S(0);
} 

int S(int S_i) {
    if (lookahead == 'a') {
        match('a');
        int S_s = S_i + 1;
        print(S_s);
        return S_s;
    }
    else if (lookahead == '(') {
        match('(');
        int L_s = L(S_i + 1);
        match(')');
        return L_s + 1;
    }
    else error(); 
}

int L(int L_i) {
    int S_i = L_i;
    int S_s = S(S_i);
    int L_prime_i = S_s;
    int L_prime_s = L_prime(L_prime_i);
    return L_prime_s;
}

int L_prime(int L_prime_i) {
    if (lookahead == ',') {
        match(',');
        int S_i = L_prime_i + 1;
        int S_s = S(S_i);
        int L_prime_1_i = S_s;
        int L_prime_1_s = L_prime(L_prime_1_i);
        return L_prime_1_s;
    }
    else {
        return L_prime_i;
    }
}
```

## 4.14

（a）给 $D$ 一个综合属性 $s$ 表示声明的 id 数。

|          产生式          |   语法制导定义    |
| :----------------------: | :---------------: |
|          $P→D$           |   $print(D.s)$    |
|       $D→D_1;D_2$        | $D.s=D_1.s+D_2.s$ |
|         $D→id:T$         |      $D.s=1$      |
| $D→proc\text{ }id;D_1;S$ |   $D.s=D_1.s+1$   |

（b）给 $D$ 一个继承属性 $i$ 表示 $D$ 中变量 $id$ 的嵌套深度。

翻译方案：

- $P→\{D.i=1;\}D$
- $D→\{D_1.i=D.i;\}D_1;\{D_2.i=D.i;\}D_2$
- $D→id:T\{print(id.name,D.i)\}$
- $D→proc\text{ }id;\{D_1.i=D.i+1;\}D_1;S$

