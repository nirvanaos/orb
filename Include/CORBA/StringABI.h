//! \file StringABI.h.
//!
//! \brief Declares the string ABI
//! 	This file mustn't be changed.

#ifndef NIRVANA_ORB_STRINGABI_H_
#define NIRVANA_ORB_STRINGABI_H_

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/real_copy.h>

namespace CORBA {
namespace Nirvana {

template <typename C>
class StringABI
{
public:
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

	size_t allocated () const
	{
		if (is_large ())
			return large_allocated ();
		else
			return 0;
	}

	bool empty () const
	{
		return size () == 0;
	}

	void marshal (StringABI& dst) const;

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
		return data_.small.size & SMALL_MASK;
	}

	void small_size (size_t s)
	{
		assert (s <= SMALL_CAPACITY);
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			data_.small.size = (unsigned char)(s << 1);
		else
			data_.small.size = (unsigned char)(s);
	}

	size_t small_size () const
	{
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
		return data_.large.size;
	}

	void large_allocated (size_t cb)
	{
		assert (!(cb & 1));
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			data_.large.allocated = LARGE_MASK | cb;
		else
			data_.large.allocated = LARGE_MASK | (cb >> 1);
	}

	size_t large_allocated () const
	{
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			return data_.large.allocated & ~LARGE_MASK;
		else
			return data_.large.allocated << 1;
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
	static const unsigned char SMALL_MASK = ::Nirvana::endian::native == ::Nirvana::endian::big ? 0x01 : 0x80;
	static const size_t LARGE_MASK = ::Nirvana::endian::native == ::Nirvana::endian::big ? 1 : ~(size_t (~0) >> 1);

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
		int raw [sizeof (ULS) / sizeof (int)];
	};

protected:
	Data data_;
};

}
}

#endif
