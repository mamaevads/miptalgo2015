//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

struct node {
	node *l, *r;
	int prior;
	char val;
	size_t cnt;

	node() {
		l = r = NULL;
		cnt = 1;
		prior = rand();
	}

	explicit node(char c) {
		val = c;
		cnt = 1;
		prior = rand();
		l = r = NULL;
	}

};

typedef node* pnode;

int getCnt(pnode t) {
	if (t == NULL)
		return 0;
	else
		return t->cnt;
}

void update(pnode t) {
	if (t != NULL)
		t->cnt = 1 + getCnt(t->l) + getCnt(t->r);
}

void merge(pnode &t, pnode l, pnode r) {
	if (l == NULL) {
		t = r;
	}
	else if (r == NULL) {
		t = l;
	}
	else {
		if (l->prior > r->prior) {
			merge(l->r, l->r, r);
			t = l;
		}
		else {
			merge(r->l, l, r->l);
			t = r;
		}
	}
	update(t);
}

void split(pnode t, pnode &l, pnode &r,size_t key) {
	if (t == NULL) {
		l = r = NULL;
	}
	else {
		if (key <= getCnt(t->l)) {
			split(t->l, l, t->l, key);
			r = t;
		}
		else {
			split(t->r, t->r, r, key - getCnt(t->l) - 1);
			l = t;
		}
	}
	update(t);
}

pnode root = NULL;

void print(pnode t, std::ostream &out) {
	if (t == NULL)
		return;
	print(t->l,out);
	out << t->val;
	print(t->r,out);
}

void shift(size_t l, size_t r, size_t s) {
	pnode t1, t2, t3;
	t1 = t2 = t3 = NULL;

	split(root, t2, t3, r + 1);
	split(t2, t1, t2, l);

	pnode tl, tr;
	tl = tr = NULL;
	split(t2, tl, tr, r - l + 1 - s);

	merge(root, t1, tr);
	merge(root, root, tl);
	merge(root, root, t3);
}

void build(const string &s) {
	for (size_t i = 0; i < s.length(); i++) {
		pnode it = new node(s[i]);
		merge(root, root, it);
	}
}

int main() {
	//freopen("test05-00020.txt", "r", stdin);
	//freopen("out20.txt", "w", stdout);
	string s;
	cin >> s;
	build(s);
	size_t queries;
	cin >> queries;
	for (int i = 0; i < queries; i++) {
		int l, r, s;
		cin >> l >> r >> s;
		shift(l - 1, r - 1, s);
	}
	print(root,cout);
	cout << endl;
}