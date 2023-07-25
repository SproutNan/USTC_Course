from z3 import *
import time
import matplotlib.pyplot as plt

def NQueens_SMT(n):
    start = time.time()

    Q = [Int(f"Q_{i+1}") for i in range(n)]
    sol = Solver()
    val_c = [And(1 <= Q[i], Q[i] <= n) for i in range(n)]
    col_c = [Distinct(Q)]
    diag_c = [If(i==j, True, And(i+Q[i]!=j+Q[j], i+Q[j]!=j+Q[i])) for i in range(n) for j in range(i)]
    sol.add(val_c)
    sol.add(col_c)
    sol.add(diag_c)

    start1 = time.time()
    if sol.check() == sat:
        end1 = time.time()
        return end1 - start1, start1 - start

def NQueens_pureSAT(n):
    start = time.time()

    Q = [[Bool(f"Q_{i+1}_{j+1}") for j in range(n)] for i in range(n)]
    sol = Solver()
    # 每行每列都只有一个皇后
    for i in range(n):
        _AL = []
        for j in range(n):
            _BL = [Q[i][k] if k == j else Not(Q[i][k]) for k in range(n)]
            _AL.append(And(*_BL))
        sol.add(Or(*_AL))
    for j in range(n):
        _AL = []
        for i in range(n):
            _BL = [Q[k][j] if k == i else Not(Q[k][j]) for k in range(n)]
            _AL.append(And(*_BL))
        sol.add(Or(*_AL))

    # 主对角线上最多只有一个皇后
    for k in range(2*n-1):
        _AL = [Q[i][j] for i in range(n) for j in range(n) if i+j==k]
        # 最多一个的分支
        _Have1 = []
        for p in range(len(_AL)):
            _BL = [Not(_AL[i]) if i != p else _AL[i] for i in range(len(_AL))]
            _Have1.append(And(*_BL))
        # 没有的分支
        _Have0 = Not(Or(*_AL))
        sol.add(Or(*_Have1, _Have0))
    # 副对角线1
    for k in range(n):
        _AL = [Q[i][j] for i in range(n) for j in range(n) if i-j==k]
        # 最多一个的分支
        _Have1 = []
        for p in range(len(_AL)):
            _BL = [Not(_AL[i]) if i != p else _AL[i] for i in range(len(_AL))]
            _Have1.append(And(*_BL))
        # 没有的分支
        _Have0 = Not(Or(*_AL))
        sol.add(Or(*_Have1, _Have0))
    # 副对角线2
    for k in range(n):
        _AL = [Q[i][j] for i in range(n) for j in range(n) if j-i==k]
        # 最多一个的分支
        _Have1 = []
        for p in range(len(_AL)):
            _BL = [Not(_AL[i]) if i != p else _AL[i] for i in range(len(_AL))]
            _Have1.append(And(*_BL))
        # 没有的分支
        _Have0 = Not(Or(*_AL))
        sol.add(Or(*_Have1, _Have0))

    start1 = time.time()
    # 求解
    if sol.check() == sat:
        end1 = time.time()
        # 打印棋盘
        m = sol.model()
        for i in range(n):
            for j in range(n):
                if m[Q[i][j]]:
                    print("Q", end=" ")
                else:
                    print(".", end=" ")
            print()
        return end1 - start1, start1 - start

if __name__ == "__main__":
    n_list = [i for i in range(4, 50)]
    SMT_found = []
    SMT_check = []
    SAT_found = []
    SAT_check = []
    for n in n_list:
        a = NQueens_SMT(n)
        SMT_found.append(a[0])
        SMT_check.append(a[1])
        b = NQueens_pureSAT(n)
        SAT_found.append(b[0])
        SAT_check.append(b[1])
        print(n, a, b)

    plt.plot(n_list, SMT_found, marker='o', label='SMT Found Time')
    plt.plot(n_list, SMT_check, marker='s', label='SMT Check Time')
    plt.plot(n_list, SAT_found, marker='o', label='pureSAT Found Time')
    plt.plot(n_list, SAT_check, marker='s', label='pureSAT Check Time')

    plt.xlabel("n")
    plt.ylabel("running time (s)")
    plt.title("Performance of SMT and pureSAT")
    plt.legend()

    plt.show()