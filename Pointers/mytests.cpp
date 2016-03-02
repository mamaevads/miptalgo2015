#include <iostream>
#include <vector>
#include "pointers.h"
#include <boost/test/minimal.hpp>

using namespace std;

bool deleted_test_class;

class TestClass {
public:
	TestClass() {};
	~TestClass() {
		deleted_test_class = true;
	};
};

void check_shared_ptr() {
	deleted_test_class = false;
	SharedPtr<TestClass> * ptr1 = new SharedPtr<TestClass>(new TestClass());
	BOOST_CHECK(!deleted_test_class);
	SharedPtr<TestClass> * ptr2 = new SharedPtr<TestClass>();
	BOOST_CHECK(ptr2->get() == NULL);
	(*ptr2) = (*ptr1);
	BOOST_CHECK(ptr1->get() == ptr2->get());
	ptr1->release();
	BOOST_CHECK(ptr1->get() == NULL);
	BOOST_CHECK(!deleted_test_class);
	delete ptr1;
	BOOST_CHECK(!deleted_test_class);
	delete ptr2;
	BOOST_CHECK(deleted_test_class);
	UniquePtr<int> unique_pointer(new int(42));
	SharedPtr<int> shared_pointer(unique_pointer);
	BOOST_CHECK(unique_pointer.get() == NULL);
}

void check_unique_ptr() {
	deleted_test_class = false;
	UniquePtr<TestClass> * ptr1 = new UniquePtr<TestClass>(new TestClass());
	BOOST_CHECK(!deleted_test_class);
	UniquePtr<TestClass> * ptr2 = new UniquePtr<TestClass>();
	BOOST_CHECK(ptr2->get() == NULL);
	(*ptr2) = (*ptr1);
	BOOST_CHECK(!deleted_test_class);
	BOOST_CHECK(ptr1->get() == NULL);
	delete ptr1;
	BOOST_CHECK(!deleted_test_class);
	delete ptr2;
	BOOST_CHECK(deleted_test_class);
}

void check_weak_ptr() {
	deleted_test_class = false;
	SharedPtr<TestClass> shared_pointer = make_shared_ptr<TestClass>();
	WeakPtr<TestClass> weak_pointer = shared_pointer;
	BOOST_CHECK(!deleted_test_class);
	SharedPtr<TestClass> tmp = weak_pointer.lock();
	BOOST_CHECK(!deleted_test_class);
	shared_pointer.reset(NULL);
	BOOST_CHECK(!deleted_test_class);
	tmp.reset(NULL);
	BOOST_CHECK(deleted_test_class);
	BOOST_CHECK(weak_pointer.lock().get() == NULL);
}

int test_main(int, char **)
{
	check_unique_ptr();
	check_shared_ptr();
	check_weak_ptr();
	return 0;
}