//#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <stdio.h>

using namespace std;

default_random_engine generator;

int generateRandom(int minx, int maxx) {
	uniform_int_distribution<int> distribution(minx, maxx);
	return distribution(generator);
}

class Hash {

public:
	static const unsigned long long MOD = 2000000011;
	unsigned long long a;
	unsigned long long b;
	int sizeOfTable;

public:
	void newAB()
	{
		a = generateRandom(1, MOD - 1);
		b = generateRandom(0, MOD - 1);
	}

	void setSize(int size)
	{
		sizeOfTable = size;
	}

	long long operator()(int key) const
	{
		return (((a * key)  + b) % MOD) % sizeOfTable; 
	}
};


class InnerSet {

private:
	Hash hash;

	int sizeOfSet;
	vector <bool> set;
	vector <int> val;

	bool isBad(const vector<int> &numbers) {
		sizeOfSet = numbers.size() * numbers.size();
		hash.setSize(sizeOfSet);
		set.assign(sizeOfSet, false);
		val.assign(sizeOfSet, 0);
		for (auto const &element : numbers) {
			int cur = hash(element);
			if (set[cur]) {
				return true;
			}
			val[cur] = element;
			set[cur] = true;
		}
		return false;
	}

public:

	explicit InnerSet(const vector<int> &numbers) {
		do {
			hash.newAB();
		} while (isBad(numbers));
		val.resize(sizeOfSet * sizeOfSet);
		for (auto const &element : numbers) {
			val[hash(element)] = element;
		}
	}

	bool Contains(int element) const {
		return sizeOfSet && set[hash(element)] && val[hash(element)] == element;
	}
};

class FixedSet {

private:
	Hash hash;
	vector <InnerSet> InnerSets;
	int sizeOfTable;

	bool isBad(const vector<int> &numbers, vector <vector <int> >& innernumbers) {
		sizeOfTable = numbers.size();
		hash.setSize(sizeOfTable);
		innernumbers.assign(sizeOfTable, vector<int>());
		for (auto const &element : numbers) {
			innernumbers[hash(element)].push_back(element);
		}
		long long squareSetSize = 0;
		for (auto const &tablenumbers : innernumbers) {
			squareSetSize += (long long)tablenumbers.size() * (long long)tablenumbers.size();
		}
		return (squareSetSize > 4 * sizeOfTable);
	}

public:

	explicit FixedSet(const vector <int> &numbers) {
		vector <vector <int> > innernumbers;
		do {
			hash.newAB();
		} while (isBad(numbers, innernumbers));
		InnerSets.clear();

		for (auto const &innerElement : innernumbers) {
			InnerSets.push_back(InnerSet(innerElement));
		}
	}

	bool Contains(int number) const {
		return InnerSets[hash(number)].Contains(number);
	}
};

int main() {
	std::ios_base::sync_with_stdio(false);
	//freopen("test21-00006.txt", "r", stdin);
	//freopen("out3.txt", "w", stdout);
	int n, m, now;
	cin >> n;
	vector <int> numbers(n);
	for (size_t i = 0; i < n; i++) {
		cin >> numbers[i];
	}
	cin >> m;
	vector<bool> answers(m);
	FixedSet helper = FixedSet(numbers);
	for (size_t i = 0; i < m; i++) {
		cin >> now;
		answers[i] = helper.Contains(now);
	}
	for (size_t i = 0; i < m; i++) {
		cout << (answers[i] ? "Yes\n" : "No\n");
	}
}