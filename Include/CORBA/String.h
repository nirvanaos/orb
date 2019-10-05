#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include <Nirvana/Nirvana.h>
#include <Nirvana/Memory_c.h>
#include "LocalMarshal_c.h"
#include <string>
#include <iterator>
#include <memory>

#if __cplusplus >= 201103L
#include <initializer_list>
#endif

#if __cplusplus >= 201703L
# include <string_view>
#endif

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

	void clear ()
	{
		if (is_large ())
			heap ()->release (large_pointer (), large_capacity () * sizeof (C));
		reset ();
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

	void reserve (size_t cap)
	{
		if (is_large ()) {
			size_t cur_cap = large_capacity ();
			if (cur_cap < cc) {

			}
		} else if (cap > SMALL_CAP) {
			heap ()->allocate (0, )
		}
	}

	void marshal (StringABI& dst) const
	{
		if (is_large ()) {
			dst.large_pointer((C*)LocalMarshal::singleton ()->marshal_memory (large_pointer (), (large_size () + 1) * sizeof (C)));
			size_t size = large_size ();
			dst.large_size (size);
			dst.large_capacity (size);
		} else
			dst.data_ = data_;
	}

	void unmarshal (const StringABI& src)
	{
		if (src.is_large ()) {
			size_t size = src.large_size ();
			size_t cb = (size + 1) * sizeof (C);
			large_pointer (LocalMarshal::singleton ()->unmarshal_memory (src.large_pointer (), cb, heap ()));
			large_size (size);
			large_capacity (cb / sizeof (C) - 1);
		} else
			data_ = src.data_;
	}

protected:
	StringABI ()
	{
		reset ();
	}

	StringABI (StringABI&& src)
	{
		data_ = src::data_;
		src.clear ();
	}

	void reset ()
	{
		std::fill_n (data_.raw, 0, countof (data_.raw));
	}

	StringABI& operator = (StringABI&& src)
	{
		data_ = src::data_;
		src.clear ();
		return *this;
	}

	static ::Nirvana::Memory_ptr heap ()
	{
		return ::Nirvana::g_default_heap;
	}

	bool is_large () const
	{
		data_.small.size & SMALL_MASK;
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

	C* large_pointer ()
	{
		return data_.large.data;
	}

	const C* large_pointer () const
	{
		return data_.large.data;
	}

	C* small_pointer ()
	{
		return data_.small.data;
	}

	const C* small_pointer () const
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
		size_t raw [sizeof (ULS) / sizeof (size_t)];
	};

	Data data_;
};

template <typename C>
class basic_string :
	public StringABI <C>
{
public:
	class const_iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = C;
		using difference_type = ptrdiff_t;
		using pointer = const C*;
		using reference = const C&;
	};

	class iterator : public const_iterator
	{
	public:
		using pointer = const C*;
		using reference = const C&;
	};

	typedef std::reverse_iterator <const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator <iterator> reverse_iterator;

	typedef std::allocator <C> allocator_type;
	typedef allocator_type::const_pointer const_pointer;
	typedef allocator_type::const_reference const_reference;
	typedef allocator_type::difference_type difference_type;
	typedef allocator_type::pointer pointer;
	typedef allocator_type::reference reference;
	typedef allocator_type::size_type size_type;
	typedef std::char_traits <C> traits_type;
	typedef C value_type;

	static const size_type npos = -1;

	basic_string ()
	{}

	basic_string (const basic_string& src)
	{
	}

	basic_string (basic_string&& src)
	{}

	basic_string (const basic_string& src, size_type off, size_type cnt)
	{}

	basic_string (const C* ptr, size_type cnt)
	{}

	basic_string (const C* ptr)
	{}

	basic_string (size_type cnt, C c)
	{}

	template <class InputIterator>
	basic_string (InputIterator b, InputIterator e)
	{}

	basic_string (const_pointer b, const_pointer e)
	{}

	basic_string (const_iterator b, const_iterator e)
	{}
};

}
}

namespace std {

template <typename C>
class basic_string <C, char_traits <C>, allocator <C> > :
	public ::CORBA::Nirvana::basic_string <C>
{
	typedef ::CORBA::Nirvana::basic_string <C> Impl;
public:
	typedef allocator <C> allocator_type;

	basic_string ()
	{}

	basic_string (const allocator_type&)
	{}

	basic_string (const basic_string& src) :
		Impl (src)
	{}

	basic_string (basic_string&& src) :
		Impl (src)
	{}

	basic_string (const basic_string& src, size_type off, size_type cnt) :
		Impl (src, off, cnt)
	{}

	basic_string (const basic_string& src, size_type off, size_type cnt, const allocator_type&) :
		Impl (src, off, cnt)
	{}

	basic_string (const C* ptr, size_type cnt) :
		Impl (ptr, cnt)
	{}

	basic_string (const C* ptr, size_type cnt, const allocator_type&) :
		Impl (ptr, cnt)
	{}

	basic_string (const C* ptr) :
		Impl (ptr)
	{}

	basic_string (const C* ptr, const allocator_type&) :
		Impl (ptr)
	{}

	basic_string (size_type cnt, C c) :
		Impl (cnt, c)
	{}

	basic_string (size_type cnt, C c, const allocator_type&) :
		Impl (cnt, c)
	{}

	template <class InputIterator>
	basic_string (InputIterator b, InputIterator e) :
		Impl (b, e)
	{}

	template <class InputIterator>
	basic_string (InputIterator b, InputIterator e, const allocator_type&) :
		Impl (b, e)
	{}

	basic_string (const_pointer b, const_pointer e) :
		Impl (b, e)
	{}

	basic_string (const_iterator b, const_iterator e) :
		Impl (b, e)
	{}
};

}

namespace CORBA {
namespace Nirvana {

template <typename C>
class String_in :
	public StringABI <C>
{
public:
	String_in (const C* s)
	{
		if (s) {
			large_pointer (s);
			large_size (std::char_traits <C>::length (s));
		}
	}
};

template <typename C>
class String_var :
	public String_in <C>
{
public:
	String_var ()
	{}

	String_var (C* s)
	{

	}

	String_var (const C* s)
	{}
};

}

typedef Nirvana::String_in <char> String_in;
typedef Nirvana::String_in <wchar_t> WString_in;

// For compatibility with old C++ mapping specification
char* string_alloc (uint32_t len);
char* string_dup (const char* s);
void string_free (char* s);

}

#endif
