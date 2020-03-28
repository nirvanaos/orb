#ifndef NIRVANA_ORB_TYPE_INTERFACE_H_
#define NIRVANA_ORB_TYPE_INTERFACE_H_

#include "Type_forward.h"
#include "I_var.h"

namespace CORBA {
namespace Nirvana {

typedef Interface* ABI_interface;

#ifdef NIRVANA_C11
template <class I> using I_in = I_ptr <I>;
#else
#define I_in I_ptr
#endif

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
		ref_ (reinterpret_cast <Interface*&> (p.p_))
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
		ref_ = 0;
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
		this->ref_ = 0;
	}

	I_out (I_var <I>& var) :
		I_inout <I> (var)
	{
		interface_release (this->ref_);
		this->ref_ = 0;
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
		this->ref_ = 0;
	}

	I_out (I_var <Interface>& var) :
		I_inout <Interface> (var)
	{
		interface_release (this->ref_);
		this->ref_ = 0;
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
	I_ret (Interface* p)
	{
		try {
			ptr_ = I::_check (p);
		} catch (...) {
			ptr_ = I::_nil ();
			interface_release (p);
			throw;
		}
	}

	~I_ret ()
	{
		release (ptr_);
	}

	operator I_ptr <I> ()
	{
		I_ptr <I> ret = ptr_;
		ptr_ = I::_nil ();
		return ret;
	}

	operator I_var <I> ()
	{
		I_ptr <I> ret = ptr_;
		ptr_ = I::_nil ();
		return ret;
	}

private:
	I_ptr <I> ptr_;
};

template <class I>
class I_VT_ret
{
public:
	I_VT_ret (Interface* p)
	{
		ptr_ = I::_check (p);
	}

	operator I_ptr <I> ()
	{
		return ptr_;
	}

private:
	I_ptr <I> ptr_;
};

//! I_ret helper class for interface
template <>
class I_ret <Interface>
{
public:
	I_ret (Interface* p) :
		ptr_ (p)
	{}

	~I_ret ()
	{
		interface_release (&ptr_);
	}

	operator I_ptr <Interface> ()
	{
		I_ptr <Interface> ret = ptr_;
		ptr_ = Interface::_nil ();
		return ret;
	}

	operator I_var <Interface> ()
	{
		I_ptr <Interface> ret = ptr_;
		ptr_ = Interface::_nil ();
		return ret;
	}

private:
	I_ptr <Interface> ptr_;
};

template <>
class I_VT_ret <Interface> :
	public I_ptr <Interface>
{
public:
	I_VT_ret (Interface* p) :
		I_ptr <Interface> (p)
	{}
};

typedef I_var <Interface> Interface_var;
typedef I_out <Interface> Interface_out;
typedef I_inout <Interface> Interface_inout;

template <class I>
struct Type <I_var <I> >
{
	typedef Interface* ABI_type;

	static const bool has_check = true;

	static void check (Interface* p)
	{
		I::_check (p);
	}

	typedef Interface* ABI_in;
	typedef Interface** ABI_out;
	typedef Interface** ABI_inout;
	typedef Interface* ABI_ret;
	typedef Interface* ABI_VT_ret;

	typedef I_var <I> C_var;
	typedef I_in <I> C_in;
	typedef I_out <I> C_out;
	typedef I_inout <I> C_inout;
	typedef I_ret <I> C_ret;
	typedef I_VT_ret <I> C_VT_ret;

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

	static Interface* ret (const I_ptr <I>& ptr)
	{
		return &ptr;
	}

	static Interface* ret (I_var <I>&& var)
	{
		return &var._retn ();
	}

	static Interface* VT_ret (const I_ptr <I>& ptr)
	{
		return &ptr;
	}

	// Valuetupe implementation mustn't return I_var
	static void VT_ret (I_var <I>&);
};

template <>
struct Type <I_var <Interface> >
{
	typedef Interface* ABI_type;

	static const bool has_check = false;

	static void check (Interface* p)
	{}

	typedef Interface* ABI_in;
	typedef Interface** ABI_out;
	typedef Interface** ABI_inout;
	typedef Interface* ABI_ret;
	typedef Interface* ABI_VT_ret;

	typedef I_var <Interface> C_var;
	typedef I_in <Interface> C_in;
	typedef I_out <Interface> C_out;
	typedef I_inout <Interface> C_inout;
	typedef I_ret <Interface> C_ret;
	typedef I_VT_ret <Interface> C_VT_ret;

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

	static Interface* ret (const I_ptr <Interface>& ptr)
	{
		return &ptr;
	}

	static Interface* ret (I_var <Interface>&& var)
	{
		return &var._retn ();
	}

	static Interface* VT_ret (const I_ptr <Interface>& ptr)
	{
		return &ptr;
	}

	// Valuetupe implementation mustn't return I_var
	static void VT_ret (I_var <Interface>&);
};

template <class I>
using TypeI = Type <I_var <I> >;

}
}

#endif
