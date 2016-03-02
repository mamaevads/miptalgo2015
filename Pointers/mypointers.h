#ifndef POINTERS_HPP_INCLUDED
#define POINTERS_HPP_INCLUDED

template<typename T>
class WeakPtr;

template<typename T>
class UniquePtr;

template<typename T>
class Pointer {
protected:
	T * ptr;
public:
	T& operator*() {
		return *ptr;
	}
	T* operator->() {
		return ptr;
	}
	T* get() {
		return ptr;
	}
	explicit operator bool() {
		return ptr != NULL;
	}
	bool operator<(const Pointer& other) {
		return ptr < other.ptr;
	}
	bool operator<=(const Pointer& other) {
		return ptr <= other.ptr;
	}
	bool operator>(const Pointer& other) {
		return ptr > other.ptr;
	}
	bool operator>=(const Pointer& other) {
		return ptr >= other.ptr;
	}
	bool operator==(const Pointer& other) {
		return ptr == other.ptr;
	}
	bool operator!=(const Pointer& other) {
		return ptr != other.ptr;
	}
};

class SharedPtrCounter {
protected:
	int cnt;
	int weak_ptr_cnt;
public:
	SharedPtrCounter(int cnt) : cnt(cnt) {
		weak_ptr_cnt = 0;
	}
	void increase() {
		++cnt;
	}
	void increase_weak() {
		++weak_ptr_cnt;
	}
	void decrease() {
		--cnt;
	}
	void deacrease_weak() {
		--weak_ptr_cnt;
	}
	bool expired() {
		return cnt == 0;
	}
	void release() {
		if (cnt == 0 && weak_ptr_cnt == 0)
			delete this;
	}
};

template<typename T>
class SharedPtr : public Pointer<T> {
	friend class WeakPtr<T>;
protected:
	typedef void(*deleter)(T*);
	SharedPtrCounter * cnt;
	deleter d;
	void destruct() {
		if (this->ptr) {
			cnt->decrease();
			if (cnt->expired()) {
				if (d)
					d(this->ptr);
				else
					delete this->ptr;
				cnt->release();
			}
			this->ptr = NULL;
		}
	}
public:
	T * release() {
		T * return_value = this->ptr;
		if (this->ptr) {
			cnt->decrease();
			cnt->release();
			this->ptr = NULL;
		}
		return return_value;
	}
	void acquire(T * normal_pointer, deleter pointer_deleter) {
		this->ptr = normal_pointer;
		if (normal_pointer)
			cnt = new SharedPtrCounter(1);
		d = pointer_deleter;
	}
	SharedPtr(T * normal_pointer = NULL, deleter pointer_deleter = NULL) {
		acquire(normal_pointer, pointer_deleter);
	}
	SharedPtr(const SharedPtr<T> &shared_pointer) {
		this->ptr = shared_pointer.ptr;
		cnt = shared_pointer.cnt;
		cnt->increase();
		d = shared_pointer.d;
	}
	SharedPtr(const WeakPtr<T> &weak_pointer) {
		this->ptr = weak_pointer.ptr;
		cnt = weak_pointer.cnt;
		cnt->increase();
		d = weak_pointer.d;
	}
	SharedPtr(UniquePtr<T> &unique_pointer) {
		this->ptr = unique_pointer.ptr;
		if (unique_pointer.ptr)
			cnt = new SharedPtrCounter(1);
		d = unique_pointer.d;
		unique_pointer.ptr = NULL;
	}
	~SharedPtr() {
		destruct();
	}
	SharedPtr& operator=(SharedPtr&& shared_pointer) {
		swap(this, shared_pointer);
		return *this;
	}
	void reset(T * normal_pointer, deleter pointer_deleter = NULL) {
		destruct();
		acquire(normal_pointer, pointer_deleter);
	}
	void swap(SharedPtr& shared_pointer) {
		std::swap(this->ptr, shared_pointer.ptr);
		std::swap(this->cnt, shared_pointer.cnt);
		std::swap(this->d, shared_pointer.d);
	}
	SharedPtr& operator=(const SharedPtr & shared_pointer) {
		this->ptr = shared_pointer.ptr;
		cnt = shared_pointer.cnt;
		cnt->increase();
		d = shared_pointer.d;
	}
};

template<typename T, typename Args>
SharedPtr<T> make_shared_ptr(Args&& arguments) {
	return SharedPtr<T>(new T(std::forward<Args>(arguments)));
}

template<typename T>
SharedPtr<T> make_shared_ptr() {
	return SharedPtr<T>(new T());
}

template<typename T>
class UniquePtr : public Pointer<T> {
	friend class SharedPtr<T>;
private:
	typedef void(*deleter)(T*);
	deleter d;
public:
	void destruct() {
		if (this->ptr) {
			if (d)
				d(this->ptr);
			else
				delete this->ptr;
			this->ptr = NULL;
		}
	}
	void release() {
		this->ptr = NULL;
	}
	void acquire(T * normal_pointer, deleter pointer_deleter) {
		this->ptr = normal_pointer;
		d = pointer_deleter;
	}
	UniquePtr(T * normal_pointer = NULL, deleter pointer_deleter = NULL) {
		acquire(normal_pointer, pointer_deleter);
	}
	UniquePtr(UniquePtr<T> &unique_pointer) {
		this->ptr = unique_pointer.ptr;
		d = unique_pointer.d;
		unique_pointer.ptr = NULL;
	}
	~UniquePtr() {
		destruct();
	}
	UniquePtr& operator=(UniquePtr&& unique_pointer) {
		swap(this, unique_pointer);
		return *this;
	}
	void reset(T * normal_pointer, deleter pointer_deleter = NULL) {
		destruct();
		acquire(normal_pointer, pointer_deleter);
	}
	void swap(UniquePtr& unique_pointer) {
		std::swap(this->ptr, unique_pointer.ptr);
		std::swap(this->d, unique_pointer.d);
	}
	UniquePtr& operator=(UniquePtr & shared_pointer) {
		this->ptr = shared_pointer.ptr;
		d = shared_pointer.d;
		shared_pointer.ptr = NULL;
	}
};

template<typename T, typename Args>
UniquePtr<T> make_unique_ptr(Args&& arguments) {
	return UniquePtr<T>(new T(std::forward<Args>(arguments)));
}

template<typename T>
UniquePtr<T> make_unique_ptr() {
	return UniquePtr<T>(new T());
}

template<typename T>
class WeakPtr :public Pointer<T> {
	friend class SharedPtr<T>;
protected:
	typedef void(*deleter)(T*);
	SharedPtrCounter * cnt;
	deleter d;
public:
	SharedPtr<T> lock() {
		if (this->ptr && !cnt->expired())
			return SharedPtr<T>(*this);
		else
			return SharedPtr<T>();
	}
	void acquire(const SharedPtr<T> & shared_pointer) {
		this->ptr = shared_pointer.ptr;
		cnt = shared_pointer.cnt;
		d = shared_pointer.d;
		cnt->increase_weak();
	}
	WeakPtr() {
		this->ptr = NULL;
	}
	WeakPtr(const SharedPtr<T> & shared_pointer) {
		acquire(shared_pointer);
	}
	void release() {
		if (cnt) {
			cnt->deacrease_weak();
			cnt->release();
		}
	}
	void swap(WeakPtr & weak_pointer) {
		swap(this->cnt, weak_pointer.cnt);
		swap(this->d, weak_pointer.d);
		swap(this->cnt, weak_pointer.d);
	}
	void reset(const SharedPtr<T> & shared_pointer) {
		release();
		acquire(shared_pointer);
	}
	WeakPtr<T>& operator=(const SharedPtr<T> shared_pointer) {
		acquire(shared_pointer);
		return *this;
	}
	~WeakPtr() {
		release();
	}
};



#endif // POINTERS_HPP_INCLUDED