#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void display(vector<int> &v) {
  for (auto &i : v) {
    cout << i << " ";
  }
  cout << endl;
}

void swap(vector<int> &v, int i, int j) {
  int tmp = v[i];
  v[i] = v[j];
  v[j] = tmp;
}

class insertSort {
public:
  insertSort() = default;

  void run(vector<int> &v) {
    int length = v.size();
    for (int i = 1; i < length; i++) {
      if (v[i] < v[i - 1]) {
        for (int j = i; j >= 1; j--) {
          if (v[j] < v[j - 1]) {
            swap(v, j, j - 1);
          } else {
            break;
          }
        }
      }
    }
  }
};

class mergeSort {
public:
  mergeSort() = default;
  void merge(vector<int> &v, int begin1, int end1, int begin2, int end2) {
    vector<int> ans;
    int i = begin1;
    int j = begin2;
    while (i <= end1 || j <= end2) {
      if (v[i] <= v[j] && i <= end1) {
        ans.push_back(v[i]);
        i++;
      } else if (v[i] <= v[j] && i > end1) {
        ans.push_back(v[j]);
        j++;
      } else if (v[j] < v[i] && j <= end2) {
        ans.push_back(v[j]);
        j++;
      } else if (v[j] < v[i] && j > end2) {
        ans.push_back(v[i]);
        i++;
      }
    }
    int size = ans.size();
    for (int k = 0; k < size; k++) {
      v[k + begin1] = ans[k];
    }
  }

  void run(vector<int> &v, int begin, int end) {
    // 这里 begin 和 end 都是闭区间
    if (end == begin)
      return;
    int mid = (begin + end) >> 1;
    run(v, begin, mid);
    run(v, mid + 1, end);
    merge(v, begin, mid, mid + 1, end);
  }
};

class quickSort {
public:
  quickSort() = default;

  int partition(vector<int> &v, int begin, int end, int piviot) {
    if (end == begin)
      return begin;
    swap(v, piviot, end); //把piviot放到数组最后
    int x = begin;
    int y = begin;
    for (; y < end; y++) {
      if (v[y] < v[end]) {
        swap(v, x, y);
        x++;
      }
    }
    swap(v, x, end);
    return x;
  }
  void run(vector<int> &v, int begin, int end) {
    if (begin >= end || begin < 0 || end < 0)
      return;
    int piviot = (begin + end) >> 1;
    int new_piviot = partition(v, begin, end, piviot);
    run(v, begin, new_piviot - 1);
    run(v, new_piviot + 1, end);
  }
};

class countingSort {
public:
  countingSort() = default;

  void run(vector<int> &v, int max) {
    int size = v.size();
    vector<int> ans;
    vector<int> tmp;
    for (int i = 0; i < max + 1; i++)
      tmp.push_back(0);
    for (int j = 0; j < size; j++)
      tmp[v[j]] += 1;
    int process = 0;
    while (process < max + 1) {
      if (tmp[process] == 0) {
        process++;
        continue;
      }
      ans.push_back(process);
      tmp[process]--;
    }
    for (int i = 0; i < size; i++)
      v[i] = ans[i];
  }
};

class heapSort {
public:
  
}

int main(int argc, char **argv) {
  vector<int> v({9, 3, 5, 4, 9, 1, 2, 7, 8, 1, 3, 6, 5, 3, 4, 0, 10, 9, 7, 9});
  // auto method = new insertSort;
  // method->run(v);
  // auto method = new mergeSort;
  // method->run(v, 0, v.size()-1);
  // auto method = new quickSort;
  // method->run(v, 0, v.size()-1);
  // auto method = new countingSort;
  // method->run(v, 10);
  display(v);
}