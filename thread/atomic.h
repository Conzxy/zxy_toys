/*
 * @version: 0.1 2021-5-28
 * @author: Conzxy
 * atomic counter
 */

#ifndef _ATOMIC_H
#define _ATOMIC_H

#include <type_traits>
#include <noncopyable.h>
#include <stdint.h>

namespace zxy{

template<typename T>
class Atomic : public noncopyable {
public:
	static_assert(std::is_integral<T>::value, "Atomic type must be integral\n");

	Atomic()
		: val_{0}
	{}

	T get() noexcept {
		return __atomic_load_n(&val_, __ATOMIC_SEQ_CST);	
	}
	
	T getAndAdd(T x) noexcept {
		return __atomic_fetch_add(&val_, x, __ATOMIC_SEQ_CST);
	}

	T addAndGet(T x) noexcept {
		return getAndAdd(x) + x;
	}

	T incrementAndGet() noexcept {
		return addAndGet(1);
	}
	
	T decrementAndGet() noexcept {
		return addAndGet(-1);
	}

	void add(T x) noexcept {
		getAndAdd(x);
	}

	void increment() noexcept {
		getAndAdd(1);
	}

	void decrement() noexcept {
		getAndAdd(-1);
	}

	T getAndSet(T x) noexcept {
		return __atomic_exchange_n(&val_, x, __ATOMIC_SEQ_CST);
	}

private:
	volatile T val_;
};

using AtomicInt32 = Atomic<int32_t>;
using AtomicInt64 = Atomic<int64_t>;

}//namespace zxy
#endif //_ATOMIC_H
