#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool Compare(string* a, string* b) {
    auto ab = *a + *b;
    auto ba = *b + *a;
    return stoi(ab) > stoi(ba);
}

int main() {
    int number;
    cin >> number;
    vector<string*> arr;
    for (int j = 0; j < number; j++) {
        int n;
        cin >> n;
        arr.push_back(new string(to_string(n)));
    }
    sort(arr.begin(), arr.end(), Compare);
    for (auto s : arr) {
        cout << s->c_str();
    }
}
