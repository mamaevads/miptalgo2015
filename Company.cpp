#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void dfs(int v, const vector<vector<int>>& g, vector<bool>& used, vector<int>* order) {
	used[v] = true;
	for (int to : g[v])
		if (!used[to])
			dfs(to, g, used, order);
	if (order)
		order->push_back(v);
}

int getAnsw(int numcan, vector<vector<int>>& win, vector<vector<int>>& lost){

	vector<bool> used(numcan, false);
	vector<int> order;
	for (int i = 0; i < numcan; i++)
		if (!used[i])
			dfs(i, win, used, &order);
	used.assign(numcan, false);
	int mn = numcan;
	vector<int> numComponent(numcan, -1);
	int color = 0;

	for (int i = numcan - 1; i >= 0; --i) {
		int v = order[i];
		if (!used[v]) {
			vector<int> comp;
			dfs(v, lost, used, &comp);
			color++;
			for (int u : comp)
				numComponent[u] = color;
			bool wasOut = false;
			for (int u : comp)
				for (int to : lost[u])
					if (numComponent[to] != color)
						wasOut = true;
			if (!wasOut)
				mn = min(mn, (int)comp.size());
		}
	}

	int answ = numcan - mn + 1;
	return answ;
}


int main(){
	int numcan, m;
	cin >> numcan >> m;
	vector<vector<int>> win(numcan, vector<int>()), lost(numcan, vector<int>());
	for (int i = 0; i < m; i++) {
		int u, v, res;
		cin >> u >> v >> res;
		--u, --v;
		if (res == 3) continue;
		if (res == 2) swap(u, v);
		win[u].push_back(v);
		lost[v].push_back(u);
	}
	
	cout << getAnsw(numcan, win, lost);
	
	return 0;
}
