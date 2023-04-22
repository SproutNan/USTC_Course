#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct road {
    int city_un;
    int city_deux;
    int fee;
};

using road = struct road;

bool compare(road* road_un, road* road_deux) {
    return road_un->fee < road_deux->fee;
}

// 返回所有城市是否都已访问
bool all_visited(vector<bool>& visited) {
    const int num = visited.size();
    for (int j = 0; j < num; j++) {
        if (visited[j] == false) {
            return false;
        }
    }
    return true;
}

int main() {
    int city_num = 0;
    int road_num = 0;
    cin >> city_num >> road_num;
    // 建立城市-道路表（好像没用）
    // vector<vector<road*>> city_road_list(city_num);
    // 建立道路表
    vector<road*> road_list;
    // 读取每一条路径
    for (int _ = 0; _ < road_num; _++) {
        int city_un = 0;
        int city_deux = 0;
        int fee = 0;
        cin >> city_un >> city_deux >> fee;
        auto new_road = new road{ city_un-1, city_deux-1, fee };
        road_list.push_back(new_road);
    }
    // 下面开始运行Prim算法
    vector<bool> visited(city_num);
    // 将所有道路按fee排序
    sort(road_list.begin(), road_list.end(), compare);
    // for (auto& road : road_list){
    //     cout << road->city_un << " " << road->city_deux << " " << road->fee <<
    //     endl;
    // }
    int fee_used = 0; // 花费的总费用
    // 将0号城市标记为已访问
    visited[0] = true;
    do {
        for (auto road : road_list) {
            if (visited[road->city_un] && !visited[road->city_deux]) {
                visited[road->city_deux] = true;
                fee_used += road->fee;
                break;
            }
            else if (visited[road->city_deux] && !visited[road->city_un]) {
                visited[road->city_un] = true;
                fee_used += road->fee;
                break;
            }
        }
    } while (!all_visited(visited));
    cout << fee_used;
}