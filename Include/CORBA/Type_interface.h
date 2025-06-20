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
#include "I_ref.h"
#ifdef LEGACY_CORBA_CPP
#include "I_var.h"
#endif
#include "MarshalHelper.h"

namespace CORBA {

class TypeCode;

namespace Internal {

template <class Itf> using I_in = I_ptr <Itf>;

//! I_inout helper class for interface
template <class Itf>
class I_inout
{
public:
	I_inout (I_ref <Itf>& var) :
		ref_ (reinterpret_cast <Interface*&> (var.p_))
	{}

	~I_inout () noexcept (false);

	Interface ** operator & () const
	{
		return &ref_;
	}

protected:
	/// Using I_ptr as inout parameter is error prone and prohibited.
	I_inout (I_ptr <Itf>& p) :
		ref_ (reinterpret_cast <Interface*&> (p.p_))
	{}

protected:
	Interface*& ref_;
};

// Outline for compact code
template <class Itf>
I_inout <Itf>::~I_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		Interface::_check (ref_, RepIdOf <Itf>::id);
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
template <class Itf>
class I_out : public I_inout <Itf>
{
public:
#ifdef LEGACY_CORBA_CPP
	I_out (I_ptr <Itf>& p) :
		I_inout <Itf> (p)
	{
		this->ref_ = 0;
	}
#endif

	I_out (I_ref <Itf>& var) :
		I_inout <Itf> (var)
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

template <class Itf> inline
I_inout <Itf> I_var <Itf>::inout ()
{
	return I_inout <Itf> (*this);
}

template <class Itf> inline
I_out <Itf> I_var <Itf>::out ()
{
	return I_out <Itf> (*this);
}

#endif

//! I_ret helper class for interface
template <class Itf>
class I_ret
{
public:
	I_ret (Interface* p) :
		p_ (p)
	{}

	~I_ret ()
	{
		interface_release (p_);
	}

#ifndef LEGACY_CORBA_CPP

	operator I_ref <Itf> ()
	{
		// Itf may be not completely defined here, so we use reinterpret_cast.
		// Internal::Interface is always the top base of any interface bridge.
		I_ref <Itf> ret (reinterpret_cast <Itf*> (Interface::_check (p_, RepIdOf <Itf>::id))); // No add reference
		p_ = nullptr;
		return ret;
	}

#else

	operator I_ptr <Itf> ()
	{
		// Itf may be not completely defined here, so we use reinterpret_cast.
		// Internal::Interface is always the top base of any interface bridge.
		I_ptr <Itf> ret (reinterpret_cast <Itf*> (Interface::_check (p_, RepIdOf <Itf>::id))); // No add reference
		p_ = nullptr;
		return ret;
	}

#endif

private:
	Interface* p_;
};

template <class Itf>
class I_VT_ret
{
public:
	I_VT_ret (Interface* p) :
		p_ (p)
	{}

	operator I_ptr <Itf> ()
	{
		// Itf may be not completely defined here, so we use reinterpret_cast.
		// Internal::Interface is always the top base of any interface bridge.
		return I_ptr <Itf> (reinterpret_cast <Itf*> (Interface::_check (p_, RepIdOf <Itf>::id)));
	}

private:
	Interface* p_;
};

//! I_ret helper class for interface
template <>
class I_ret <Interface>
{
public:
	I_ret (Interface* p) :
		p_ (p)
	{}

	~I_ret ()
	{
		interface_release (p_);
	}

#ifndef LEGACY_CORBA_CPP

	operator I_ref <Interface> ()
	{
		I_ref <Interface> ret (p_);
		p_ = nullptr;
		return ret;
	}

#else

	operator I_ptr <Interface> ()
	{
		I_ptr <Interface> ret = p_;
		p_ = nullptr;
		return ret;
	}

#endif

private:
	Interface* p_;
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

template <class Itf>
struct TypeItfBase
{
	static const bool is_CDR = false;
	static const bool is_CDR_seq = false;
	static const bool is_var_len = true;
	static const size_t CDR_size = sizeof (void*);
	static const size_t CDR_align = alignof (void*);

	typedef Interface* ABI;

	typedef Interface* ABI_in;
	typedef Interface** ABI_out;
	typedef Interface* ABI_ret;
	typedef Interface* ABI_VT_ret;

	typedef I_ptr <Itf> C_ptr;
#ifdef LEGACY_CORBA_CPP
	typedef I_var <Itf> Var;
	typedef I_ptr <Itf> VRet;
#else
	typedef I_ref <Itf> Var;
	typedef Var VRet;
#endif
	typedef I_in <Itf> C_in;
	typedef I_out <Itf> C_out;
	typedef I_inout <Itf> C_inout;
	typedef I_ret <Itf> C_ret;
	typedef I_VT_ret <Itf> C_VT_ret;

#ifdef LEGACY_CORBA_CPP

	static Interface* ret (const I_ptr <Itf>& ptr)
	{
		return &ptr;
	}

	static I_var <Itf>& out (ABI_out p)
	{
		check_pointer (p);
		if (*p)
			::Nirvana::throw_BAD_PARAM ();
		return reinterpret_cast <I_var <Itf>&> (*p);
	}

#else

	static I_ref <Itf>& out (ABI_out p)
	{
		check_pointer (p);
		if (*p)
			::Nirvana::throw_BAD_PARAM ();
		return reinterpret_cast <I_ref <Itf>&> (*p);
	}

#endif

	static Interface* ret (I_ref <Itf>&& var) noexcept
	{
		Interface* p = &I_ptr <Itf> (var.p_);
		var.p_ = nullptr;
		return p;
	}

	static Interface* ret () noexcept
	{
		return nullptr;
	}

	static Interface* VT_ret (const I_ptr <Itf>& ptr) noexcept
	{
		return &ptr;
	}

	static Interface* VT_ret () noexcept
	{
		return nullptr;
	}

	NIRVANA_DEPRECATED ("Valuetupe implementation for state members must return I_ptr, not I_ref")
	static void VT_ret (I_ref <Itf>&);

	typedef I_ptr <Itf> ConstRef;
};

template <class Itf>
struct TypeItfCommon : TypeItfBase <Itf>
{
	typedef TypeItfBase <Itf> Base;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;

	static const bool has_check = true;

	static Itf* check (Interface* p)
	{
		return reinterpret_cast <Itf*> (Interface::_check (p, RepIdOf <Itf>::id));
	}

	static I_ptr <Itf> in (ABI_in p)
	{
		return check (p);
	}

#ifdef LEGACY_CORBA_CPP

	static I_var <Itf>& inout (ABI_out p)
	{
		check_pointer (p);
		check (*p);
		return reinterpret_cast <I_var <Itf>&> (*p);
	}

#else

	static I_ref <Itf>& inout (ABI_out p)
	{
		check_pointer (p);
		check (*p);
		return reinterpret_cast <I_ref <Itf>&> (*p);
	}

#endif
};

template <class Itf>
struct TypeItf : TypeItfCommon <Itf>
{};

template <class Itf>
struct TypeItfMarshalable :
	TypeItfCommon <Itf>,
	MarshalHelper <Itf, I_ref <Itf> >
{
	// For interfaces, in and out parameters marshalled in the same manner.
	static void marshal_out (I_ref <Itf>& src, IORequest_ptr rq)
	{
		Type <Itf>::marshal_in (src, rq);
	}
};

template <>
struct TypeItf <TypeCode> : TypeItfMarshalable <TypeCode>
{
	inline static void marshal_in (I_ptr <TypeCode> src, IORequest_ptr rq);
	inline static void unmarshal (IORequest_ptr rq, I_ref <TypeCode>& dst);
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
		check_pointer (p);
		return reinterpret_cast <I_ref <Interface>&> (*p);
	}
};

template <>
struct Type <Interface> : TypeItf <Interface>
{};

// For sequences and arrays map reference type to interface type

#ifndef LEGACY_CORBA_CPP

template <class Itf>
struct Type <I_ref <Itf> > : public Type <Itf>
{};

#else

template <class Itf>
struct Type <I_var <Itf> > : public Type <Itf>
{};

#endif

}
}

#endif
