//! \file StringABI.h.
//!
//! \brief Declares the string ABI

#ifndef NIRVANA_ORB_STRINGABI_H_
#define NIRVANA_ORB_STRINGABI_H_

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/real_copy.h>

namespace CORBA {
namespace Nirvana {

template <typename C> class String_var;

template <typename C>
class StringABI
{
public:
	StringABI ()
	{}

	StringABI (StringABI&& src)
	{
		data_ = src.data_;
		src.reset ();
	}

	static size_t max_size ()
	{
		return (SIZE_MAX / 2 + 1) / sizeof (C) - 1;
	}

	size_t size () const
	{
		if (is_large ())
			return large_size ();
		else
			return small_size ();
	}

	size_t capacity () const
	{
		if (is_large ())
			return large_capacity ();
		else
			return SMALL_CAPACITY;
	}

	bool empty () const
	{
		return size () == 0;
	}

	void _marshal (StringABI& dst) const;
	void _adopt () const;

protected:
	const C* _ptr () const
	{
		if (is_large ())
			return large_pointer ();
		else
			return small_pointer ();
	}

	C* _ptr ()
	{
		if (is_large ())
			return large_pointer ();
		else
			return small_pointer ();
	}

	void reset ()
	{
		std::fill_n (data_.raw, countof (data_.raw), 0);
	}

	bool is_large () const
	{
		return (data_.small.size & SMALL_MASK) != 0;
	}

	void small_size (size_t s)
	{
		assert (s <= SMALL_CAPACITY);
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			data_.small.size = (unsigned char)(s << 1);
		else
			data_.small.size = (unsigned char)(s);
		assert (!is_large ());
	}

	size_t small_size () const
	{
		assert (!is_large ());
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			return data_.small.size >> 1;
		else
			return data_.small.size;
	}

	void large_size (size_t s)
	{
		data_.large.size = s;
	}

	size_t large_size () const
	{
		assert (is_large ());
		return data_.large.size;
	}

	void allocated (size_t cb)
	{
		assert (!(cb & 2));
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			data_.large.allocated = LARGE_MASK | cb;
		else
			data_.large.allocated = LARGE_MASK | (cb >> 1) | (cb & 1);
		assert (is_large ());
	}

	size_t allocated () const
	{
		size_t space = data_.large.allocated;
		if ((space & (LARGE_MASK | 1)) == LARGE_MASK) {
			if (::Nirvana::endian::native == ::Nirvana::endian::big)
				return space & ~LARGE_MASK;
			else
				return space << 1;
		} else
			return 0;
	}

	bool is_constant_allocated () const
	{
		return (data_.large.allocated & (LARGE_MASK | 1)) == (LARGE_MASK | 1);
	}

	size_t large_capacity () const
	{
		assert (is_large ());
		size_t space = data_.large.allocated & ~(LARGE_MASK | 1);
		if (::Nirvana::endian::native == ::Nirvana::endian::little)
			space <<= 1;
		return space / sizeof (C) - 1;
	}

	void large_pointer (C* p)
	{
		data_.large.data = p;
	}

	const C* large_pointer () const
	{
		return data_.large.data;
	}

	C* large_pointer ()
	{
		return data_.large.data;
	}

	const C* small_pointer () const
	{
		return data_.small.data;
	}

	C* small_pointer ()
	{
		return data_.small.data;
	}

private:
	static const unsigned char SMALL_MASK = ::Nirvana::endian::native == ::Nirvana::endian::big ? 0x02 : 0x80;
	static const size_t LARGE_MASK = ::Nirvana::endian::native == ::Nirvana::endian::big ? 2 : ~(size_t (~0) >> 1);

#pragma pack (push, 1)

	struct Large
	{
		C* data;
		size_t size;
		size_t allocated;
	};

	struct Small
	{
		union
		{
			unsigned char padding [sizeof (Large) - sizeof (unsigned char)];
			C data [sizeof (padding) / sizeof (C)];
		};
		unsigned char size;
	};

	static_assert (sizeof (Large) == sizeof (Small), "sizeof (Large) == sizeof (Small)");

protected:
	static const size_t SMALL_CAPACITY = sizeof (Small::data) / sizeof (C) - 1;

	union ULS
	{
		Large large;
		Small small;
	};

	union Data
	{
		Small small;
		Large large;
		size_t raw [sizeof (ULS) / sizeof (size_t)];
	};

#pragma pack (pop)

	friend class String_var <C>;

protected:
	Data data_;
};

}
}

#endif
