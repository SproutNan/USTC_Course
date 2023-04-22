#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> caculate_next(string s) {
    int n = s.size();
    vector<int> next(n);
    for (int i = 1, j = 0; i < n; ++i) {
        while (j != 0 && s.at(j) != s.at(i)) {
            j = next.at(j - 1);
        }
        if (s.at(j) == s.at(i)) {
            j++;
        }
        next.at(i) = j;
    }
    return next;
}

int matcher(string& s, string& p, vector<int>& next) {
    int n = s.size();
    int m = p.size();
    for (int i = 0, j = 0; i < n; ++i) {
        while (j != 0 && s.at(i) != p.at(j)) {
            j = next.at(j - 1);
        }
        if (s.at(i) == p.at(j)) {
            j++;
        }
        if (j == m) {
            return i - m + 1;
        }
    }
    return -1;
}

int main() {
    int T;
    cin >> T;
    for (int i = 0; i < T; i++) {
        string s;
        string p;
        cin >> p >> s;
        int p_size = p.size();
        auto next = caculate_next(p);
        int num = 0;
        while (true) {
            int pos = matcher(s, p, next);
            if (pos == -1) {
                break;
            }
            else {
                num++;
                s = s.substr(pos + 1);
            }
        }
        cout << num << endl;
    }
}