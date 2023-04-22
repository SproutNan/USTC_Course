int _foo(int n, int a) {
    if (1 == n) {
        return a;
    }
    return _foo(n - 1, (n % 3 + 1) * a);
}

int foo(int n) {
    if (1 == n) {
        return 1;
    }
    return _foo(n, 1);
}

int main() {
    return foo(10);
}