#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

inline ptrdiff_t fenwick_prev(ptrdiff_t i){
	return (i & (i + 1));
}

inline ptrdiff_t fenwick_next(ptrdiff_t i){
	return (i | (i + 1));
}
int sum(vector<vector<vector<int> > > & mas, ptrdiff_t x, ptrdiff_t y, ptrdiff_t z)
{
	int result = 0;
	for (ptrdiff_t i = x; i >= 0; i = (fenwick_prev(i) - 1))
		for (ptrdiff_t j = y; j >= 0; j = (fenwick_prev(j) - 1))
			for (ptrdiff_t k = z; k >= 0; k = (fenwick_prev(k) - 1))
				result += mas[i][j][k];
	return result;
}

void inc(vector<vector<vector<int> > > & mas, ptrdiff_t n, ptrdiff_t x, ptrdiff_t y, ptrdiff_t z, int kol)
{
	for (ptrdiff_t i = x; i < n; i = fenwick_next(i))
		for (ptrdiff_t j = y; j < n; j = fenwick_next(j))
			for (ptrdiff_t k = z; k < n; k = fenwick_next(k))
				mas[i][j][k] += kol;
}
int main(){
	ptrdiff_t n;
	cin >> n;
	vector<vector<vector<int> > >  mas(n, vector<vector<int>>(n, vector<int>(n)));
	int m;
	cin >> m;
	while (m != 3){
		if (m == 1){
			ptrdiff_t x, y, z;
			int k;
			cin >> x >> y >> z >> k;
			inc(mas, n, x, y, z, k);
		}
		else {
			ptrdiff_t x1, y1, z1, x2, y2, z2;
			cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
			cout << sum(mas, x2, y2, z2) - sum(mas, x2, y2, z1 - 1) - sum(mas, x2, y1 - 1, z2) - sum(mas, x1 - 1, y2, z2)
				+ sum(mas, x1 - 1, y1 - 1, z2) + sum(mas, x1 - 1, y2, z1 - 1) + sum(mas, x2, y1 - 1, z1 - 1) - sum(mas, x1 - 1, y1 - 1, z1 - 1) << "\n";
		}
		cin >> m;
	}
	return 0;
}