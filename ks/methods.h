#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>

using namespace std;

void buildAll(string & str, vector<int> & sufAr, vector<int> & LCP);
void getLCP(vector<int> & str, const vector<int> & sufAr, vector<int> & LCP);
void getSufAr(const vector<int> & str, vector<int> & sufAr, int n, int K);
int rename(const vector<int> & s, vector<int> & str12, vector<int> & sufAr12, int n0, int n12, int K);
void goRadix(const vector<int> & in, vector<int> & out, const vector<int> & s, int n, int K);
void goRadix(const vector<int> & in, vector<int> & out, const vector<int> & s, int offset, int n, int K);
void solveOneTwo(vector<int> & str12, vector<int> & sufAr12, int n12, int K12);
void solveZero(const vector<int> & str, vector<int> & str0, vector<int> & sufAr0, const vector<int> & sufAr12, int n0, int n12, int K);
void merge(const vector<int> & str, const vector<int> & str12, vector<int> & sufAr, const vector<int> & sufAr0,
	const vector<int> & sufAr12, int n, int n0, int n12, int t);
int getIndex(const vector<int> & sufAr12, int t, int n0);
bool leq(int a1, int a2, int b1, int b2);
bool leq(int a1, int a2, int a3, int b1, int b2, int b3);
bool sufComp(const vector<int> & s, const vector<int> & str12, const vector<int> & sufAr12, int n0, int i, int j, int t);
