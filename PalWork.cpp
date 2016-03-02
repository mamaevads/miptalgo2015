#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;
const int MAXLENS = 5;
const int MAXN = 100;

int convertFromBinary(const vector<int>& digits) {
	int p = 1, res = 0;
	for (int c : digits)
	{
		res += c * p;
		p *= 2;
	}
	return res;
}

int main() {
#ifdef HOME
	freopen("input.txt", "r", stdin);
#endif
	int n; string in;
	cin >> n >> in;
	if (in.length() == 1) {
		cout << (n >= 1);
		return 0;
	}
	int m = (n + 1) / 2;
	int suffLen = min((int)in.length() - 1, m);
	int numMasks = 1 << suffLen;
	vector<vector<long long>> d(2, vector<long long>(numMasks));
	for (int msk = 0; msk < numMasks; msk++)
		d[0][msk] = 1;

	set<int> banned;
	vector<int> digits;
	for (char symb : in)
		digits.push_back(symb - 'A');
	banned.insert(convertFromBinary(digits));
	reverse(digits.begin(), digits.end());
	banned.insert(convertFromBinary(digits));

	for (int len = suffLen + 1; len <= m; len++) {
		for (int msk = 0; msk < numMasks; msk++)
			for (int prevDigit = 0; prevDigit < 2; prevDigit++) {
				long long prevMsk = prevDigit + ((msk << 1) & (numMasks - 1));
				long long allMsk = (msk << 1) + prevDigit;
				if (!banned.count(allMsk)) {
					d[1][msk] += d[0][prevMsk];
				}
			}
		swap(d[0], d[1]);
		d[1].assign(d[0].size(), 0);
	}
	long long ans = 0;
	for (int leftMsk = 0; leftMsk < numMasks; leftMsk++) {
		string left;
		int msk = leftMsk;
		for (int i = 0; i < suffLen; i++)
		{
			left += char('A' + msk % 2);
			msk /= 2;
		}
		string right = left;
		if (n % 2 && suffLen)
			right.pop_back();
		reverse(right.begin(), right.end());
		string medium = left + right;
		bool ok = (medium.find(in) == string::npos);
		reverse(medium.begin(), medium.end());
		ok &= (medium.find(in) == string::npos);
		if (ok) {
			ans += d[0][leftMsk];
		}
	}
	cout << ans << endl;
	system("pause");
	return 0;
}
