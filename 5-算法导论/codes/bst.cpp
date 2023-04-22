#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

bool compare_v(std::pair<int, int>& a, std::pair<int, int>& b) {
    return a.first < b.first;
}

bool compare_c(std::pair<int, int>& a, std::pair<int, int>& b) {
    return a.second < b.second;
}

int main() {
    while (true) {
        std::ios::sync_with_stdio(false);
        int n;
        std::cin >> n;
        std::unordered_map<int, int> pos;
        std::vector<std::pair<int, int>> v;
        for (int i = 0; i < n; i++) {
            int tmp;
            std::cin >> tmp;
            if (pos[tmp] == 0) {
                v.push_back(std::pair<int, int>(tmp, 1));
                pos[tmp] = v.size();
            }
            else {
                int u = pos[tmp];
                v[u - 1].second++;
            }
        }
        std::sort(v.begin(), v.end(), compare_v);
        std::sort(v.begin(), v.end(), compare_c);
        for (auto& k : v) {
            int t_c = k.second;
            for (int j = 0; j < t_c; j++) {
                printf("%d ", k.first);
            }
        }
    }
}