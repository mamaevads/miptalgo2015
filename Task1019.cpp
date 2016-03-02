#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <stdio.h>

using namespace std;

bool const WHITE = 1;

struct TResult
{
	int value, start;
	TResult() : value(-1) {
	}
	TResult(int value, int start) : value(value), start(start) {
	}
	void updateMax(const TResult& data){
		if (value < data.value){
			value = data.value;
			start = data.start;
		}
	}
};

struct TInfo{
	int prefLen, sufLen, lenSegment, startIndex;
	TResult best;
	explicit TInfo(int lenSegment = 0, int startIndex = 0) : prefLen(lenSegment), sufLen(lenSegment),
		lenSegment(lenSegment), startIndex(startIndex), best(lenSegment, startIndex){
	}

	TInfo(int prefLen, int sufLen, int lenSegment, int startIndex, const TResult& best) : prefLen(prefLen), sufLen(sufLen),
	lenSegment(lenSegment), startIndex(startIndex), best(best){
	
	}
	
	void update(const TInfo& left, const TInfo& right){
		
		prefLen = left.prefLen;
		if (left.prefLen == left.lenSegment)
			prefLen += right.prefLen;
		sufLen = right.sufLen;
		if (right.prefLen == right.lenSegment)
			sufLen += left.sufLen;

		lenSegment = left.lenSegment + right.lenSegment;
		startIndex = left.startIndex;
		
		best = left.best;
		best.updateMax(right.best);
		best.updateMax(TResult(left.sufLen + right.prefLen, right.startIndex - left.sufLen));
	}

};



struct TSegmentTree{
	vector<TInfo> tree;
	vector<int> postponed;
	
	explicit TSegmentTree(const vector <int>& points){
		size_t sizeTree = 1;
		while (sizeTree < points.size()) 
			sizeTree *= 2;
		tree.assign(2 * sizeTree, TInfo());
		postponed.assign(2 * sizeTree, -1);
		for (size_t i = 0, j = sizeTree; i + 1 < points.size(); i++, j++){
			tree[j] = TInfo(points[i + 1] - points[i], points[i]);
		}	
		for (size_t i = sizeTree - 1; i >= 1; i--)
			tree[i].update(tree[getLeftChildIndex(i)], tree[getRightChildIndex(i)]);
	}
	void update(int left, int right, bool value){
		update(1, 0, tree.size() / 2, left, right, value);
	}
	TResult getLongestSegment() const {
		return tree[1].best;
	}
private:
	int getLeftChildIndex(int i){
		return 2 * i;
	}

	int getRightChildIndex(int i){
		return 2 * i + 1;
	}

	void assign(int vertex, bool value){
		postponed[vertex] = value;
		if (value == WHITE) {
			tree[vertex].prefLen = tree[vertex].sufLen = tree[vertex].lenSegment;
			tree[vertex].best = TResult(value, tree[vertex].lenSegment);
		}
		else {
			tree[vertex].prefLen = tree[vertex].sufLen = 0;
			tree[vertex].best = TResult(0, tree[vertex].startIndex);
		}
	}
	void push(int vertex) {
		if (postponed[vertex] != -1) {
			assign(getLeftChildIndex(vertex) , postponed[vertex]);
			assign(getRightChildIndex(vertex), postponed[vertex]);
			postponed[vertex] = -1;
		}
	}
	void update(int vertex, int vertexleft, int vertexright, int leftborder, int rightborder, bool value) {
		if (vertexleft >= rightborder|| leftborder >= vertexright)
			return;
		if (leftborder <= vertexleft && vertexright <= rightborder){
			assign(vertex, value);
			return;
		}
		push(vertex);
		int mid = (vertexleft + vertexright) / 2;
		update(2 * vertex, vertexleft, mid, leftborder, rightborder, value);
		update(2 * vertex + 1, mid, vertexright, leftborder, rightborder, value);
		tree[vertex].update(tree[2 * vertex], tree[2 * vertex + 1]);
	}
};



int main(){
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);

	int n;
	cin >> n;
	vector <int> startpoints(n), endpoints(n);
	vector <char> color(n);
	map <int, int> points;
	points[0] = 0;
	points[1e9] = 0;
	for (int i = 0; i < n; i++){
		cin >> startpoints[i] >> endpoints[i] >> color[i];
		points[startpoints[i]] = 0;
		points[endpoints[i]] = 0;
	}
	vector<int> arrayPoints;
	for (auto& element : points) {
		element.second = arrayPoints.size();
		arrayPoints.push_back(element.first);
	}
	TSegmentTree tree(arrayPoints);
	for (int i = 0; i < n; i++){
		TResult best = tree.getLongestSegment();

		int left = points[startpoints[i]];
		int right = points[endpoints[i]];
		tree.update(left, right, color[i] == 'w');

	}
	TResult best = tree.getLongestSegment();
	cout << best.start << " " << best.start + best.value << "\n";
	
	return 0;
}