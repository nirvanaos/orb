#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include <Nirvana/basic_string.h>

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
using String_in = const StringBase <C>&;

template <typename C>
class String_inout_base
{
public:
	String_inout_base (std::basic_string <C>& s) :
		s_ (s)
	{}

	String_inout_base (const String_inout_base& s) :
		s_ (s.s_)
	{}

	~String_inout_base () noexcept (false);

	StringABI <C>* operator & () const
	{
		return &s_;
	}

	operator std::basic_string <C>& () const
	{
		return s_;
	}

	std::basic_string <C> move () const
	{
		return std::basic_string <C> (std::move (s_));
	}

	String_inout_base& operator = (const String_inout_base& s)
	{
		s_ = s.s_;
		return *this;
	}

	template <class Tr, class Al>
	String_inout_base& operator = (const std::basic_string <C, Tr, Al>& s)
	{
		size_t cc = s.size ();
		s_.assign (s.data (), cc);
		return *this;
	}

	String_inout_base& operator = (const C* s)
	{
		s_ = s;
		return *this;
	}

	size_t size () const
	{
		return s_.size ();
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_inout_base& operator = (C* s);

#endif

protected:
	std::basic_string <C>& s_;
};

template <typename C> // Outline for compact code
String_inout_base <C>::~String_inout_base () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		s_._check_or_clear ();
	} catch (...) {
		if (!ex)
			throw;
	}
}

template <typename C>
class String_inout : public String_inout_base <C>
{
public:
	String_inout (std::basic_string <C>& s) :
		String_inout_base <C> (s)
	{}

	String_inout (const String_inout& s) :
		String_inout_base <C> (s)
	{}

	template <class Tr, class Al>
	String_inout& operator = (const std::basic_string <C, Tr, Al>& s)
	{
		String_inout_base <C>::operator = (s);
		return *this;
	}

	String_inout& operator = (const C* s)
	{
		String_inout_base <C>::operator = (s);
		return *this;
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_inout& operator = (C* s)
	{
		String_inout_base <C>::operator = (s);
		return *this;
	}

#endif
};

template <typename C>
class String_out : public String_inout_base <C>
{
public:
	String_out (std::basic_string <C>& s) :
		String_inout_base <C> (s)
	{
		s.clear ();
	}

	String_out (const String_out& s) :
		String_inout_base <C> (s)
	{}

	template <class Tr, class Al>
	String_out& operator = (const std::basic_string <C, Tr, Al>& s)
	{
		String_inout_base <C>::operator = (s);
		return *this;
	}

	String_out& operator = (const C* s)
	{
		String_inout_base <C>::operator = (s);
		return *this;
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_out& operator = (C* s)
	{
		String_inout_base <C>::operator = (s);
		return *this;
	}

#endif
};

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

	String_var (StringABI <C>&& src)
	{
		this->data_ = src.data_;
		src.reset ();
		this->_check_or_clear ();
	}

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

template <typename C>
String_inout_base <C>& String_inout_base <C>::operator = (C* s)
{
	static_cast <String_var <C>&> (s_) = s;
	return *this;
}

#endif

template <typename C>
const std::basic_string <C>& _unmarshal_in (const StringABI <C>* abi)
{
	_check_pointer (abi);
	const std::basic_string <C>& s (static_cast <const std::basic_string <C>&> (*abi));
	s._check ();
	return s;
}

template <size_t BOUND, typename C>
const std::basic_string <C>& _unmarshal_in (const StringABI <C>* abi)
{
	const std::basic_string <C>& s = _unmarshal_in (abi);
	_check_bound (s, BOUND);
	return s;
}

template <typename C>
std::basic_string <C>& _unmarshal_inout (StringABI <C>* abi)
{
	_check_pointer (abi);
	std::basic_string <C>& s (static_cast <std::basic_string <C>&> (*abi));
	s._check ();
	return s;
}

template <size_t BOUND, typename C>
std::basic_string <C>& _unmarshal_inout (StringABI <C>* abi)
{
	std::basic_string <C>& s = _unmarshal_inout (abi);
	_check_bound (s, BOUND);
	return s;
}

template <typename C>
std::basic_string <C>& _unmarshal_out (StringABI <C>* abi)
{
	_check_pointer (abi);
	std::basic_string <C>& s (static_cast <std::basic_string <C>&> (*abi));
	if (!s.empty ())
		::Nirvana::throw_MARSHAL ();
	return s;
}

template <size_t BOUND, typename C>
std::basic_string <C>& _unmarshal_out (StringABI <C>* abi)
{
	return _unmarshal_out (abi);
}

}

typedef Nirvana::String_in <char> String_in;
typedef Nirvana::String_in <wchar_t> WString_in;

typedef Nirvana::String_var <char> String_var;
typedef Nirvana::String_var <wchar_t> WString_var;

typedef Nirvana::String_out <char> String_out;
typedef Nirvana::String_out <wchar_t> WString_out;

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

namespace std {

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_check () const
{
	// Do some check
	const_pointer p;
	size_type cc;
	if (this->is_large ()) {
		p = this->large_pointer ();
		cc = this->large_size ();
		CORBA::Nirvana::_check_pointer (p);
		if (cc > this->large_capacity () || !heap ()->is_readable (p, (cc + 1) * sizeof (value_type)))
			::Nirvana::throw_MARSHAL ();
	} else {
		p = this->small_pointer ();
		cc = this->small_size ();
		if (cc > ABI::SMALL_CAPACITY)
			::Nirvana::throw_MARSHAL ();
	}
	if (p [cc])
		::Nirvana::throw_MARSHAL ();
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_check_or_clear ()
{
	try {
		_check ();
	} catch (...) {
		release_memory ();
		this->reset ();
		throw;
	}
}

}

#endif
