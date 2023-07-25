#include <array>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

vector<string> split(const string& str) {
    char delimiter = ',';
    vector<string> result;
    string temp;
    for (char i : str) {
        if (i == delimiter) {
            result.push_back(temp);
            temp = "";
        }
        else temp += i;
    }
    result.push_back(temp);
    return result;
}

class shift {
public:
    shift(const shift &other) = default;
    shift(int day, int shift_cnt) : day(day), shift_cnt(shift_cnt) {}

    // 把当前排班赋值给指定的阿姨
    bool assign(int aunt_index) {
        if (enable == -1) {
            enable = aunt_index;
            return true;
        }
        return false;
    }

    // 禁止排某阿姨的班
    void ban(int aunt_index) {
        if (ban1 == -1) ban1 = aunt_index;
        else ban2 = aunt_index;
        banned_num++;
    }

    // 查找是否被ban
    [[nodiscard]] bool isBanned(int aunt_index) const {
        return ban1 == aunt_index || ban2 == aunt_index;
    }

    // 是否被赋值
    [[nodiscard]] bool isAssigned() const {
        return enable != -1;
    }

    int day{};            // 表示第几天
    int shift_cnt{};      // 表示这天第几个shift
    int enable = -1;    // 表示当前已被选择的阿姨的下标，-1表示这个shift尚未被赋值
    int banned_num = 0; // 表示当前shift被禁止的阿姨的数量，用于最小启发式搜索
    int ban1 = -1, ban2 = -1;
};

class mission {
public:
    mission(int n, int d, int s) : n(n), d(d), s(s) {
        for (int i = 0; i < d; i++) {
            for (int j = 0; j < s; j++) {
                shifts.emplace_back(i, j);
            }
        }
        aunt_shifts = vector<int>(n, 0);
    }
    mission(const mission &other) {
        for (auto &tt : other.shifts) {
            shifts.emplace_back(tt);
        }
        n = other.n;
        d = other.d;
        s = other.s;
        aunt_shifts = vector<int>(other.aunt_shifts);
        shift_requests = vector<int>(other.shift_requests);
    }

    // 把得到的答案转换成结果，并统计数据
    vector<string> transAns(auto& requests) {
        vector<string> tmp_ans;
        int cnt = 0;
        for (int i = 0; i < d; i++) {
            string ans;
            for (int j = 0; j < s; j++) {
                int arranged_aunt = shifts.at(i * s + j).enable;
                ans += to_string(arranged_aunt + 1);
                if (requests.at(i).at(j).at(arranged_aunt) == 1) {
                    cnt++;
                }
                if (j < s - 1) {
                    ans += ',';
                }
            }
            tmp_ans.push_back(ans);
        }
        tmp_ans.push_back(to_string(cnt));
        return tmp_ans;
    }

    // 判断当前节点，是否所有的排班都已被赋值
    bool isComplete() {
        for (auto &tt : shifts) {
            if (tt.enable == -1)
                return false;
        }
        return true;
    }

    // 返回是否还可继续赋值: 还有班没排，但是有一班因为约束无法排班了
    bool CanContinue() {
        if (!isComplete()) {
            for (auto &tt : shifts) {
                if (tt.banned_num >= n)
                    return false;
            }
        }
        return true;
    }

    // 初始化当前每个shift有多少阿姨请求排班
    void initShiftNum(auto& requests) {
        for (int dd = 0; dd < d; dd++) {
            for (int ss = 0; ss < s; ss++) {
                int shift_req_num = 0;
                for (int nn = 0; nn < n; nn++) {
                    if (requests.at(dd).at(ss).at(nn)) {
                        shift_req_num++;
                    }
                }
                shift_requests.push_back(shift_req_num);
            }
        }
    }

    vector<shift> shifts;
    // 统计在某个shift有多少人发出请求
    vector<int> shift_requests;
    vector<int> aunt_shifts; // 当前每个阿姨被排了多少班
    int n{}, d{}, s{};
};

int main() {
    int N, D, S;
    scanf("%d,%d,%d", &N, &D, &S);
    // 用一个二维数组存储，某天某班有哪些阿姨请求排班
    vector<vector<vector<int>>> requests(D, vector<vector<int>>(S, vector<int>(N, 0)));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < D; j++) {
            string req;
            cin >> req;
            vector<string> reqs = split(req);

            for (int s = 0; s < S; s++) {
                if (reqs[s] == "1") {
                    requests.at(j).at(s).at(i) = 1;
                }
            }
        }
    }

    stack<mission> stk;

    auto start_mission = mission(N, D, S);
    start_mission.initShiftNum(requests);
    stk.push(start_mission);

    vector<string> glob_ans;

    while (!stk.empty()) {
        auto current_mission = stk.top();
        stk.pop();

        // 需要检查是否无可赋值
        //   下面一行表示，当前任务不可赋值（约束），或者排班不正确需要剪枝，必须丢弃这个节点
        if (!current_mission.CanContinue()) continue;
        //   下面一行表示，当前任务已经结束，可以输出验证
        if (current_mission.isComplete()) {
            bool flag = false;
            for (auto& item : current_mission.aunt_shifts) {
                if (item < D * S / N) {
                    flag = true;
                    break;
                }
            }
            if (flag) continue;
            // 统计满足要求的个数
            glob_ans = current_mission.transAns(requests);
            break;
        }

        // 选择还没被排班的班次，再从这样的班次中找到具有最小可排班集合的班次
        // 这里不用 : 是有好处的，方便索引
        int available_aunt_num = N + 1;
        int target_index = -1;
        for (int k = 0; k < D * S; k++) {
            auto current_shift = current_mission.shifts.at(k);
            // 计算当前班次发起请求的阿姨数量
            auto remains_aunt_num = current_mission.shift_requests.at(k);
            // 这个班次未被赋值
            if (!current_shift.isAssigned() && (remains_aunt_num < available_aunt_num)) {
                available_aunt_num = remains_aunt_num;
                target_index = k;
            }
        }
        current_mission.shift_requests.at(target_index) = N + 1;
        // 最终选择是第 k 班(第day天，第shift班)，现在要考虑给第 k 班哪个阿姨赋值
        int k_day = target_index / S;
        int k_shift = target_index % S;
        // cout << "target_index: " << target_index << ", k_shift: " << k_shift << endl;
        auto k_shift_object = current_mission.shifts.at(target_index);
        // 因为是搜索，需要把第 k 班所有可能的赋值都放进去（也就是说，除了ban掉的其他都要放），但是放的顺序有讲究，最不应该赋值的最先入栈，最应该赋值的最后入栈

        // 第0个位置是阿姨编号，第1个位置是阿姨当前排班数，第2个位置是表示阿姨有没有请求排班
        // 只需要剔除被ban的阿姨即可
        vector<tuple<int, int, int>> tmp;
        auto current_shift_status = current_mission.aunt_shifts;
        for (int i = 0; i < N; i++) {
            if (!k_shift_object.isBanned(i)) {
                tmp.emplace_back(i, current_shift_status[i], requests[k_day][k_shift][i]);
            }
        }
        // 现在tmp中，是第二个元素小的排在前面，第一个元素是阿姨编号，第三个元素是是否请求
        sort(tmp.begin(), tmp.end(), [](auto &a, auto &b) {
            if (get<2>(a) != get<2>(b)) {
                return get<2>(a) < get<2>(b);
            }
            return get<1>(a) > get<1>(b);
        });
        // 现在需要根据tmp中的个数T，按照tmp中的顺序创建T个新的mission对象
        // 对每个对象做如下处理：
        //   赋值、约束传播、入栈
        for (auto& t : tmp) {
            auto new_mission = mission(current_mission);
            // 赋值
            new_mission.shifts.at(target_index).assign(get<0>(t));
            //new_mission.assign_order.emplace_back(target_index, get<0>(t));
            new_mission.aunt_shifts.at(get<0>(t)) += 1;

            // 约束传播
            if (target_index > 0) {
                new_mission.shifts.at(target_index-1).ban(get<0>(t));
            }
            if (target_index < D * S - 1) {
                new_mission.shifts.at(target_index+1).ban(get<0>(t));
            }
            stk.push(new_mission);
        }
    }
    // 打印答案
    for (auto& ss : glob_ans) {
        cout << ss << endl;
    }
}
