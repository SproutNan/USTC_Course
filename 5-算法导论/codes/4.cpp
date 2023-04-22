#include <iostream>
#include <string>
#include <vector>

using namespace std;

int kmp(string s) {
    int n = s.size();
    vector<int> next(n, -1);
    for (int i = 1; i < n; ++i) {
        int j = next.at(i - 1);
        while (j != -1 && s.at(j + 1) != s.at(i)) {
            j = next.at(j);
        }
        if (s.at(j + 1) == s.at(i)) {
            next.at(i) = j + 1;
        }
    }
    if (next.at(n - 1) != -1 && n % (n - next.at(n - 1) - 1) == 0) {
        return n - next.at(n - 1) - 1;
    }
    return -1;
}

int main() {
    string s;
    cin >> s;
    auto substr = kmp(s);
    if (substr == -1) cout << -1;
    else cout << s.substr(0, substr);
}