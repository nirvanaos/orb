//! \file StringABI.h.
//!
//! \brief Declares the string ABI
//! 	This file mustn't be changed.

#ifndef NIRVANA_ORB_STRINGABI_H_
#define NIRVANA_ORB_STRINGABI_H_

#include "LocalMarshal_c.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
class StringABI
{
public:
	static size_t max_size ()
	{
		return size_t (~0) >> 1;
	}

	void swap (StringABI& rhs)
	{
		Data tmp = data_;
		data_ = rhs.data_;
		rhs.data_ = tmp;
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
			return SMALL_CAP;
	}

	bool empty () const
	{
		return size () == 0;
	}

	void marshal (StringABI <C>& dst) const
	{
		if (is_large ()) {
			dst.large_pointer ((C*)LocalMarshal::singleton ()->marshal_memory (large_pointer (), (large_size () + 1) * sizeof (C)));
			size_t size = large_size ();
			dst.large_size (size);
			dst.large_capacity (size);
		} else
			dst.data_ = data_;
	}

	void unmarshal (const StringABI <C>& src, ::Nirvana::Memory_ptr heap)
	{
		if (src.is_large ()) {
			size_t size = src.large_size ();
			size_t cb = (size + 1) * sizeof (C);
			large_pointer (LocalMarshal::singleton ()->unmarshal_memory (src.large_pointer (), cb, heap));
			large_size (size);
			large_capacity (cb / sizeof (C) - 1);
		} else
			data_ = src.data_;
	}

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
		if (std::endian::native == std::endian::big)
			data_.small.size = (unsigned char)(s << 1);
		else
			data_.small.size = (unsigned char)(s);
	}

	size_t small_size () const
	{
		if (std::endian::native == std::endian::big)
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

	void large_capacity (size_t s)
	{
		data_.large.capacity = LARGE_MASK | s;
	}

	size_t large_capacity () const
	{
		return data_.large.capacity & ~LARGE_MASK;
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
	static const unsigned char SMALL_MASK = std::endian::native == std::endian::big ? 0x01 : 0x80;
	static const size_t LARGE_MASK = std::endian::native == std::endian::big ? 1 : ~(size_t (~0) >> 1);

	struct Large
	{
		C* data;
		size_t size;
		size_t capacity;
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
	static const size_t SMALL_CAP = sizeof (Small::data) / sizeof (C) - 1;

private:
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
