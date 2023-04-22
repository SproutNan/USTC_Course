#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int merge(int *v, int begin1, int end1, int begin2, int end2) {
  vector<int> ans;
  int i = begin1;
  int j = begin2;
  int count = 0;
  while (i <= end1 || j <= end2) {
    if (v[i] <= v[j] && i <= end1) {
      ans.push_back(v[i]);
      i++;
    } else if (v[i] <= v[j] && i > end1) {
      ans.push_back(v[j]);
      int tmp = end1 - i + 1;
      count += tmp%10000007;
      count %= 10000007;
      j++;
    } else if (v[j] < v[i] && j <= end2) {
      ans.push_back(v[j]);
      int tmp = end1 - i + 1;
      count += tmp%10000007;
      count %= 10000007;
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
  return count;
}

int run(int *v, int begin, int end) {
  // 这里 begin 和 end 都是闭区间
  if (end == begin)
    return 0;
  int mid = (begin + end) >> 1;
  int a = run(v, begin, mid) % 10000007;
  int b = run(v, mid + 1, end) % 10000007;
  int c = merge(v, begin, mid, mid + 1, end) % 10000007;
  return (a+b+c)%10000007;
}

int main() {
  std::ios::sync_with_stdio(false);
  int n;
  cin >> n;
  int *arr = (int *)malloc(n * sizeof(int));
  for (int j = 0; j < n; j++) {
    cin >> arr[j];
  }
  int ans = run(arr, 0, n - 1);
  ans %= 10000007;
  cout << ans;
}