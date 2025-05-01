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
#include "RepId.h"

namespace CORBA {

template <class>
class servant_reference;

namespace Internal {

template <class Itf> class I_ptr;
template <class Itf> class I_ref_base;
template <class Itf> class I_ref;
template <class Itf> class I_var;
template <class Itf> class I_inout;
template <class Itf> class I_ret;
template <class Itf> class ClientInterfacePrimary;
template <class VB, typename T> class ValueBox;

#ifndef NDEBUG
const uintptr_t UNINITIALIZED_PTR = 1;
#endif

template <class Itf>
class I_ptr_base
{
public:
	typedef Itf ItfType;

	/// Zero init skipped for performance
	I_ptr_base () noexcept
#ifndef NDEBUG
		: p_ ((Itf*)UNINITIALIZED_PTR)
#endif
	{}

	I_ptr_base (Itf* p) noexcept :
		p_ (p)
	{}

	I_ptr_base (std::nullptr_t) noexcept :
		p_ (nullptr)
	{}

	I_ptr_base (const I_ptr_base& src) noexcept :
		p_ (src.p_)
	{}

	I_ptr_base (const I_ref <Itf>& src) noexcept;

	I_ptr_base& operator = (const I_ptr_base& src) noexcept
	{
		p_ = src.p_;
		return *this;
	}

	Itf* operator -> () const
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
	void move_from (I_ref <Itf>& src) noexcept;

protected:
	friend class I_ref_base <Itf>;
	template <class I1> friend class I_var;
	template <class I1> friend class I_ref;
	friend class I_inout <Itf>;
	friend class I_ret <Itf>;
	template <class I1> friend class I_ptr;

#ifdef LEGACY_CORBA_CPP
	friend class I_var <Itf>;
#endif

	Itf* p_;
};

//! Interface pointer template.
template <class Itf>
class I_ptr : public I_ptr_base <Itf>
{
	typedef I_ptr_base <Itf> Base;

public:
	I_ptr () noexcept
	{}

	I_ptr (Itf* p) noexcept :
		Base (p)
	{}

	I_ptr (std::nullptr_t) noexcept :
		Base (nullptr)
	{}

	I_ptr (const I_ptr& src) noexcept :
		Base (src)
	{}

	template <class I1>
	I_ptr (const I_ptr <I1>& src) :
		Base (wide (src.p_))
	{}

	I_ptr (const I_ref <Itf>& src) noexcept :
		Base (src)
	{}

	template <class I1>
	I_ptr (const I_ref <I1>& src);

#ifdef LEGACY_CORBA_CPP

	/// Move constructor in case returned I_ref assigned to I_ptr:
	///    I_ref <Object> func ();
	///    Object_ptr obj = func ();
	NIRVANA_DEPRECATED ("Potentially unsafe conversion from reference to pointer")
	I_ptr (I_ref <Itf>&& src) noexcept
	{
		this->move_from (src);
	}

#endif

	I_ptr (BridgeVal <Itf>* p) noexcept :
		Base (static_cast <Itf*> (static_cast <Bridge <Itf>*> (p)))
	{}

	template <class VB, typename T>
	I_ptr (ValueBox <VB, T>* p) noexcept :
		I_ptr (static_cast <I_ptr <Itf> > (*p))
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

	I_ptr& operator = (std::nullptr_t) noexcept
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

	I_ptr& operator = (BridgeVal <Itf>* p) noexcept
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
		return operator = (static_cast <BridgeVal <Itf>*> (static_cast <S*> (sr)));
	}

	Interface* operator & () const noexcept
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		// Itf may be not completely defined so we use reinterpret_cast
		return reinterpret_cast <Interface*> (this->p_);
	}

private:
	// Calls EPV function to obtain pointer to the base interface.
	template <class I1>
	static Itf* wide (I1* p)
	{
		return p ? static_cast <Itf*> (&static_cast <I_ptr <Itf> > (*p)) : nullptr;
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

	I_ptr (std::nullptr_t) noexcept :
		Base (nullptr)
	{}

	I_ptr (const I_ptr& src) noexcept :
		Base (src)
	{}

	template <class Itf>
	I_ptr (const I_ptr <Itf>& src) noexcept :
		Base (&src)
	{}

	I_ptr (const I_ref <Interface>& src) noexcept :
		Base (src)
	{}

	template <class Itf>
	I_ptr (const I_ref <Itf>& src) noexcept :
		Base (I_ptr <Itf> (src))
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

	template <class Itf>
	Itf* downcast () const noexcept
	{
		assert (!p_ || RepId::compatible (p_->_epv ().interface_id, RepIdOf <Itf>::id));

		// Interface `Itf` may be not completely defined, use reinterpret_cast.
		// Using reinterpret_cast is safe here because the Interface is top base.
		return reinterpret_cast <Itf*> (p_);
	}

};

inline I_ptr <Interface> Interface::_nil () noexcept
{
	return I_ptr <Interface> ((Interface*)nullptr);
}

}

#ifdef LEGACY_CORBA_CPP

/// CORBA::release
template <class Itf> inline
void release (const Internal::I_ptr <Itf>& ptr)
{
	Internal::interface_release (&ptr);
}

#endif

/// CORBA::is_nil()
template <class Itf> inline
bool is_nil (const Internal::I_ptr <Itf>& ptr)
{
	return !ptr;
}

/// CORBA::is_nil()
template <class Itf> inline
bool is_nil (const Internal::I_ref <Itf>& ptr)
{
	return !ptr;
}

}

#endif
