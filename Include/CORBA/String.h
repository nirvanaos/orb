#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include <Nirvana/basic_string.h>
#include "ABI.h"

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

template <typename C>
using String = std::basic_string <C, std::char_traits <C>, std::allocator <C> >;

template <typename C>
struct ABI <String <C> > :
	public ABI_VariableLen <String <C> >
{
	typedef String <C> StringType;

	typedef const StringBase <C>* ABI_in;
	typedef const StringBase <C>& In;

	static const StringType& in (ABI_in p)
	{
		_check_pointer (p);
		const StringType& s = static_cast <const StringType&> (*p);
		check (s);
		return s;
	}

	static StringType& out (StringType* p)
	{
		StringType& val = ABI_VariableLen <StringType>::out (p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static void check (const StringType& s);
};

template <typename C>
void ABI <String <C> >::check (const StringType& s)
{
	// Do some check
	const C* p;
	size_t cc;
	if (s.is_large ()) {
		p = s.large_pointer ();
		cc = s.large_size ();
		CORBA::Nirvana::_check_pointer (p);
		if (cc > s.large_capacity () || !StringType::heap ()->is_readable (p, (cc + 1) * sizeof (C)))
			::Nirvana::throw_BAD_PARAM ();
	} else {
		p = s.small_pointer ();
		cc = s.small_size ();
		if (cc > StringABI <C>::SMALL_CAPACITY)
			::Nirvana::throw_BAD_PARAM ();
	}
	if (p [cc])
		::Nirvana::throw_BAD_PARAM ();
}

template <typename C>
using ABI_String = ABI <String <C> >;

template <typename C>
using String_in = typename ABI_String <C>::In;

template <typename C>
using String_out = typename ABI_String <C>::Out;

template <typename C>
using String_inout = typename ABI_String <C>::InOut;

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

typedef Nirvana::String_var <char> String_var;
typedef Nirvana::String_var <wchar_t> WString_var;

typedef Nirvana::String_in <char> String_in;
typedef Nirvana::String_in <wchar_t> WString_in;

typedef Nirvana::String_out <char> String_out;
typedef Nirvana::String_out <wchar_t> WString_out;

typedef Nirvana::String_inout <char> String_inout;
typedef Nirvana::String_inout <wchar_t> WString_inout;

#ifdef LEGACY_STRING_MAPPING_SUPPORT

// For compatibility with old C++ mapping specification
char* string_alloc (uint32_t len);
char* string_dup (const char* s);
void string_free (char* s);
wchar_t* wstring_alloc (uint32_t len);
wchar_t* wstring_dup (const wchar_t* s);
void wstring_free (wchar_t* s);

#endif

}

#endif
