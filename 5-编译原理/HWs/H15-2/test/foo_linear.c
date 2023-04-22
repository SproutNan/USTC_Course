int foo(int n) {
    if (1 == n) { 
        return 1;
    } 
    return (n % 3 + 1) * foo(n - 1);
}

int main() {
    return foo(10);
}