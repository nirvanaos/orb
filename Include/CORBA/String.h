#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include <Nirvana/basic_string.h>

namespace CORBA {
namespace Nirvana {

template <typename C>
class String_in :
	public std::basic_string <C>
{
public:
	String_in ()
	{}

	String_in (const C* s)
	{
		if (s) {
			this->large_pointer (s);
			this->large_size (std::char_traits <C>::length (s));
			this->allocated (0, StringABI <C>::DONT_RELEASE);
		}
	}
};

template <typename C>
class String_out
{
public:
	String_out (std::basic_string <C>& s) :
		s_ (s)
	{
		s.clear ();
		s.shrink_to_fit ();
	}

	String_out (const String_out& src) :
		s_ (src.s_)
	{}

	String_out& operator = (const String_out& src)
	{
		s_ = src.s_;
		return *this;
	}

	String_out& operator = (const std::basic_string <C>& src)
	{
		s_ = src;
		return *this;
	}

	String_out& operator = (const C* src)
	{
		s_ = src;
		return *this;
	}

	// TODO: Mark as deprecated
	String_out& operator = (C* s);

	operator std::basic_string <C>& ()
	{
		return s_;
	}

private:
	std::basic_string <C>& s_;
};

// For compatibility with old C++ mapping specification
template <typename C>
class String_var :
	public String_in <C>
{
public:
	String_var ()
	{}

	// TODO: Mark as deprecated
	String_var (C* s)
	{
		adopt (s);
	}

	String_var (const C* s)
	{
		this->reset ();
		this->assign (s);
	}

	String_var& operator = (const C* s)
	{
		this->assign (s);
		return *this;
	}

	// TODO: Mark as deprecated
	String_var& operator = (C* s)
	{
		this->release_memory ();
		adopt (s);
		return *this;
	}

	operator C* ()
	{
		return this->_ptr ();
	}

	operator const C* () const
	{
		return this->_ptr ();
	}

	const String_in <C>& in () const
	{
		return *this;
	}

	std::basic_string <C>& inout ()
	{
		return *this;
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
	void adopt (C* s);
};

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

template <typename C>
String_out <C>& String_out <C>::operator = (C* s)
{
	static_cast <String_var <C>&> (s_) = s;
	return *this;
}

}

typedef Nirvana::String_in <char> String_in;
typedef Nirvana::String_in <wchar_t> WString_in;

typedef Nirvana::String_var <char> String_var;
typedef Nirvana::String_var <wchar_t> WString_var;

typedef Nirvana::String_out <char> String_out;
typedef Nirvana::String_out <wchar_t> WString_out;

// For compatibility with old C++ mapping specification
char* string_alloc (uint32_t len);
char* string_dup (const char* s);
void string_free (char* s);
wchar_t* wstring_alloc (uint32_t len);
wchar_t* wstring_dup (const wchar_t* s);
void wstring_free (wchar_t* s);

}

#endif
