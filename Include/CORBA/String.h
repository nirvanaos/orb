#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include <Nirvana/basic_string.h>

namespace CORBA {
namespace Nirvana {

template <typename C>
String_in <C>::String_in (const C* s)
{
	if (s) {
		size_t cc = std::char_traits <C>::length (s);
		this->large_pointer (const_cast <C*> (s));
		this->large_size (cc);
		this->allocated (::Nirvana::round_up ((cc + 1) * sizeof (C), 4) | 1);
	} else
		this->reset ();
}

template <typename C, size_t BOUND = 0>
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

	operator const std::basic_string <C>& () const
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
		if (BOUND && cc > BOUND)
			throw BAD_PARAM ();
		s_.assign (s.data (), cc);
		return *this;
	}

	String_inout_base& operator = (const C* s)
	{
		if (BOUND) {
			size_t cc = std::char_traits <C>::length (s);
			if (cc > BOUND)
				throw BAD_PARAM ();
			s_.assign (s, cc);
		} else
			s_ = s;
		return *this;
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_inout_base& operator = (C* s);

#endif

private:
	std::basic_string <C>& s_;
};

template <typename C, size_t BOUND>
String_inout_base <C, BOUND>::~String_inout_base () noexcept (false)
{
	if (!std::uncaught_exception ()) {
		s_._unmarshal_or_clear ();
		if (BOUND && s_.size () > BOUND) {
			s_.clear ();
			throw BAD_PARAM ();
		}
	}
}

template <typename C, size_t BOUND = 0>
class String_inout : public String_inout_base <C, BOUND>
{
public:
	String_inout (std::basic_string <C>& s) :
		String_inout_base <C, BOUND> (s)
	{
		if (BOUND && s.size () > BOUND)
			throw BAD_PARAM ();
	}

	String_inout (const String_inout& s) :
		String_inout_base <C, BOUND> (s)
	{}

	template <class Tr, class Al>
	String_inout& operator = (const std::basic_string <C, Tr, Al>& s)
	{
		String_inout_base <C, BOUND>::operator = (s);
		return *this;
	}

	String_inout& operator = (const C* s)
	{
		String_inout_base <C, BOUND>::operator = (s);
		return *this;
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_inout& operator = (C* s)
	{
		String_inout_base <C, BOUND>::operator = (s);
		return *this;
	}

#endif
};

template <typename C, size_t BOUND = 0>
class String_out : public String_inout_base <C, BOUND>
{
public:
	String_out (std::basic_string <C>& s) :
		String_inout_base <C, BOUND> (s)
	{
		s.clear ();
	}

	String_out (const String_out& s) :
		String_inout_base <C, BOUND> (s)
	{}

	template <class Tr, class Al>
	String_out& operator = (const std::basic_string <C, Tr, Al>& s)
	{
		String_inout_base <C, BOUND>::operator = (s);
		return *this;
	}

	String_out& operator = (const C* s)
	{
		String_inout_base <C, BOUND>::operator = (s);
		return *this;
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_out& operator = (C* s)
	{
		String_inout_base <C, BOUND>::operator = (s);
		return *this;
	}

#endif
};

template <typename C, size_t BOUND = 0>
class String_var :
	public std::basic_string <C>
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
		size_t cc = std::char_traits::length (s);
		if (BOUND && cc > BOUND)
			throw BAD_PARAM ();
		this->assign (s, cc);
	}

	String_var (const std::basic_string <C>& s) :
		std::basic_string <C> (check_length (s))
	{}

	String_var (std::basic_string <C>&& s) :
		std::basic_string <C> (std::move (check_length (s)))
	{}

	String_var (StringABI <C>&& src)
	{
		this->data_ = src.data_;
		src.reset ();
		this->_unmarshal_or_clear ();
	}

	String_var& operator = (const C* s)
	{
		if (BOUND) {
			size_t cc = std::char_traits <C>::length (s);
			if (cc > BOUND)
				throw BAD_PARAM ();
			this->assign (s, cc);
		} else
			std::basic_string <C>::operator = (s);
		return *this;
	}

	template <class Tr, class Al>
	String_var& operator = (const std::basic_string <C, Tr, Al>& s)
	{
		size_t cc = s.size ();
		if (BOUND && cc > BOUND)
			throw BAD_PARAM ();
		this->assign (s.data (), cc);
		return *this;
	}

	String_var& operator = (std::basic_string <C>&& s)
	{
		if (BOUND && s.size () > BOUND)
			throw BAD_PARAM ();
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

	const String_in <C> in () const
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
	static const std::basic_string <C>& check_length (const std::basic_string <C>& s)
	{
		if (BOUND && s.size () > BOUND)
			throw BAD_PARAM ();
		return s;
	}

	static std::basic_string <C>& check_length (std::basic_string <C>& s)
	{
		if (BOUND && s.size () > BOUND)
			throw BAD_PARAM ();
		return s;
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT
	void adopt (C* s);
#endif
};

#ifdef LEGACY_STRING_MAPPING_SUPPORT

template <typename C, size_t BOUND>
void String_var <C, BOUND>::adopt (C* s)
{
	if (s) {
		size_t cc = std::char_traits <C>::length (s);
		if (BOUND && cc > BOUND)
			throw BAD_PARAM ();
		this->large_pointer (s);
		this->large_size (cc);
		size_t au = ::Nirvana::StdString::heap ()->query (s, ::Nirvana::Memory::ALLOCATION_UNIT);
		this->large_allocated (::Nirvana::round_up ((cc + 1) * sizeof (C), au));
	} else
		this->reset ();
}

template <typename C, size_t BOUND>
String_inout_base <C, BOUND>& String_inout_base <C, BOUND>::operator = (C* s)
{
	static_cast <String_var <C, BOUND>&> (s_) = s;
	return *this;
}

#endif

template <typename C>
void _check_bound (const String_in <C>& s, size_t max_length)
{
	assert (max_length);
	if (s.size () > max_length)
		throw BAD_PARAM ();
}

template <typename C>
const std::basic_string <C>& _unmarshal_in (const StringABI <C>* abi)
{
	return _unmarshal_inout (const_cast <StringABI <C>*> (abi));
}

template <size_t BOUND, typename C>
const std::basic_string <C>& _unmarshal_in (const StringABI <C>* abi)
{
	return _unmarshal_inout <BOUND, C> (const_cast <StringABI <C>*> (abi));
}

template <typename C>
std::basic_string <C>& _unmarshal_inout (StringABI <C>* abi)
{
	_check_pointer (abi);
	std::basic_string <C>& s (static_cast <std::basic_string <C>&> (*abi));
	s._unmarshal ();
	return s;
}

template <size_t BOUND, typename C>
String_inout <C, BOUND> _unmarshal_inout (StringABI <C>* abi)
{
	_check_pointer (abi);
	std::basic_string <C>& s (static_cast <std::basic_string <C>&> (*abi));
	s._unmarshal (BOUND);
	return String_inout <C, BOUND> (s);
}

template <typename C>
std::basic_string <C>& _unmarshal_out (StringABI <C>* abi)
{
	_check_pointer (abi);
	std::basic_string <C>& s (static_cast <std::basic_string <C>&> (*abi));
	if (!s.empty ())
		throw MARSHAL ();
	return s;
}

template <size_t BOUND, typename C>
String_out <C, BOUND> _unmarshal_out (StringABI <C>* abi)
{
	return String_out <C, BOUND> (_unmarshal_out <C> (abi));
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
void basic_string <C, T, allocator <C> >::_unmarshal (size_type max_size) const
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
	if (max_size && cc > max_size)
		throw CORBA::BAD_PARAM ();
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_unmarshal_or_clear ()
{
	try {
		_unmarshal ();
	} catch (...) {
		release_memory ();
		this->reset ();
		throw;
	}
}

template <typename C, class T>
template <size_t BOUND>
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::operator = (::CORBA::Nirvana::String_inout_base <C, BOUND>&& s)
{
	return operator = (s.move ());
}

}

#endif
