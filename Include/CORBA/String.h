#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include <Nirvana/basic_string.h>
#include "TypeVarLen.h"

namespace std {

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_check () const
{
	// Do some check
	const C* p;
	size_t cc;
	if (this->is_large ()) {
		p = this->large_pointer ();
		cc = this->large_size ();
		CORBA::Nirvana::_check_pointer (p);
		if (cc > this->large_capacity () || !heap ()->is_readable (p, (cc + 1) * sizeof (C)))
			::Nirvana::throw_BAD_PARAM ();
	} else {
		p = this->small_pointer ();
		cc = this->small_size ();
		if (cc > ABI::SMALL_CAPACITY)
			::Nirvana::throw_BAD_PARAM ();
	}
	if (p [cc])
		::Nirvana::throw_BAD_PARAM (); // Not zero-terminated
}

}

namespace CORBA {
namespace Nirvana {

template <typename C>
StringBase <C>::StringBase (const C* s)
{
	if (s) {
		size_t cc = std::char_traits <C>::length (s);
		this->large_pointer (const_cast <C*> (s));
		this->large_size (cc);
		this->allocated (0);
	} else
		this->reset ();
}

template <typename C, class T = std::char_traits <C> >
using StringT = std::basic_string <C, T, std::allocator <C> >;

typedef StringT <Char> String;
typedef StringT <WChar> WString;

template <typename C, class T>
struct Type <StringT <C, T> > :
	public TypeVarLen <StringT <C, T> >
{
	typedef StringT <C, T> StringType;

	static void check (const StringType& s)
	{
		s._check ();
	}

	static StringType& out (StringType* p)
	{
		StringType& val = TypeVarLen <StringType>::out (p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}
};

template <typename C, class T = std::char_traits <C> >
using TypeString = Type <StringT <C, T> >;

template <typename C, class T = std::char_traits <C> >
using String_in = typename TypeString <C, T>::C_in;

template <typename C, class T = std::char_traits <C> >
using String_out = typename TypeString <C, T>::C_out;

template <typename C, class T = std::char_traits <C> >
using String_inout = typename TypeString <C, T>::C_inout;

template <typename C>
class String_var : public std::basic_string <C>
{
public:
	String_var ()
	{}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_var (C* s)
	{
		adopt (s);
	}

#endif

	String_var (const C* s)
	{
		size_t cc = std::char_traits <C>::length (s);
		this->assign (s, cc);
	}

	String_var (const std::basic_string <C>& s) :
		std::basic_string <C> (s)
	{}

	String_var (std::basic_string <C>&& s) :
		std::basic_string <C> (std::move (s))
	{}

	String_var& operator = (const C* s)
	{
		std::basic_string <C>::operator = (s);
		return *this;
	}

	template <class Tr, class Al>
	String_var& operator = (const std::basic_string <C, Tr, Al>& s)
	{
		std::basic_string <C>::operator = (s);
		return *this;
	}

	String_var& operator = (std::basic_string <C>&& s)
	{
		std::basic_string <C>::operator = (std::move (s));
		return *this;
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_var& operator = (C* s)
	{
		this->release_memory ();
		adopt (s);
		return *this;
	}

#endif

	operator C* ()
	{
		return this->_ptr ();
	}

	operator const C* () const
	{
		return this->_ptr ();
	}

	String_in <C> in () const
	{
		return *this;
	}

	String_inout <C> inout ()
	{
		return String_inout <C> (*this);
	}

	String_out <C> out ()
	{
		return String_out <C> (*this);
	}

	std::basic_string <C> _retn ()
	{
		return std::move (*this);
	}

private:

#ifdef LEGACY_STRING_MAPPING_SUPPORT
	void adopt (C* s);
#endif
};

#ifdef LEGACY_STRING_MAPPING_SUPPORT

template <typename C>
void String_var <C>::adopt (C* s)
{
	if (s) {
		size_t cc = std::char_traits <C>::length (s);
		this->large_pointer (s);
		this->large_size (cc);
		size_t au = ::Nirvana::StdString::heap ()->query (s, ::Nirvana::Memory::ALLOCATION_UNIT);
		this->large_allocated (::Nirvana::round_up ((cc + 1) * sizeof (C), au));
	} else
		this->reset ();
}

#endif

}

typedef Nirvana::String_var <Char> String_var;
typedef Nirvana::String_var <WChar> WString_var;

typedef Nirvana::String_in <Char> String_in;
typedef Nirvana::String_in <WChar> WString_in;

typedef Nirvana::String_out <Char> String_out;
typedef Nirvana::String_out <WChar> WString_out;

typedef Nirvana::String_inout <Char> String_inout;
typedef Nirvana::String_inout <WChar> WString_inout;

#ifdef LEGACY_STRING_MAPPING_SUPPORT

// For compatibility with old C++ mapping specification
Char* string_alloc (uint32_t len);
Char* string_dup (const Char* s);
void string_free (Char* s);
WChar* wstring_alloc (uint32_t len);
WChar* wstring_dup (const WChar* s);
void wstring_free (WChar* s);

#endif

}

#endif
