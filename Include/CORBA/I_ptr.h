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
#ifndef NIRVANA_ORB_I_PTR_H_
#define NIRVANA_ORB_I_PTR_H_
#pragma once

#include "Bridge.h"

namespace CORBA {

template <class>
class servant_reference;

namespace Internal {

template <class I> class I_ptr;
template <class I> class I_ref_base;
template <class I> class I_ref;
template <class I> class I_var;
template <class I> class I_inout;
template <class I> class I_ret;
template <class I> class ClientInterfacePrimary;
template <class VB, typename T> class ValueBox;

#ifndef NDEBUG
const uintptr_t UNINITIALIZED_PTR = 1;
#endif

template <class I>
class I_ptr_base
{
public:
	typedef I ItfType;

	/// Zero init skipped for performance
	I_ptr_base () noexcept
#ifdef _DEBUG
		: p_ ((I*)UNINITIALIZED_PTR)
#endif
	{}

	I_ptr_base (I* p) noexcept :
		p_ (p)
	{}

	I_ptr_base (nullptr_t) noexcept :
		p_ (nullptr)
	{}

	I_ptr_base (const I_ptr_base& src) noexcept :
		p_ (src.p_)
	{}

	I_ptr_base (const I_ref <I>& src) noexcept;

	I_ptr_base& operator = (const I_ptr_base& src) noexcept
	{
		p_ = src.p_;
		return *this;
	}

	I* operator -> () const
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)p_);
		if (!p_)
			::Nirvana::throw_INV_OBJREF ();
		return p_;
	}

	explicit operator bool () const noexcept
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)p_);
		return p_ != 0;
	}

	explicit operator bool () noexcept
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)p_);
		return p_ != 0;
	}

protected:
	void move_from (I_ref <I>& src) noexcept;

protected:
	friend class I_ref_base <I>;
	template <class I1> friend class I_var;
	template <class I1> friend class I_ref;
	friend class I_inout <I>;
	friend class I_ret <I>;
	template <class I1> friend class I_ptr;

#ifdef LEGACY_CORBA_CPP
	friend class I_var <I>;
#endif

	I* p_;
};

//! Interface pointer template.
template <class I>
class I_ptr : public I_ptr_base <I>
{
	typedef I_ptr_base <I> Base;

public:
	I_ptr () noexcept
	{}

	I_ptr (I* p) noexcept :
		Base (p)
	{}

	I_ptr (nullptr_t) noexcept :
		Base (nullptr)
	{}

	I_ptr (const I_ptr& src) noexcept :
		Base (src)
	{}

	template <class I1>
	I_ptr (const I_ptr <I1>& src) :
		Base (wide (src.p_))
	{}

	I_ptr (const I_ref <I>& src) noexcept :
		Base (src)
	{}

	template <class I1>
	I_ptr (const I_ref <I1>& src);

#ifdef LEGACY_CORBA_CPP

	/// Move constructor in case returned I_ref assigned to I_ptr:
	///    I_ref <Object> func ();
	///    Object_ptr obj = func ();
	NIRVANA_DEPRECATED ("Potentially unsafe conversion from reference to pointer")
	I_ptr (I_ref <I>&& src) noexcept
	{
		this->move_from (src);
	}

#endif

	I_ptr (BridgeVal <I>* p) noexcept :
		Base (static_cast <I*> (static_cast <Bridge <I>*> (p)))
	{}

	template <class VB, typename T>
	I_ptr (ValueBox <VB, T>* p) noexcept :
		I_ptr (static_cast <I_ptr <I> > (*p))
	{}

	template <class S>
	I_ptr (const servant_reference <S>& sr) noexcept :
		I_ptr (static_cast <S*> (sr))
	{}

	I_ptr& operator = (const I_ptr& src) noexcept
	{
		Base::operator = (src);
		return *this;
	}

	I_ptr& operator = (nullptr_t) noexcept
	{
		Base::operator = (nullptr);
		return *this;
	}

	template <class I1>
	I_ptr& operator = (const I_ptr <I1>& src)
	{
		Base::operator = (wide (src.p_));
		return *this;
	}

	I_ptr& operator = (BridgeVal <I>* p) noexcept
	{
		return operator = (I_ptr (p));
	}

	template <class VB, typename T>
	I_ptr& operator = (ValueBox <VB, T>* p) noexcept
	{
		return operator = (I_ptr (p));
	}

	template <class S>
	I_ptr& operator = (const servant_reference <S>& sr) noexcept
	{
		return operator = (static_cast <BridgeVal <I>*> (static_cast <S*> (sr)));
	}

	Interface* operator & () const noexcept
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		// I may be not completely defined so we use reinterpret_cast
		return reinterpret_cast <Interface*> (this->p_);
	}

private:
	// Calls EPV function to obtain pointer to the base interface.
	template <class I1>
	static I* wide (I1* p)
	{
		return p ? static_cast <I*> (&static_cast <I_ptr <I> > (*p)) : nullptr;
	}
};

template <class S, class T> class ValueBox;

template <>
class I_ptr <Interface> : public I_ptr_base <Interface>
{
	typedef I_ptr_base <Interface> Base;

public:
	I_ptr () noexcept
	{}

	I_ptr (Interface* p) noexcept :
		Base (p)
	{}

	I_ptr (nullptr_t) noexcept :
		Base (nullptr)
	{}

	I_ptr (const I_ptr& src) noexcept :
		Base (src)
	{}

	template <class I>
	I_ptr (const I_ptr <I>& src) :
		Base (src.p_)
	{}

	I_ptr (const I_ref <Interface>& src) noexcept :
		Base (src)
	{}

	I_ptr& operator = (const I_ptr& src) noexcept
	{
		Base::operator = (src);
		return *this;
	}

	Interface* operator & () const noexcept
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		return this->p_;
	}

	template <class I>
	I_ptr <I> downcast () noexcept
	{
		return I::_unsafe_cast (p_);
	}

};

inline I_ptr <Interface> Interface::_nil () noexcept
{
	return I_ptr <Interface> ((Interface*)nullptr);
}

}

#ifdef LEGACY_CORBA_CPP

/// CORBA::release
template <class I> inline
void release (const Internal::I_ptr <I>& ptr)
{
	Internal::interface_release (&ptr);
}

#endif

/// CORBA::is_nil()
template <class I> inline
bool is_nil (const Internal::I_ptr <I>& ptr)
{
	return !ptr;
}

/// CORBA::is_nil()
template <class I> inline
bool is_nil (const Internal::I_ref <I>& ptr)
{
	return !ptr;
}

}

#endif
