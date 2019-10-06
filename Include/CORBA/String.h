#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include "StlUtils.h"
#include <Nirvana/real_copy.h>
#include "StringABI.h"
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

class StdString :
	public StdContainer
{
public:
	ORB_STL_NORETURN static void xout_of_range (const char* msg)
	{
		xout_of_range ("invalid string position");
	}
	ORB_STL_NORETURN static void xlength_error (const char* msg)
	{
		xlength_error ("string too long");
	}

	static ::Nirvana::Memory_ptr heap ()
	{
		return ::Nirvana::g_default_heap;
	}
};

template <typename C>
class basic_string :
	public StringABI <C>,
	private StdString
{
	typedef StringABI <C> ABI;
public:
	using const_iterator = StdConstIterator <basic_string <C> >;
	using iterator = StdIterator <basic_string <C> >;

	typedef std::reverse_iterator <const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator <iterator> reverse_iterator;

	typedef std::allocator <C> allocator_type;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::difference_type difference_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::size_type size_type;
	typedef typename std::char_traits <C> traits_type;
	typedef C value_type;

	static const size_type npos = -1;

	~basic_string ()
	{
		release_memory ();
	}

	// Constructors
	
	basic_string ()
	{
		this->reset ();
	}

	basic_string (const basic_string <value_type>& src)
	{
		if (src.is_large ()) {
			ABI::reset ();
			assign (src.large_pointer (), src.large_size ());
		} else
			this->data_ = src.data_;
	}

	basic_string (basic_string <value_type>&& src)
	{
		this->data_ = src::data_;
		src.reset ();
	}

	basic_string (const basic_string <value_type>& src, size_type off, size_type cnt = npos)
	{
		this->reset ();
		assign (src, off, npos);
	}

	basic_string (const C* ptr, size_type cnt)
	{
		this->reset ();
		assign (ptr, cnt);
	}

	basic_string (const C* ptr)
	{
		this->reset ();
		assign (ptr);
	}

	basic_string (size_type cnt, C c)
	{
		this->reset ();
		assign (cnt, c);
	}

	template <class InputIterator>
	basic_string (InputIterator b, InputIterator e)
	{
		this->reset ();
		assign (b, e);
	}

	basic_string (const_pointer b, const_pointer e)
	{
		this->reset ();
		assign (b, e);
	}

	basic_string (const_iterator b, const_iterator e)
	{
		this->reset ();
		assign (b, e);
	}

#if __cplusplus >= 201103L
	basic_string (std::initializer_list <value_type> ilist)
	{
		this->reset ();
		assign (ilist.begin (), ilist.end ());
	}
#endif

	// Assignments

	basic_string <C>& operator = (value_type c)
	{
		return assign (&c, 1);
	}

	basic_string <C>& operator = (const value_type* s)
	{
		return assign (s);
	}

	basic_string <value_type>& operator = (const basic_string <value_type>& src)
	{
		return assign (src.large_pointer (), src.large_size ());
	}

	basic_string <value_type>& operator = (basic_string <value_type>&& src)
	{
		release_memory ();
		this->data_ = src::data_;
		src.reset ();
		return *this;
	}

#if __cplusplus >= 201103L
	basic_string <value_type>& operator = (std::initializer_list <value_type> ilist)
	{
		return assign (ilist.begin (), ilist.end ());
	}
#endif

	basic_string <C>& assign (const value_type* ptr)
	{
		return assign (ptr, traits_type::length (ptr));
	}

	basic_string <C>& assign (const value_type* ptr, size_type count);
	basic_string <C>& assign (const basic_string <C>& str, size_type off, size_type count);

	basic_string <C>& assign (const basic_string <C>& src)
	{
		return assign (src.data (), src.size ());
	}

	basic_string <C>& assign (size_type count, value_type c)
	{
		std::fill_n (commit (count), count, c);
		return *this;
	}

	template <class InputIterator>
	basic_string <C>& assign (InputIterator b, InputIterator e)
	{
		std::copy (b, e, commit (e - b));
		return *this;
	}

	basic_string <C>& assign (const_pointer b, const_pointer e)
	{
		return assign (b, e - b);
	}

	basic_string <C>& assign (const_iterator b, const_iterator e)
	{
		return assign (&*b, e - b);
	}

#if __cplusplus >= 201103L
	basic_string <value_type>& assign (std::initializer_list <value_type> ilist)
	{
		return assign (ilist.begin (), ilist.end ());
	}
#endif

	// append

	basic_string <C>& append (const value_type* ptr)
	{
		return append (ptr, traits_type::length (ptr));
	}

	basic_string <C>& append (const value_type* ptr, size_type count);

	basic_string <C>& append (const basic_string <C>& str, size_type off, size_type count);

	basic_string <C>& append (const basic_string <C>& src)
	{
		return append (src.data (), src.size ());
	}

	basic_string <C>& append (size_type count, value_type c)
	{
		std::fill_n (commit_append (count), count, c);
		return *this;
	}

	template <class InputIterator>
	basic_string <C>& append (InputIterator b, InputIterator e)
	{
		std::copy (b, e, commit_append (e - b));
		return *this;
	}

	basic_string <C>& append (const_pointer b, const_pointer e)
	{
		return append (b, e - b);
	}

	basic_string <C>& append (const_iterator b, const_iterator e)
	{
		return append (&*b, e - b);
	}

	basic_string <C>& operator += (value_type c)
	{
		append (1, c);
	}

	basic_string <C>& operator += (const value_type* s)
	{
		append (s);
	}

	basic_string <C>& operator += (const basic_string <C>& s)
	{
		append (s);
	}

	// Misc. operations
	
	const_reference at (size_type off) const
	{
		if (this->is_large ()) {
			if (off >= this->large_size ())
				xout_of_range ();
			return this->large_pointer () [off];
		} else {
			if (off >= this->small_size ())
				xout_of_range ();
			return this->small_pointer () [off];
		}
	}

	reference at (size_type off)
	{
		return const_cast <reference> (const_cast <basic_string <C>*> (this)->at (off));
	}

	const_reference operator [] (size_type off) const
	{
		return this->_ptr () [off];
	}

	reference operator [] (size_type off)
	{
		return this->_ptr () [off];
	}

	const_reference back () const
	{
		return this->_ptr [this->size () - 1];
	}

	reference back ()
	{
		return this->_ptr [this->size () - 1];
	}

	const value_type* data () const
	{
		return this->_ptr ();
	}

	const value_type* c_str () const
	{
		return data ();
	}



	void clear ();
	void reserve (size_type cap = 0);
	void shrink_to_fit ();

	const_iterator cbegin () const
	{
		return const_iterator (this->_ptr (), *this);
	}

	iterator begin ()
	{
		return iterator (this->_ptr (), *this);
	}

	const_iterator begin () const
	{
		return cbegin ();
	}

	const_iterator cend () const
	{
		return const_iterator (this->_ptr () + this->size (), *this);
	}

	iterator end ()
	{
		return iterator (this->_ptr () + this->size (), *this);
	}

	const_iterator end () const
	{
		return cend ();
	}


	void unmarshal (const ABI& src)
	{
		ABI::unmarshal (src, heap ());
	}

private:
	void release_memory ()
	{
		if (this->is_large ())
			heap ()->release (this->large_pointer (), byte_size (this->large_capacity ()));
	}

	pointer commit (size_type size);
	pointer commit_append (size_type append);

	static size_t byte_size (size_type char_cnt)
	{
		return (char_cnt + 1) * sizeof (value_type);
	}

	static size_type char_cnt (size_t byte_size)
	{
		return byte_size / sizeof (value_type) - 1;
	}

	static size_type add_size (size_type s1, size_type s2)
	{
		if (this->max_size () - s1 < s2)
			xlength_error ();
		return s1 + s2;
	}
};

template <typename C>
void basic_string <C>::clear ()
{
	if (this->is_large ()) {
		pointer p = this->large_pointer ();
		p [0] = 0;
		size_t cc = this->large_size ();
		if (cc) {
			this->large_size (0);
			heap ()->decommit (p + 1, cc * sizeof (value_type));
		}
	} else {
		this->small_pointer () [0] = 0;
		this->small_size (0);
	}
}

template <typename C>
void basic_string <C>::reserve (size_type cap)
{
	if (!cap)
		shrink_to_fit ();
	else if (cap > ABI::max_size ())
		xlength_error ();
	if (this->is_large ()) {
		size_t cur_cap = this->large_capacity ();
		if (cur_cap < cap) {
			size_t space = byte_size (cap);
			this->large_pointer ((pointer)::Nirvana::MemoryHelper (heap ()).reserve (this->large_pointer (), byte_size (this->large_size ()), byte_size (cur_cap), space));
			this->large_capacity (char_cnt (space));
		}
	} else if (cap > ABI::SMALL_CAP) {
		size_t space = byte_size (cap);
		pointer p = (pointer)::Nirvana::MemoryHelper (heap ()).reserve (space);
		size_t cc = this->small_size ();
		heap ()->commit (p, byte_size (cc));
		if (cc)
			Nirvana::real_copy (this->small_pointer (), this->small_pointer () + cc + 1, p);
		else
			p [0] = 0;
		this->large_pointer (p);
		this->large_size (cc);
		this->large_capacity (char_cnt (space));
	}
}

template <typename C>
void basic_string <C>::shrink_to_fit ()
{
	if (this->is_large ()) {
		size_t cc = this->large_size ();
		if (cc <= ABI::SMALL_CAP) {
			C* p = this->large_pointer ();
			size_t space = byte_size (this->large_capacity ());
			Nirvana::real_copy (p, p + cc + 1, this->small_pointer ());
			this->small_size (cc);
			heap ()->release (p, space);
		} else {
			size_t space = byte_size (this->large_capacity ());
			::Nirvana::MemoryHelper (heap ()).shrink_to_fit (this->large_pointer (), space, byte_size (cc));
			this->large_capacity (char_cnt (space));
		}
	}
}

template <typename C>
basic_string <C>& basic_string <C>::assign (const value_type* ptr, size_type count)
{
	if (count <= ABI::SMALL_CAP && !this->is_large ()) {
		pointer p = this->small_pointer ();
		*::Nirvana::real_copy (ptr, ptr + count, p) = 0;
		this->small_size (count);
	} else if (count > ABI::max_size ())
		xlength_error ();
	else {
		void* p;
		size_t space;
		if (!this->is_large ()) {
			p = nullptr;
			space = 0;
		} else {
			p = this->large_pointer ();
			space = byte_size (this->large_capacity ());
		}
		pointer p = (pointer)::Nirvana::MemoryHelper (heap ()).assign (p, space, byte_size (this->large_size ()), ptr, byte_size (count));
		p [count] = 0;
		this->large_pointer (p);
		this->large_size (count);
		this->large_capacity (char_cnt (space));
	}
	return *this;
}

template <typename C>
basic_string <C>& basic_string <C>::assign (const basic_string <C>& str, size_type off, size_type count)
{
	if (npos == count) {
		if (off > str.size ())
			xout_of_range ();
		count = str.size () - off;
	} else if (count + off > str.size ())
		xout_of_range ();

	assign (str.data () + off, count);
}

template <typename C>
basic_string <C>& basic_string <C>::append (const value_type* ptr, size_type count)
{
	size_type old_size = this->size ();
	size_type new_size = add_size (old_size, count);
	reserve (new_size);
	if (this->is_large ()) {
		pointer p = this->large_pointer ();
		heap ()->copy (p + old_size, ptr, byte_size (count));
		p [new_size] = 0;
		this->large_size (new_size);
	} else {
		pointer p = this->small_pointer ();
		*::Nirvana::real_copy (ptr, ptr + count, p) = 0;
		this->small_size (new_size);
	}
	return *this;
}

template <typename C>
typename basic_string <C>::pointer basic_string <C>::commit (size_type size)
{
	if (!this->is_large () && this->small_capacity () >= size) {
		this->small_size (size);
		this->small_pointer () [size] = 0;
		return this->small_pointer ();
	} else {
		size_t space = byte_size (this->large_capacity ());
		pointer p = (pointer)::Nirvana::MemoryHelper (heap ()).commit (this->large_pointer (), space,
			byte_size (this->large_size ()), byte_size (size));
		p [size] = 0;
		this->large_pointer (p);
		this->large_size (size);
		this->large_capacity (char_cnt (space));
		return p;
	}
}

template <typename C>
typename basic_string <C>::pointer basic_string <C>::commit_append (size_type append_size)
{
	size_type old_size = this->size ();
	size_type new_size = add_size (old_size, append_size);
	reserve (new_size);
	if (!this->is_large () && this->small_capacity () >= new_size) {
		this->small_size (new_size);
		this->small_pointer () [new_size] = 0;
		return this->small_pointer () + old_size;
	} else {
		size_t space = byte_size (new_size);
		pointer p = (pointer)::Nirvana::MemoryHelper (heap ()).commit (this->large_pointer (), space,
			byte_size (old_size), byte_size (new_size));
		p [new_size] = 0;
		this->large_pointer (p);
		this->large_size (new_size);
		this->large_capacity (char_cnt (space));
		return p + old_size;
	}
}

}
}

namespace std {

template <typename C>
class basic_string <C, char_traits <C>, allocator <C> > :
	public ::CORBA::Nirvana::basic_string <C>
{
	typedef ::CORBA::Nirvana::basic_string <C> Impl;
	typedef typename Impl::allocator_type allocator_type;
	typedef typename Impl::const_pointer const_pointer;
	typedef typename Impl::const_reference const_reference;
	typedef typename Impl::difference_type difference_type;
	typedef typename Impl::pointer pointer;
	typedef typename Impl::reference reference;
	typedef typename Impl::size_type size_type;
	typedef typename char_traits <C> traits_type;
	typedef C value_type;

	typedef typename Impl::const_iterator const_iterator;
	typedef typename Impl::iterator iterator;
	typedef typename Impl::const_reverse_iterator const_reverse_iterator;
	typedef typename Impl::reverse_iterator reverse_iterator;

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
	public std::basic_string <C>
{
public:
	String_in (const C* s)
	{
		if (s) {
			this->large_pointer (s);
			this->large_size (std::char_traits <C>::length (s));
		}
	}

	~String_in ()
	{
		this->reset ();
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
