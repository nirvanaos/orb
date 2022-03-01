/// \file
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_ORB_TYPE_INTERFACE_H_
#define NIRVANA_ORB_TYPE_INTERFACE_H_
#pragma once

#include "Type_forward.h"
#include "I_var.h"
#include "TypeVarLenHelper.h"

namespace CORBA {

class TypeCode;

namespace Internal {

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
	I_inout (I_ref <I>& var) :
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
	I_inout (I_ref <Interface>& var) :
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

//! I_out helper class for interface
template <class I>
class I_out : public I_inout <I>
{
public:
#ifdef LEGACY_CORBA_CPP
	I_out (I_ptr <I>& p) :
		I_inout <I> (p)
	{
		this->ref_ = 0;
	}
#endif

	I_out (I_ref <I>& var) :
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
	I_out (I_ref <Interface>& var) :
		I_inout <Interface> (var)
	{
		interface_release (this->ref_);
		this->ref_ = 0;
	}
};

#ifdef LEGACY_CORBA_CPP

template <class I> inline
I_inout <I> I_var <I>::inout ()
{
	return I_inout <I> (*this);
}

template <class I> inline
I_out <I> I_var <I>::out ()
{
	return I_out <I> (*this);
}

#endif

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
		interface_release (&ptr_);
	}

	operator I_ref <I> ()
	{
		I_ref <I> ret (ptr_.p_);
		ptr_ = I::_nil ();
		return ret;
	}

#ifdef LEGACY_CORBA_CPP

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

#endif

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
		interface_release (ptr_);
	}

	operator I_ref <Interface> ()
	{
		I_ref <Interface> ret (ptr_);
		ptr_ = nullptr;
		return ret;
	}

#ifdef LEGACY_CORBA_CPP

	operator I_ptr <Interface> ()
	{
		I_ptr <Interface> ret = ptr_;
		ptr_ = nullptr;
		return ret;
	}

	operator I_var <Interface> ()
	{
		I_ptr <Interface> ret = ptr_;
		ptr_ = nullptr;
		return ret;
	}

#endif

private:
	Interface* ptr_;
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

template <class I>
struct TypeItfBase
{
	static const bool fixed_len = false;

	typedef Interface* ABI;

	typedef Interface* ABI_in;
	typedef Interface** ABI_out;
	typedef Interface* ABI_ret;
	typedef Interface* ABI_VT_ret;

	typedef I_ptr <I> C_ptr;
#ifdef LEGACY_CORBA_CPP
	typedef I_var <I> Var;
	typedef I_var <I> C_var;
	typedef I_ptr <I> VRet;
#else
	typedef I_ref <I> Var;
	typedef Var VRet;
#endif
	typedef I_in <I> C_in;
	typedef I_out <I> C_out;
	typedef I_inout <I> C_inout;
	typedef I_ret <I> C_ret;
	typedef I_VT_ret <I> C_VT_ret;

#ifdef LEGACY_CORBA_CPP

	static Interface* ret (const I_ptr <I>& ptr)
	{
		return &ptr;
	}

	static I_var <I>& out (ABI_out p)
	{
		_check_pointer (p);
		if (*p)
			::Nirvana::throw_BAD_PARAM ();
		return reinterpret_cast <I_var <I>&> (*p);
	}

#else

	static I_ref <I>& out (ABI_out p)
	{
		_check_pointer (p);
		if (*p)
			::Nirvana::throw_BAD_PARAM ();
		return reinterpret_cast <I_ref <I>&> (*p);
	}

#endif

	static Interface* ret (I_ref <I>&& var) NIRVANA_NOEXCEPT
	{
		Interface* p = var.p_;
		var.p_ = nullptr;
		return p;
	}

	static Interface* ret () NIRVANA_NOEXCEPT
	{
		return nullptr;
	}

	static Interface* VT_ret (const I_ptr <I>& ptr) NIRVANA_NOEXCEPT
	{
		return &ptr;
	}

	static Interface* VT_ret () NIRVANA_NOEXCEPT
	{
		return nullptr;
	}

	// Valuetupe implementation for state members must return I_ptr, not I_ref.
	// Otherwise compilation error will occur.
	static void VT_ret (I_ref <I>&);

	typedef I_ptr <I> ConstRef;
};

template <class I>
struct TypeItfCommon : TypeItfBase <I>
{
	typedef TypeItfBase <I> Base;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;

	static const bool has_check = true;

	static void check (Interface* p)
	{
		I::_check (p);
	}

	static I_ptr <I> in (ABI_in p)
	{
		return I::_check (p);
	}

#ifdef LEGACY_CORBA_CPP

	static I_var <I>& inout (ABI_out p)
	{
		_check_pointer (p);
		I::_check (*p);
		return reinterpret_cast <I_var <I>&> (*p);
	}

#else

	static I_ref <I>& inout (ABI_out p)
	{
		_check_pointer (p);
		I::_check (*p);
		return reinterpret_cast <I_ref <I>&> (*p);
	}

#endif
};

template <class I>
struct TypeItf : TypeItfCommon <I>
{};

template <class I>
struct TypeItfMarshalable :
	TypeItfCommon <I>,
	TypeVarLenHelper <I, I_ref <I> >
{
	// By default, for interfaces, in and out parameters marshalled in the same manner.
	// But this method is overridden for value types.
	static void marshal_out (I_ref <I>& src, IORequest_ptr rq)
	{
		Type <I>::marshal_in (src, rq);
	}
};

template <>
struct TypeItf <TypeCode> : TypeItfMarshalable <TypeCode>
{
	static void marshal_in (I_ptr <TypeCode> src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, I_ref <TypeCode>& dst);
};

template <>
struct TypeItf <Interface> : TypeItfBase <Interface>
{
	static const bool has_check = false;

	static void check (Interface*)
	{}

	typedef Interface* ABI_in;
	typedef Interface** ABI_out;
	typedef Interface* ABI_ret;
	typedef Interface* ABI_VT_ret;

#ifdef LEGACY_CORBA_CPP
	typedef I_var <Interface> C_var;
#endif

	typedef I_in <Interface> C_in;
	typedef I_out <Interface> C_out;
	typedef I_inout <Interface> C_inout;
	typedef I_ret <Interface> C_ret;
	typedef I_VT_ret <Interface> C_VT_ret;

	static I_ptr <Interface> in (ABI_in p)
	{
		return p;
	}

	static I_ref <Interface>& inout (ABI_out p)
	{
		_check_pointer (p);
		return reinterpret_cast <I_ref <Interface>&> (*p);
	}
};

template <>
struct Type <Interface> : TypeItf <Interface>
{};

template <class I>
struct Type <I_ref <I> > : public Type <I>
{};

}
}

#endif
