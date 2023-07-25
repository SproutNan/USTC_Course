from z3 import *

def Iff(x, y):
    return Not(Xor(x, y))

def calculate_a_plus_b(a, b):
    n = len(bin(max(a, b))) - 1
    a_str = (bin(a)[2:]).zfill(n)[::-1]
    b_str = (bin(b)[2:]).zfill(n)[::-1]
    sol = Solver()
    A = [Bool(f'a_{i}') for i in range(n)]
    B = [Bool(f'b_{i}') for i in range(n)]
    C = [Bool(f'c_{i}') for i in range(n)]
    D = [Bool(f'd_{i}') for i in range(n)]
    for i in range(n):
        sol.add(If(a_str[i] == '1', A[i], Not(A[i])))
        sol.add(If(b_str[i] == '1', B[i], Not(B[i])))
    sol.add(Not(C[0]))
    for i in range(1, n):
        sol.add(Iff(C[i], Or(Or(And(A[i - 1], B[i - 1]), And(A[i - 1], C[i - 1])), And(B[i - 1], C[i - 1]))))
    for i in range(n):
        sol.add(Iff(D[i], Iff(A[i], Iff(B[i], C[i]))))

    if sol.check() == sat:
        m = sol.model()
        S = ''.join([m.evaluate(If(D[i], '1', '0')).as_string() for i in range(n)])
        return int(S[::-1], 2)
    else:
        return -1

def calculate_a_minus_b(a, b):
    n = len(bin(max(a, b))) - 1
    a_str = (bin(a)[2:]).zfill(n)[::-1]
    b_str = (bin(b)[2:]).zfill(n)[::-1]
    sol = Solver()
    A = [Bool(f'a_{i}') for i in range(n)]
    B = [Bool(f'b_{i}') for i in range(n)]
    C = [Bool(f'c_{i}') for i in range(n)]
    D = [Bool(f'd_{i}') for i in range(n)]
    for i in range(n):
        sol.add(If(a_str[i] == '1', D[i], Not(D[i])))
        sol.add(If(b_str[i] == '1', B[i], Not(B[i])))
    sol.add(Not(C[0]))
    for i in range(1, n):
        sol.add(Iff(C[i], Or(Or(And(A[i - 1], B[i - 1]), And(A[i - 1], C[i - 1])), And(B[i - 1], C[i - 1]))))
    for i in range(n):
        sol.add(Iff(D[i], Iff(A[i], Iff(B[i], C[i]))))

    if sol.check() == sat:
        m = sol.model()
        S = ''.join([m.evaluate(If(A[i], '1', '0')).as_string() for i in range(n)])
        return int(S[::-1], 2)
    else:
        return -1


if __name__ == "__main__":
    option = sys.argv[1]
    a = int(sys.argv[2])
    b = int(sys.argv[3])

    if option == 'minus':
        result = calculate_a_minus_b(a, b)
        if result != -1:
            print(f"The difference between {a} and {b} is {result}")
        else:
            print('error, check parameters')
    elif option == 'plus':
        result = calculate_a_plus_b(a, b)
        if result != -1:
            print(f"The sum of {a} and {b} is {result}")
        else:
            print('error, check parameters')
    else:
        print("Invalid option. Please enter 'plus' or 'minus'")
