#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;
const int MAXLENS = 5;
const int MAXN = 100;
int d[MAXN/2 + 1][1 << MAXLENS];

int convertFromBinary(const vector<int>& digits) {
    int p = 1, res = 0;
    for (int c: digits)
    {
        res += c * p;
        p *= 2;
    }
    return res;
}

int main() {
#ifdef HOME
#endif
    int n; string in;
    cin >> n >> in;
    if (in.length() == 1) {
        cout << (n >= 1);
        return 0;
    }
    int m = n / 2 + n % 2;
    int k = min((int) in.length() - 1, m);
    int numMasks = 1 << k;
    for (int msk = 0; msk < numMasks; msk++)
        d[k][msk] = 1;

    set<int> banned;
    vector<int> digits;
    for (char symb: in)
        digits.push_back(symb - 'A');
    banned.insert(convertFromBinary(digits));
    reverse(digits.begin(), digits.end());
    banned.insert(convertFromBinary(digits));

    for (int len = k + 1; len <= m; len++) 
        for (int msk = 0; msk < numMasks; msk++)
                for (int prevDigit = 0; prevDigit < 2; prevDigit++) {
                    int prevMsk = prevDigit + ((msk << 1) & (numMasks - 1));
                    int allMsk = (msk << 1) + prevDigit;
                    if (!banned.count(allMsk)) {
                        d[len][msk] += d[len - 1][prevMsk];
                        cerr << d[len][msk] << endl;
                    }
                }
    int ans = 0;
    for (int leftMsk = 0; leftMsk < numMasks; leftMsk++) {
        string left;
        int msk = leftMsk;
        for (int i = 0; i < k; i++)
        {
            left += char('A' + msk % 2);
            msk /= 2;
        }
        string right = left;
        if (n % 2 && k)
            right.pop_back();
        reverse(right.begin(), right.end());
        string medium = left + right;
        bool ok = (medium.find(in) == string::npos);
        reverse(medium.begin(), medium.end());
        ok &= (medium.find(in) == string::npos);
        if (ok) {
            ans += d[m][leftMsk];
        }
    }
    cout << ans << endl;
    return 0;
}
