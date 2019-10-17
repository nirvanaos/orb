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
			size_t cc = std::char_traits <C>::length (s);
			this->large_pointer (const_cast <C*> (s));
			this->large_size (cc);
			this->allocated (::Nirvana::round_up ((cc + 1) * sizeof (C), 4) | 1);
		}
	}

	explicit String_in (const std::basic_string <C>& s) :
		std::basic_string (s)
	{}

	String_in (std::basic_string <C>&& s) :
		std::basic_string (std::move (s))
	{}

	operator const StringABI <C>* () const
	{
		return this;
	}
};

template <typename C>
class String_inout
{
public:
	String_inout (std::basic_string <C>& s) :
		s_ (s)
	{}

	String_inout (const String_inout& s) :
		s_ (s.s_)
	{}

	~String_inout ()
	{
		s_._unmarshal_or_clear ();
	}

	operator StringABI <C>* () const
	{
		return &s_;
	}

	String_inout& operator = (const String_inout& s)
	{
		s_ = s.s_;
		return *this;
	}

	String_inout& operator = (const C* s)
	{
		s_ = s;
		return *this;
	}

	String_inout& operator = (C* s);

private:
	std::basic_string <C>& s_;
};

template <typename C>
class String_out : public String_inout <C>
{
public:
	String_out (std::basic_string <C>& s) :
		String_inout <C> (s)
	{
		s._clear_out ();
	}
};

template <typename C>
class String_var :
	public String_in <C>
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
		this->reset ();
		this->assign (s);
	}

	String_var (const std::basic_string <C>& s) :
		String_in <C> (s)
	{}

	String_var (std::basic_string <C>&& s) :
		String_in <C> (std::move (s))
	{}

	String_var (StringABI <C>&& src)
	{
		this->data_ = src.data_;
		src.reset ();
		this->_unmarshal_or_clear ();
	}

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

	const String_in <C>& in () const
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
String_inout <C>& String_inout <C>::operator = (C* s)
{
	static_cast <String_var <C>&> (s_) = s;
	return *this;
}

#endif

template <typename C>
const std::basic_string <C>& _unmarshal_in (const StringABI <C>* abi)
{
	return _unmarshal_inout (const_cast <StringABI <C>*> (abi));
}

template <typename C>
std::basic_string <C>& _unmarshal_inout (StringABI <C>* abi)
{
	_check_pointer (abi);
	std::basic_string <C>& s (static_cast <std::basic_string <C>&> (*abi));
	s._unmarshal ();
	return s;
}

template <typename C>
std::basic_string <C>& _unmarshal_out (StringABI <C>* abi)
{
	_check_pointer (abi);
	std::basic_string <C>& s (static_cast <std::basic_string <C>&> (*abi));
	s._unmarshal_out ();
	return s;
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
void basic_string <C, T, allocator <C> >::_unmarshal () const
{
	// Do some check
	const_pointer p;
	size_type cc;
	if (this->is_large ()) {
		p = this->large_pointer ();
		cc = this->large_size ();
		if (cc > this->large_capacity () || !heap ()->is_readable (p, (cc + 1) * sizeof (value_type)))
			throw CORBA::MARSHAL ();
	} else {
		p = this->small_pointer ();
		cc = this->small_size ();
		if (cc > ABI::SMALL_CAPACITY)
			throw CORBA::MARSHAL ();
	}
	if (p [cc])
		throw CORBA::MARSHAL ();
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_unmarshal_or_clear ()
{
	try {
		_unmarshal ();
	} catch (...) {
		_clear_out ();
		throw;
	}
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_unmarshal_out () const
{
	const size_t* end = this->data_.raw + countof (this->data_.raw);
	if (std::find_if (this->data_.raw, end, [](size_t i) { return i != 0; }) != end)
		throw CORBA::MARSHAL ();
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_clear_out ()
{
	release_memory ();
	this->reset ();
}

}

#endif
