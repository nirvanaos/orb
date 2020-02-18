#ifndef NIRVANA_ORB_TYPEINTERFACE_H_
#define NIRVANA_ORB_TYPEINTERFACE_H_

#include "Type_forward.h"
#include "I_var.h"

namespace CORBA {
namespace Nirvana {

//! I_in helper class for interface
template <class I>
class I_in
{
public:
	I_in (I_ptr <I> ptr) :
		p_ (ptr.p_)
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)p_);
	}

	I_in (const I_var <I>& var) :
		p_ (var.p_)
	{}

	Interface* operator & () const
	{
		return p_;
	}

private:
	Interface* p_;
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
	I_inout (I_var <I>& var) :
		ref_ (reinterpret_cast <Interface*&> (var.p_))
	{}

	~I_inout () noexcept (false);

	Interface ** operator & () const
	{
		return &ref_;
	}

protected:
	/// Using I_ptr as inout parameter is error prone and prohibited.
	I_inout (I_ptr <I>& p) :
		ref_ (p.p_)
	{}

protected:
	Interface*& ref_;
};

// Outline for compact code
template <class I>
I_inout <I>::~I_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		I::_check (ref_);
	} catch (...) {
		interface_release (ref_);
		ref_ = nullptr;
		if (!ex)
			throw;
	}
}

template <>
class I_inout <Interface>
{
public:
	I_inout (I_var <Interface>& var) :
		ref_ (var.p_)
	{}

	~I_inout ()
	{}

	Interface** operator & () const
	{
		return &ref_;
	}

protected:
	/// Using I_ptr as inout parameter is error prone and prohibited.
	I_inout (I_ptr <Interface>& p) :
		ref_ (p.p_)
	{}

protected:
	Interface*& ref_;
};


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
		I_inout <I>::ref_ = 0;
	}

	I_out (I_var <I>& var) :
		I_inout <I> (var)
	{
		interface_release (I_inout <I>::ref_);
		I_inout <I>::ref_ = 0;
	}
};

//! I_out helper class for interface
template <>
class I_out <Interface> : public I_inout <Interface>
{
public:
	I_out (I_ptr <Interface>& p) :
		I_inout <Interface> (p)
	{
		I_inout <Interface>::ref_ = 0;
	}

	I_out (I_var <Interface>& var) :
		I_inout <Interface> (var)
	{
		interface_release (I_inout <Interface>::ref_);
		I_inout <Interface>::ref_ = 0;
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

	operator I_ptr <I> ()
	{
		return val_._retn ();
	}

	operator I_var <I> ()
	{
		return val_._retn ();
	}

private:
	I_var <I> val_;
};

//! I_ret helper class for interface
template <>
class I_ret <Interface>
{
public:
	I_ret (Interface* p) :
		val_ (p)
	{}

	operator I_ptr <Interface> ()
	{
		return val_._retn ();
	}

	operator I_var <Interface> ()
	{
		return val_._retn ();
	}

private:
	I_var <Interface> val_;
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

template <>
struct Type <I_var <Interface> >
{
	static const bool has_check = false;

	static void check (const I_var <Interface>& p)
	{}

	typedef Interface* ABI_in;
	typedef Interface** ABI_out;
	typedef Interface** ABI_inout;
	typedef Interface* ABI_ret;

	typedef I_var <Interface> C_var;
	typedef I_in <Interface> C_in;
	typedef I_out <Interface> C_out;
	typedef I_inout <Interface> C_inout;
	typedef I_ret <Interface> C_ret;

	static I_ptr <Interface> in (ABI_in p)
	{
		return p;
	}

	static I_var <Interface>& inout (ABI_inout p)
	{
		_check_pointer (p);
		return reinterpret_cast <I_var <Interface>&> (*p);
	}

	static I_var <Interface>& out (ABI_out p)
	{
		_check_pointer (p);
		if (*p)
			::Nirvana::throw_BAD_PARAM ();
		return reinterpret_cast <I_var <Interface>&> (*p);
	}
};

template <class I>
using TypeI = Type <I_var <I> >;

}
}

#endif
