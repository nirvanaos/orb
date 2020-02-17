#ifndef NIRVANA_ORB_TYPEINTERFACE_H_
#define NIRVANA_ORB_TYPEINTERFACE_H_

#include "Type_forward.h"
#include "I_var.h"

namespace CORBA {
namespace Nirvana {

//! I_in helper class for interface
template <class I>
class I_in : public I_ptr <I>
{
public:
	I_in (I_ptr <I> p) :
		I_ptr <I> (p)
	{}

	Interface* operator & () const
	{
		return *this;
	}
};

template <class I> inline
I_in <I> I_var <I>::in () const
{
	return *this;
}

//! I_inout helper class for interface
template <class I>
class I_inout
{
public:
	I_inout (I_var <I>& val) :
		I_inout (static_cast <I_ptr <I>&> (val))
	{}

	~I_inout () noexcept (false);

	Interface ** operator & () const
	{
		return reinterpret_cast <Interface**> (&ref_.p_);
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
		I::_check (ref_);
	} catch (...) {
		release (ref_);
		ref_ = I_ptr <I>::nil ();
		if (!ex)
			throw;
	}
}

template <class I> inline
I_inout <I> I_var <I>::inout ()
{
	return I_inout <I> (*this);
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

template <class I> inline
I_out <I> I_var <I>::out ()
{
	return I_out <I> (*this);
}

//! I_ret helper class for interface
template <class I>
class I_ret
{
public:
	I_ret (Interface* p) :
		val_ (I::_check (p))
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
		I::_check (p);
	}

	typedef Interface* ABI_in;
	typedef Interface** ABI_out;
	typedef Interface** ABI_inout;
	typedef Interface* ABI_ret;

	typedef I_var <I> C_var;
	typedef I_in <I> C_in;
	typedef I_out <I> C_out;
	typedef I_inout <I> C_inout;
	typedef I_ret <I> C_ret;

	static I_ptr <I> in (ABI_in p)
	{
		return I::_check (p);
	}

	static I_var <I>& inout (ABI_inout p)
	{
		_check_pointer (p);
		I::_check (*p);
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
