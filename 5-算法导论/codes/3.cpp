// @ https://www.luogu.com.cn/problem/P3381
#include<iostream>
#include<queue>
#define INF 9999999
#define MAX 600000
#pragma warning(disable:4996)

using namespace std;

int V_num, E_num, source, target;

int iter = 1;

int dis[MAX], curr[MAX], pred[MAX];

struct node {
	int target, net;
	int c;
} Edges[MAX];

void insert_edge(int u, int v, int c) {
	iter++;
	Edges[iter].target = v;
	Edges[iter].c = c;
	Edges[iter].net = pred[u];
	pred[u] = iter;

	iter++;
	Edges[iter].target = u;
	Edges[iter].c = 0;
	Edges[iter].net = pred[v];
	pred[v] = iter;
}

bool bfs(int V_num, int source, int target) {
	for (int i = 1; i <= V_num; i++) {
		dis[i] = INF;
	}

	queue<int> q;
	q.push(source);
	dis[source] = 0;
	curr[source] = pred[source];

	while (q.size()) {
		int x = q.front();
		q.pop();
		for (int i = pred[x]; i; i = Edges[i].net) {
			int v = Edges[i].target;
			if (Edges[i].c > 0 && dis[v] == INF) {
				q.push(v);
				curr[v] = pred[v];
				dis[v] = dis[x] + 1;
				if (v == target) return true;
			}
		}
	}
	return false;
}

int dfs(int x, int sum) {
	if (x == target) {
		return sum;
	}
	int current_residual, res = 0;
	for (int i = curr[x]; i && sum; i = Edges[i].net) {
		curr[x] = i;
		int v = Edges[i].target;
		if (Edges[i].c > 0 && (dis[v] == dis[x] + 1)) {
			current_residual = dfs(v, min(sum, Edges[i].c));
			if (current_residual == 0) dis[v] = INF; 
			Edges[i].c -= current_residual;
			Edges[i ^ 1].c += current_residual;
			res += current_residual;
			sum -= current_residual;
		}
	}
	return res;
}

int main() {
	scanf("%d %d %d %d", &V_num, &E_num, &source, &target);
	for (int i = 1; i <= E_num; i++) {
		int u, v, c;
		scanf("%d %d %d", &u, &v, &c);
		insert_edge(u, v, c);
	}
	int ans = 0;
	while (bfs(V_num, source, target)) {
		ans += dfs(source, INF);
	}
	cout << ans;
}