// Mock for atomic core counter
#ifndef NIRVANA_CORE_ATOMICCOUNTER_H_
#define NIRVANA_CORE_ATOMICCOUNTER_H_

#include <Nirvana/Nirvana.h>
#include <stdint.h>

namespace Nirvana {
namespace Core {

// Emulate core AtomicCounter synopsis.
// Test is always sinlge-threaded, so we don't need atomic counting.
class AtomicCounter
{
public:
	typedef uint32_t UIntType;
	typedef int32_t IntType;

	AtomicCounter (uint32_t init) :
		cnt_ (init)
	{}

	operator uint32_t () const
	{
		return cnt_;
	}

	uint32_t increment ()
	{
		return ++cnt_;
	}

	uint32_t decrement ()
	{
		return --cnt_;
	}

private:
	volatile uint32_t cnt_;
};

//! Reference counter always initialized to 1 and can't be incremented from zero.
class RefCounter : public AtomicCounter
{
public:
	RefCounter () :
		AtomicCounter (1)
	{}

	RefCounter (const RefCounter&) :
		AtomicCounter (1)
	{}

	RefCounter& operator = (const RefCounter&)
	{
		return *this;
	}

	uint32_t decrement ()
	{
		assert ((UIntType)*this > 0);
		return AtomicCounter::decrement ();
	}

	uint32_t increment ()
	{
		uint32_t ret = AtomicCounter::increment ();
		assert (ret > 1);
		return ret;
	}
};

}
}

#endif
