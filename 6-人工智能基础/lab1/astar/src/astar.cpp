#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>

using namespace std;

class State {
public:
    // 创建一个 N*N 的初始状态
    explicit State(int N, string& data) : width(N), data(data) {}
    // 输出状态识别码
    string hash() {
        return data;
    }
    // 给出启发式估计
    int heuristic() {
        int one_count = 0;
        for (int i = 0; i < width; i++)
            for (int j = 0; j < width; j++)
                if (data.at(i * width + j) == '1') {
                    one_count++;
                }
        return one_count / 3;
    }
    // 进行操作
    void operate(int i, int j, int method) {
        if (method == 1) {
            flip(i - 1, j);
            flip(i, j + 1);
        }
        else if (method == 2) {
            flip(i - 1, j);
            flip(i, j - 1);
        }
        else if (method == 3) {
            flip(i + 1, j);
            flip(i, j - 1);
        }
        else {
            flip(i + 1, j);
            flip(i, j + 1);
        }
        flip(i, j);
    }
    bool get(int r, int c) {
        return data.at(r * width + c) == '1';
    }
    int width;

private:
    string data;
    void flip(int i, int j) {
        if (data.at(i * width + j) == '1')
            data.at(i * width + j) = '0';
        else data.at(i * width + j) = '1';
    }
};

class Mission {
public:
    explicit Mission(State &s, int g = 0) : s(s), g(g + 1) {
        // g(n)：从初始状态到当前状态的代价
        f = g + s.heuristic();
    }

    int f{}, g{0};
    State s;
    vector<string> path;
};

struct Compare {
    bool operator()(const Mission &a, const Mission &b) const {
        return a.f > b.f;
    }
};

class Slover {
public:
    Slover(int width) : width(width) {}

    void read() {
        string ss = string(width * width, '0');
        char ch;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < width; j++) {
                cin >> ch;
                ss.at(i * width + j) = ch;
            }
        }
        State root_state(width, ss);
        pq.emplace(root_state);
    }

    void expand(int ii, int jj, int method, Mission& mission) {
        State child_s(mission.s);
        child_s.operate(ii, jj, method);
        if (!visited[child_s.hash()]) {
            visited[child_s.hash()] = true;
            Mission child_m(child_s, mission.g);
            child_m.path = mission.path;
            child_m.path.push_back(to_string(ii) + "," + to_string(jj) + "," + to_string(method));
            pq.push(child_m);
        }
    }

    void search() {
        // 搜索
        while (!pq.empty()) {
            auto mission = pq.top();
            pq.pop();

            auto mission_hash = mission.s.hash();

            if (mission_hash.find('1') == string::npos) {
                cout << mission.path.size() << endl;
                for (auto &s : mission.path) {
                    cout << s << endl;
                }
                break;
            }

            // 找到第一个1的位置
            int ii = -1, jj = -1;
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < width; j++) {
                    if (mission.s.get(i, j)) {
                        ii = i, jj = j;
                        break;
                    }
                }
                if (ii != -1) break;
            }

            // 生成子状态
            // *   *
            // * * *
            // *   *
            // 中间的是 ii,jj
            if (ii - 1 >= 0 && jj - 1 >= 0) {
                expand(ii, jj - 1, 1, mission);
            }
            if (ii - 1 >= 0 && jj + 1 < width) {
                expand(ii, jj + 1, 2, mission);
            }
            if (ii + 1 < width && jj - 1 >= 0) {
                expand(ii, jj - 1, 4, mission);
            }
            if (ii + 1 < width && jj + 1 < width) {
                expand(ii, jj + 1, 3, mission);
            }
            // 以 ii jj 为中心
            if (ii > 0 && jj > 0) {
                expand(ii, jj, 2, mission);
            }
            if (ii > 0 && jj < width - 1) {
                expand(ii, jj, 1, mission);
            }
            if (ii < width - 1 && jj > 0) {
                expand(ii, jj, 3, mission);
            }
            if (ii < width - 1 && jj < width - 1) {
                expand(ii, jj, 4, mission);
            }
            // * * *
            //   *(ii, jj)
            // * * *
            if (ii - 1 >= 0 && jj - 1 >= 0) {
                expand(ii - 1, jj, 3, mission);
            }
            if (ii - 1 >= 0 && jj + 1 < width) {
                expand(ii - 1, jj, 4, mission);
            }
            if (ii + 1 < width && jj - 1 >= 0) {
                expand(ii + 1, jj, 2, mission);
            }
            if (ii + 1 < width && jj + 1 < width) {
                expand(ii + 1, jj, 1, mission);
            }
        }
    }
private:
    int width;
    map<string, bool> visited;
    priority_queue<Mission, std::vector<Mission>, Compare> pq;
};

int main() {
    int width;
    cin >> width;
    Slover slover(width);
    slover.read();
    slover.search();
}
