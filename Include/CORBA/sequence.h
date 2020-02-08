// Nirvana project
// Sequence - CORBA sequence declarations

#ifndef NIRVANA_ORB_SEQUENCE_H_
#define NIRVANA_ORB_SEQUENCE_H_

#include "Nirvana/vector.h"

namespace CORBA {
namespace Nirvana {

template <typename T>
using Sequence_in = const std::vector <T>&;

template <typename T>
class Sequence_inout_base
{
public:
	Sequence_inout_base (std::vector <T>& s) :
		s_ (s)
	{}

	Sequence_inout_base (const Sequence_inout_base& s) :
		s_ (s.s_)
	{}

	~Sequence_inout_base () noexcept (false);

	SequenceABI <T>* operator & () const
	{
		return &s_;
	}

	operator std::vector <T>& () const
	{
		return s_;
	}

	size_t size () const
	{
		return s_.size ();
	}

private:
	std::vector <T>& s_;
};

template <typename T> // Outline for compact code
Sequence_inout_base <T>::~Sequence_inout_base () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		s_._check_or_clear ();
	} catch (...) {
		if (!ex)
			throw;
	}
}

template <typename T>
class Sequence_inout : public Sequence_inout_base <T>
{
public:
	Sequence_inout (std::vector <T>& s) :
		Sequence_inout_base <T> (s)
	{}

	Sequence_inout (const Sequence_inout& s) :
		Sequence_inout_base <T> (s)
	{}
};

template <typename T>
class Sequence_out : public Sequence_inout_base <T>
{
public:
	Sequence_out (std::vector <T>& s) :
		Sequence_inout_base <T> (s)
	{
		s.clear ();
	}

	Sequence_out (const Sequence_out& s) :
		Sequence_inout_base <T> (s)
	{}
};

template <typename T>
class Sequence_var : public std::vector <T>
{
public:
	Sequence_var (SequenceABI <T>&& src)
	{
		this->data_ = src.data_;
		src.reset ();
		this->_check_or_clear ();
	}

	std::vector <T> _retn ()
	{
		return std::move (*this);
	}
};

template <typename T>
const std::vector <T>& _unmarshal_in (const SequenceABI <T>* abi)
{
	_check_pointer (abi);
	const std::vector <T>& v (static_cast <const std::vector <T>&> (*abi));
	v._check ();
	return v;
}

template <size_t BOUND, typename T>
const std::vector <T>& _unmarshal_in (const SequenceABI <T>* abi)
{
	const std::vector <T>& s = _unmarshal_in (abi);
	_check_bound (s, BOUND);
	return s;
}

template <typename T>
std::vector <T>& _unmarshal_inout (SequenceABI <T>* abi)
{
	_check_pointer (abi);
	std::vector <T>& s (static_cast <std::vector <T>&> (*abi));
	s._check ();
	return s;
}

template <size_t BOUND, typename T>
std::vector <T>& _unmarshal_inout (SequenceABI <T>* abi)
{
	std::vector <T>& s = _unmarshal_inout (abi);
	_check_bound (s, BOUND);
	return s;
}

template <typename T>
std::vector <T>& _unmarshal_out (SequenceABI <T>* abi)
{
	_check_pointer (abi);
	std::vector <T>& s (static_cast <std::vector <T>&> (*abi));
	if (!s.empty ())
		throw MARSHAL ();
	return s;
}

template <size_t BOUND, typename T>
std::vector <T>& _unmarshal_out (SequenceABI <T>* abi)
{
	return _unmarshal_out (abi);
}

}
} // namespace CORBA

namespace std {

template <class T>
void vector <T, allocator <T> >::_check () const
{
	// Do some check
	const_pointer p = data ();
	if (p)
		CORBA::Nirvana::_check_pointer (p);
	size_type cnt = size ();
	if (cnt > 0 && (cnt > this->capacity () || !heap ()->is_readable (p, cnt * sizeof (value_type))))
		::Nirvana::throw_MARSHAL ();
}

template <class T>
void vector <T, allocator <T> >::_check_or_clear ()
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
