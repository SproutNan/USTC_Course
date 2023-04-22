//@ https://blog.csdn.net/weixin_72060925/article/details/128223983
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#pragma warning(disable:4996)
#define INF 9999999

using namespace std;

typedef struct vertex {
	int num;
	int d;
	map<int, int> adj; // first 对方顶点数字，second 是cost
} vertex;

using heap = priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>;

class DG {
public:
	vector<vertex*> V;

	int vertex_num;

	DG(int vertex_num) {
		this->vertex_num = vertex_num;
		for (int i = 0; i < vertex_num; i++) {
			this->V.push_back(new vertex{ i, INF });
		}
	}

	void source_init(int source) {
		this->V[source]->d = 0;
	}

	void insert_edge(int source, int target, int cost) {
		if (this->V[source]->adj[target] != 0 && this->V[source]->adj[target] < cost) {
			return;
		}
		this->V[source]->adj[target] = cost;
	}

	void relax(int u, int v, heap& q) {
		int new_d = this->V[u]->d + this->V[u]->adj[v];
		if (this->V[v]->d > new_d) {
			this->V[v]->d = new_d;
			q.push(make_pair(new_d, v));
		}
	}
};

int main() {
	int V_num, E_num, source, target;
	scanf("%d %d %d %d", &V_num, &E_num, &source, &target);
	source--;
	target--;
	DG graph(V_num);
	for (int i = 0; i < E_num; i++) {
		int s, t, c;
		scanf("%d %d %d", &s, &t, &c);
		graph.insert_edge(s - 1, t - 1, c);
	}

	graph.source_init(source);
	heap q;

	vector<bool> S(V_num, false);

	q.push(make_pair(0, source));

	while (q.size()) {
		auto u_vertex = q.top();
		int u = u_vertex.second;
		q.pop();
		if (S[u]) {
			continue;
		}
		S[u] = true;

		for (auto& v_vertex : graph.V[u]->adj) {
			int v = v_vertex.first;
			graph.relax(u, v, q);
		}
	}

	if (graph.V[target]->d >= INF) {
		cout << -1;
	}
	else {
		cout << graph.V[target]->d;
	}
}
