#include "methods.h"

void buildAll(string & str, vector<int> & sufAr, vector<int> & LCP) {
	int n = str.length();
	vector<int> newstr(n + 3);
	vector<int> newsufar(n + 3);
	for (int i = 0; i < n; i++)
		newstr[i] = str[i];
	getSufAr(newstr, newsufar, n, 256);

	for (int i = 0; i < n; i++)
		sufAr[i] = newsufar[i];
	getLCP(newstr, sufAr, LCP);
}


void getLCP(vector<int> & str, const vector<int> & sufAr, vector<int> & LCP){
	int n = sufAr.size();
	vector<int> rank(n);
	str[n] = -1;
	for (int i = 0; i < n; i++)
		rank[sufAr[i]] = i;
	int len= 0;
	for (int i = 0; i < n; i++)
		if (rank[i] > 0){
			int j = sufAr[rank[i] - 1];
			while (str[i + len] == str[j + len]) len++;
			LCP[rank[i]] = len;
			if (len > 0) len--;
		}
}

void getSufAr(const vector<int> & str, vector<int> & sufAr, int n, int K){
	int n0 = (n + 2) / 3;
	int n1 = (n + 1) / 3;
	int n2 = n / 3;
	int t = n0 - n1;  
	int n12 = n1 + n2 + t;

	vector<int> str12(n12 + 3);
	vector<int> sufAr12(n12 + 3);
	vector<int> str0(n0);
	vector<int> sufAr0(n0);

	for (int i = 0, j = 0; i < n + t; i++)
		if (i % 3 != 0)
			str12[j++] = i;

	int K12 = rename(str, str12, sufAr12, n0, n12, K);

	solveOneTwo(str12, sufAr12, n12, K12);
	solveZero(str, str0, sufAr0, sufAr12, n0, n12, K);
	merge(str, str12, sufAr, sufAr0, sufAr12, n, n0, n12, t);
}



int rename(const vector<int> & s, vector<int> & str12, vector<int> & sufAr12, int n0, int n12, int K){

	goRadix(str12, sufAr12, s, 2, n12, K);
	goRadix(sufAr12, str12, s, 1, n12, K);
	goRadix(str12, sufAr12, s, 0, n12, K);

	int name = 0;
	int c0 = -1, c1 = -1, c2 = -1;

	for (int i = 0; i < n12; i++){
		if (s[sufAr12[i]] != c0 || s[sufAr12[i] + 1] != c1 || s[sufAr12[i] + 2] != c2){
			name++;
			c0 = s[sufAr12[i]];
			c1 = s[sufAr12[i] + 1];
			c2 = s[sufAr12[i] + 2];
		}

		if (sufAr12[i] % 3 == 1)
			str12[sufAr12[i] / 3] = name;   
		else
			str12[sufAr12[i] / 3 + n0] = name;   
	}
	return name;
}

void goRadix(const vector<int> & in, vector<int> & out, const vector<int> & s, int n, int K) {
	goRadix(in, out, s, 0, n, K);
}

void goRadix(const vector<int> & in, vector<int> & out, const vector<int> & s, int offset, int n, int K){
	vector<int> count(K + 2);                  
	for (int i = 0; i < n; i++)
		count[s[in[i] + offset] + 1]++;   
	for (int i = 1; i <= K + 1; i++)
		count[i] += count[i - 1];
	for (int i = 0; i < n; i++)
		out[count[s[in[i] + offset]]++] = in[i];      
}

void solveOneTwo(vector<int> & str12, vector<int> & sufAr12, int n12, int K12){
	if (K12 == n12) 
		for (int i = 0; i < n12; i++)
			sufAr12[str12[i] - 1] = i;
	else{
		getSufAr(str12, sufAr12, n12, K12);
		for (int i = 0; i < n12; i++)
			str12[sufAr12[i]] = i + 1;
	}
}

void solveZero(const vector<int> & str, vector<int> & str0, vector<int> & sufAr0, const vector<int> & sufAr12, int n0, int n12, int K){
	for (int i = 0, j = 0; i < n12; i++)
		if (sufAr12[i] < n0)
			str0[j++] = 3 * sufAr12[i];

	goRadix(str0, sufAr0, str, n0, K);
}


void merge(const vector<int> & str, const vector<int> & str12, vector<int> & sufAr, const vector<int> & sufAr0,
	const vector<int> & sufAr12, int n, int n0, int n12, int t){
	int p = 0, k = 0;

	while (t != n12 && p != n0){
		int i = getIndex(sufAr12, t, n0); 
		int j = sufAr0[p];                     

		if (sufComp(str, str12, sufAr12, n0, i, j, t)){
			sufAr[k++] = i;
			t++;
		}
		else{
			sufAr[k++] = j;
			p++;
		}
	}
	while (p < n0)
		sufAr[k++] = sufAr0[p++];
	while (t < n12)
		sufAr[k++] = getIndex(sufAr12, t++, n0);
}

int getIndex(const vector<int> & sufAr12, int t, int n0){
	if (sufAr12[t] < n0)
		return sufAr12[t] * 3 + 1;
	else
		return (sufAr12[t] - n0) * 3 + 2;
}

bool leq(int a1, int a2, int b1, int b2) {
	return a1 < b1 || a1 == b1 && a2 <= b2;
}

bool leq(int a1, int a2, int a3, int b1, int b2, int b3) {
	return a1 < b1 || a1 == b1 && leq(a2, a3, b2, b3);
}

bool sufComp(const vector<int> & s, const vector<int> & str12, const vector<int> & sufAr12, int n0, int i, int j, int t) {
	if (sufAr12[t] < n0)
		return leq(s[i], str12[sufAr12[t] + n0], s[j], str12[j / 3]);
	else
		return leq(s[i], s[i + 1], str12[sufAr12[t] - n0 + 1], s[j], s[j + 1], str12[j / 3 + n0]);
}


int main(){
	int k;
	cin >> k;
    string str;
	cin >> str;
	vector<int> sufarr(k);
	vector<int> LCP(k);
	str = str + str.substr(0, k-1);
	int n = str.length();
	for (int i = 0; i < n - k + 1; i++) {
		string temp = str.substr(i, k);
		buildAll(temp, sufarr, LCP);
		long long result = (k*(k + 1)) / 2;
		for (int j = 0; j < k; j++)
			result = result - LCP[j];
		cout << result<<" ";

		sufarr.assign(k,0);
		LCP.assign(k, 0);
	}

	//system("pause");
	return 0;
}



