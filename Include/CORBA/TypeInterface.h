#ifndef NIRVANA_ORB_TYPEINTERFACE_H_
#define NIRVANA_ORB_TYPEINTERFACE_H_

#include "Type_forward.h"
#include "Interface_forward.h"

namespace CORBA {
namespace Nirvana {

template <class I> class T_out;
template <class I> class T_inout;

//! T_var helper class for interface
template <class I>
class T_var : public T_ptr <I>
{
public:
	T_var () :
		T_ptr <I> (I::_nil ())
	{}

	T_var (T_ptr <I> p) :
		T_ptr <I> (p)
	{}

	T_var (const T_var <I>& src) :
		T_ptr <I> (I::_duplicate (src))
	{}

	T_var (T_var <I>&& src) NIRVANA_NOEXCEPT :
		T_ptr <I> (src)
	{
		static_cast <T_ptr <I>&> (src) = I::_nil ();
	}

	~T_var ()
	{
		release (*this);
	}

	T_var <I>& operator = (T_ptr <I> p)
	{
		reset (p);
		return *this;
	}

	T_var <I>& operator = (const T_var <I>& src)
	{
		if (&src != this) {
			reset (I::_nil ());
			T_ptr <I>::operator = (I::_duplicate (src));
		}
		return *this;
	}

	T_var <I>& operator = (T_var <I>&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			reset (src);
			static_cast <T_ptr <I>&> (src) = I::_nil ();
		}
		return *this;
	}

	T_ptr <I> in () const
	{
		return *this;
	}

	T_inout <I> inout ()
	{
		return T_inout <I> (*this);
	}

	T_out <I> out ()
	{
		return T_out <I> (*this);
	}

	T_ptr <I> _retn ()
	{
		T_ptr <I> p = *this;
		T_ptr <I>::operator = (T_ptr <I>::nil ());
		return p;
	}

protected:
	void reset (T_ptr <I> p)
	{
		release (*this);
		T_ptr <I>::operator = (p);
	}
};

template <class I>
inline T_ptr <I>::T_ptr (T_var <I>&& var)
{
	p_ = var.p_;
	static_cast <T_ptr <I>&> (var).p_ = nullptr;
}

//! T_in helper class for interface
template <class I>
class T_in : public T_ptr <I>
{
public:
	T_in (T_ptr <I> p) :
		T_ptr <I> (p)
	{}

	BridgeMarshal <I>* operator & () const
	{
		return *this;
	}
};

//! T_inout helper class for interface
template <class I>
class T_inout
{
public:
	T_inout (T_var <I>& val) :
		T_inout (static_cast <T_ptr <I>&> (val))
	{}

	~T_inout () noexcept (false);

	BridgeMarshal <I>** operator & () const
	{
		return reinterpret_cast <BridgeMarshal <I>**> (&ref_.p_);
	}

protected:
	/// Using T_ptr as inout parameter is error prone and prohibited.
	T_inout (T_ptr <I>& p) :
		ref_ (p)
	{}

protected:
	T_ptr <I>& ref_;
};

// Outline for compact code
template <class I>
T_inout <I>::~T_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		I::unmarshal (ref_);
	} catch (...) {
		Interface::_release (ref_);
		ref_ = T_ptr <I>::nil ();
		if (!ex)
			throw;
	}
}

//! T_out helper class for interface
template <class I>
class T_out : public T_inout <I>
{
public:
	T_out (T_ptr <I>& p) :
		T_inout <I> (p)
	{
		T_inout <I>::ref_ = I::_nil ();
	}

	T_out (T_var <I>& val) :
		T_inout <I> (val)
	{
		release (T_inout <I>::ref_);
		T_inout <I>::ref_ = T_ptr <I>::nil ();
	}
};

//! T_ret helper class for interface
template <class I>
class T_ret
{
public:
	T_ret (BridgeMarshal <I>* p) :
		val_ (I::unmarshal (p))
	{}

	operator T_var <I> ()
	{
		return val_._retn ();
	}

private:
	T_var <I> val_;
};

typedef T_var <Interface> Interface_var;
typedef T_out <Interface> Interface_out;
typedef T_inout <Interface> Interface_inout;

template <class I>
struct Type <T_var <I> >
{
	static const bool has_check = true;

	static void check (const T_var <I>& p)
	{
		I::unmarshal (p);
	}

	typedef BridgeMarshal <I>* ABI_in;
	typedef BridgeMarshal <I>** ABI_out;
	typedef BridgeMarshal <I>** ABI_inout;
	typedef BridgeMarshal <I>* ABI_ret;

	typedef T_var <I> C_var;
	typedef T_in <I> C_in;
	typedef T_out <I> C_out;
	typedef T_inout <I> C_inout;
	typedef T_ret <I> C_ret;

	static T_ptr <I> in (ABI_in p)
	{
		return I::unmarshal (p);
	}

	static T_var <I>& inout (ABI_inout p)
	{
		_check_pointer (p);
		I::unmarshal (*p);
		return reinterpret_cast <T_var <I>&> (*p);
	}

	static T_var <I>& out (ABI_out p)
	{
		_check_pointer (p);
		if (*p)
			::Nirvana::throw_BAD_PARAM ();
		return reinterpret_cast <T_var <I>&> (*p);
	}
};

template <class I>
using TypeI = Type <T_var <I> >;

}
}

#endif
