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
			this->large_pointer (const_cast <C*> (s));
			this->large_size (std::char_traits <C>::length (s));
			this->allocated (0);
		}
	}

	String_in (std::basic_string <C>&& s) :
		std::basic_string (std::move (s))
	{}
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

	std::basic_string <C>* operator & () const
	{
		return &s_;
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

	String_var (std::basic_string <C>&& s) :
		String_in <C> (std::move (s))
	{}

	String_var& operator = (const C* s)
	{
		this->assign (s);
		return *this;
	}

	String_var& operator = (std::basic_string <C>&& s)
	{
		std::basic_string <C>::operator = (std::move (s));
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

namespace std {

template <typename C, class T>
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::_unmarshal (::CORBA::Nirvana::StringABI <C>* abi)
{
	::CORBA::Nirvana::_check_pointer (abi);
	basic_string <C>& s (static_cast <basic_string <C>&> (*abi));
	s._unmarshal ();
	return s;
}

template <typename C, class T>
const basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::_unmarshal (const ::CORBA::Nirvana::StringABI <C>* abi)
{
	return _unmarshal (const_cast <::CORBA::Nirvana::StringABI <C>*> (abi));
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_unmarshal () const
{
	// Do some check
	const_pointer p = data ();
	size_type cc = length ();
	if (!heap ()->is_readable (p, (cc + 1) * sizeof (value_type) || p [cc]))
		throw CORBA::MARSHAL ();
}

template <typename C, class T>
basic_string <C, T, allocator <C> >::basic_string (ABI&& src) :
	ABI (std::move (src))
{
	_unmarshal ();
}

}

#endif
