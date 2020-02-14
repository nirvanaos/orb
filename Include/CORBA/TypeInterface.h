#ifndef NIRVANA_ORB_TYPEINTERFACE_H_
#define NIRVANA_ORB_TYPEINTERFACE_H_

#include "Type_forward.h"
#include "Interface_forward.h"

namespace CORBA {
namespace Nirvana {

template <class I> class I_out;
template <class I> class I_inout;

//! I_var helper class for interface
template <class I>
class I_var : public I_ptr <I>
{
public:
	I_var () :
		I_ptr <I> (I::_nil ())
	{}

	I_var (I_ptr <I> p) :
		I_ptr <I> (p)
	{}

	I_var (const I_var <I>& src) :
		I_ptr <I> (I::_duplicate (src))
	{}

	I_var (I_var <I>&& src) NIRVANA_NOEXCEPT :
		I_ptr <I> (src)
	{
		static_cast <I_ptr <I>&> (src) = I::_nil ();
	}

	~I_var ()
	{
		release (*this);
	}

	I_var <I>& operator = (I_ptr <I> p)
	{
		reset (p);
		return *this;
	}

	I_var <I>& operator = (const I_var <I>& src)
	{
		if (&src != this) {
			reset (I::_nil ());
			I_ptr <I>::operator = (I::_duplicate (src));
		}
		return *this;
	}

	I_var <I>& operator = (I_var <I>&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			reset (src);
			static_cast <I_ptr <I>&> (src) = I::_nil ();
		}
		return *this;
	}

	I_ptr <I> in () const
	{
		return *this;
	}

	I_inout <I> inout ()
	{
		return I_inout <I> (*this);
	}

	I_out <I> out ()
	{
		return I_out <I> (*this);
	}

	I_ptr <I> _retn ()
	{
		I_ptr <I> p = *this;
		I_ptr <I>::operator = (I_ptr <I>::nil ());
		return p;
	}

	operator BridgeMarshal <I>* ()
	{
		BridgeMarshal <I>* p = I_ptr <I>::operator Bridge <I>* ();
		I_ptr <I>::operator = (I_ptr <I>::nil ());
		return p;
	}

protected:
	void reset (I_ptr <I> p)
	{
		release (*this);
		I_ptr <I>::operator = (p);
	}
};

template <class I>
inline I_ptr <I>::I_ptr (I_var <I>&& var)
{
	p_ = var.p_;
	static_cast <I_ptr <I>&> (var).p_ = nullptr;
}

//! I_in helper class for interface
template <class I>
class I_in : public I_ptr <I>
{
public:
	I_in (I_ptr <I> p) :
		I_ptr <I> (p)
	{}

	BridgeMarshal <I>* operator & () const
	{
		return *this;
	}
};

//! I_inout helper class for interface
template <class I>
class I_inout
{
public:
	I_inout (I_var <I>& val) :
		I_inout (static_cast <I_ptr <I>&> (val))
	{}

	~I_inout () noexcept (false);

	BridgeMarshal <I>** operator & () const
	{
		return reinterpret_cast <BridgeMarshal <I>**> (&ref_.p_);
	}

protected:
	/// Using I_ptr as inout parameter is error prone and prohibited.
	I_inout (I_ptr <I>& p) :
		ref_ (p)
	{}

protected:
	I_ptr <I>& ref_;
};

// Outline for compact code
template <class I>
I_inout <I>::~I_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		I::unmarshal (ref_);
	} catch (...) {
		Interface::_release (ref_);
		ref_ = I_ptr <I>::nil ();
		if (!ex)
			throw;
	}
}

//! I_out helper class for interface
template <class I>
class I_out : public I_inout <I>
{
public:
	I_out (I_ptr <I>& p) :
		I_inout <I> (p)
	{
		I_inout <I>::ref_ = I::_nil ();
	}

	I_out (I_var <I>& val) :
		I_inout <I> (val)
	{
		release (I_inout <I>::ref_);
		I_inout <I>::ref_ = I_ptr <I>::nil ();
	}
};

//! I_ret helper class for interface
template <class I>
class I_ret
{
public:
	I_ret (BridgeMarshal <I>* p) :
		val_ (I::unmarshal (p))
	{}

	operator I_var <I> ()
	{
		return val_._retn ();
	}

private:
	I_var <I> val_;
};

typedef I_var <Interface> Interface_var;
typedef I_out <Interface> Interface_out;
typedef I_inout <Interface> Interface_inout;

template <class I>
struct Type <I_var <I> >
{
	static const bool has_check = true;

	static void check (const I_var <I>& p)
	{
		I::unmarshal (p);
	}

	typedef BridgeMarshal <I>* ABI_in;
	typedef BridgeMarshal <I>** ABI_out;
	typedef BridgeMarshal <I>** ABI_inout;
	typedef BridgeMarshal <I>* ABI_ret;

	typedef I_var <I> C_var;
	typedef I_in <I> C_in;
	typedef I_out <I> C_out;
	typedef I_inout <I> C_inout;
	typedef I_ret <I> C_ret;

	static I_ptr <I> in (ABI_in p)
	{
		return I::unmarshal (p);
	}

	static I_var <I>& inout (ABI_inout p)
	{
		_check_pointer (p);
		I::unmarshal (*p);
		return reinterpret_cast <I_var <I>&> (*p);
	}

	static I_var <I>& out (ABI_out p)
	{
		_check_pointer (p);
		if (*p)
			::Nirvana::throw_BAD_PARAM ();
		return reinterpret_cast <I_var <I>&> (*p);
	}
};

template <class I>
using TypeI = Type <I_var <I> >;

}
}

#endif
