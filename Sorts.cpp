#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;


double fRand(double fMin, double fMax){
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}
template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type >>
void insertionSort(Iterator first, Iterator last, Comparator comparator = Comparator()){
	if (last - first < 0)
		return;
	Iterator minimum = first;
	for (Iterator i = first + 1; i < last; ++i)
		if (*i < *minimum)
			minimum = i;
	iter_swap(first, minimum);
	while (++first < last)
		for (Iterator j = first; *j < *(j - 1); j--)
			iter_swap(j - 1, j);

}
template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type >>
void goInsert(Iterator first, Iterator last, Comparator comparator = Comparator()){
	insertionSort(first, last, comparator); 
	
}
template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type >>
void quickSort(Iterator first, Iterator last, Comparator comparator = Comparator()) {
	if (last - first > 0) {
		auto pivot = *(first + (last - first) / 2);
		Iterator left(first);
		Iterator right(last);
		while (left <= right) {
			while (*left < pivot) ++left;
			while (pivot < *right) --right;
			if (left <= right) {
				swap(*left, *right);
				++left;
				--right;
			}
		}
		quickSort(first, right, comparator);
		quickSort(left, last, comparator);
	}
}
template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type >>
void goQuick(Iterator first, Iterator last, Comparator comparator = Comparator()) {
	quickSort(first, last - 1, comparator);
}
template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type >>
void heapSort(Iterator first, Iterator last, Comparator comparator = Comparator()){
	if (last - first < 0)
		return;
	make_heap(first, last, comparator);
	for (Iterator i = last; i != first; i--)
		pop_heap(first, i, comparator);
}
template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type >>
void goHeap(Iterator first, Iterator last, Comparator comparator = Comparator()) {
	heapSort(first, last, comparator);
}
template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type >>
void selectionSort(Iterator first, Iterator last, Comparator comparator = Comparator()){
	if (last - first < 0)
		return;
	Iterator minimum;
	while (first != last)
	{
		minimum = min_element(first, last);
		iter_swap(first, minimum);
		first++;
	}
}
template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type >>
void goSelect(Iterator first, Iterator last, Comparator comparator = Comparator()) {
	selectionSort(first, last, comparator);
}


int main(){

	vector<int> test11;
	for (size_t i = 10; i < 10000; i = i * 10){
		if (!test11.empty()) test11.clear();
		test11.resize(i);
		for (size_t j = 0; j < i; j++){
			test11[j] = rand() % 10;
		}
		vector<int> test12(test11);
		sort(test12.begin(),test12.end());
		vector<int> test13(test11);
		goQuick(test13.begin(), test13.end());
		for (size_t j = 0; j < i; j++){
			if (test12[j] != test13[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "IF NO ERROR QUICK: " << i << "ELEMENTS: TESTS OK" << endl;
		
		test13 = test11;
		goSelect(test13.begin(), test13.end());
		for (size_t j = 0; j < i; j++){
			if (test12[j] != test13[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout <<"IF NO ERROR SELECTION: "<< i << "ELEMENTS: TESTS OK" << endl;

		test13 = test11;
		goInsert(test13.begin(), test13.end());
		for (size_t j = 0; j < i; j++){
			if (test12[j] != test13[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "IF NO ERROR INSERTION: " << i << "ELEMENTS: TESTS OK" << endl;

		test13 = test11;
		goHeap(test13.begin(), test13.end());
		for (size_t j = 0; j < i; j++){
			if (test12[j] != test13[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "IF NO ERROR HEAP: " << i << "ELEMENTS: TESTS OK" << endl;

	}

	deque<int> test21;
	for (size_t i = 10; i < 10000; i = i * 10){
		if (!test21.empty()) test11.clear();
		test21.resize(i);
		for (size_t j = 0; j < i; j++){
			test11.push_back(fRand(0,10));
		}
		deque<int> test22(test21);
		sort(test22.begin(), test22.end());
		deque<int> test23(test21);
		goQuick(test23.begin(), test23.end());
		for (size_t j = 0; j < i; j++){
			if (test22[j] != test23[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "QUICK: " << i << "ELEMENTS: TESTS OK" << endl;

		test23 = test21;
		goSelect(test23.begin(), test23.end());
		for (size_t j = 0; j < i; j++){
			if (test22[j] != test23[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "SELECTION: " << i << "ELEMENTS: TESTS OK" << endl;

		test23 =test21;
		goInsert(test23.begin(), test23.end());
		for (size_t j = 0; j < i; j++){
			if (test22[j] != test23[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "INSERTION: " << i << "ELEMENTS: TESTS OK" << endl;

		test23 = test21;
		goHeap(test23.begin(), test23.end());
		for (size_t j = 0; j < i; j++){
			if (test22[j] != test23[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "HEAP: " << i << "ELEMENTS: TESTS OK" << endl;

	}

	vector<double> test31;
	for (size_t i = 10; i < 10000; i = i * 10){
		if (!test31.empty()) test31.clear();
		test31.resize(i);
		for (size_t j = 0; j < i; j++){
			test11.push_back(rand() % 10);
		}
		vector<double> test32(test31);
		sort(test32.begin(), test32.end());
		vector<double> test33(test31);
		goQuick(test33.begin(), test33.end());
		for (size_t j = 0; j < i; j++){
			if (test32[j] != test33[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "QUICK: " << i << "ELEMENTS: TESTS OK" << endl;

		test33 = test31;
		goSelect(test33.begin(), test33.end());
		for (size_t j = 0; j < i; j++){
			if (test32[j] != test33[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "SELECTION: " << i << "ELEMENTS: TESTS OK" << endl;

		test33 = test31;
		goInsert(test33.begin(), test33.end());
		for (size_t j = 0; j < i; j++){
			if (test32[j] != test33[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "INSERTION: " << i << "ELEMENTS: TESTS OK" << endl;

		test33 = test31;
		goHeap(test33.begin(), test33.end());
		for (size_t j = 0; j < i; j++){
			if (test32[j] != test33[j]) {
				cout << "ERROR";
				break;
			}
		}
		cout << "HEAP: " << i << "ELEMENTS: TESTS OK" << endl;

	}

	return 0;
}