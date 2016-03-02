#include <iostream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
using namespace std;

struct SegmentTree{
	vector<int> tree;

	explicit SegmentTree(const vector <int>& points){
		size_t sizeTree = 1;
		while (sizeTree < points.size())
			sizeTree *= 2;
		tree.assign(2 * sizeTree, 0);
	}

	void update(size_t index){
		tree[index]++;
		if (index != 1) update(index / 2);
	}

	int find(int value, size_t vertex) const{
		while (vertex < tree.size() / 2){
			if (tree[vertex * 2] >= value){
				vertex = 2 * vertex;
			}
			else if (tree[vertex * 2] < value){
				value = value - tree[vertex * 2];
				vertex = vertex * 2 + 1;
			}
		}
		return vertex - tree.size() / 2;
	}
};

int main(){
	std::ios_base::sync_with_stdio(false);
	size_t n;
	cin >> n;
	double a;
	cin >> a;
	vector<int> values(n), helper(n);
	for (size_t i = 0; i < n; i++){
		cin >> helper[i];
		values[i] = helper[i];
	}
	sort(helper.begin(), helper.end());
	helper.erase(unique(helper.begin(), helper.end()), helper.end());
	unordered_map<int, size_t> maphelper(helper.size());
	for (size_t i = 0; i < helper.size(); i++){
		maphelper[helper[i]] = i;
	}
	SegmentTree tree = SegmentTree(helper);
	for (size_t i = 0; i < n; i++){
		tree.update(tree.tree.size() / 2 + maphelper[values[i]]);
		cout<<helper[tree.find((int)(a*i) + 1, 1)]<<'\n' ;
	}

	return 0;
}