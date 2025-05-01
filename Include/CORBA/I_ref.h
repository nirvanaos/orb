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
#ifndef NIRVANA_ORB_I_REF_H_
#define NIRVANA_ORB_I_REF_H_
#pragma once

#include "I_ptr.h"
#include <utility>

namespace CORBA {
namespace Internal {

template <class Itf> class I_ref;
template <> class I_ref <Interface>;

template <class Itf>
struct TypeItfBase;

template <class Itf>
class I_ref_base
{
public:
	typedef Itf ItfType;

	I_ref_base () noexcept :
		p_ (nullptr)
	{}

	I_ref_base (std::nullptr_t) noexcept :
		p_ (nullptr)
	{}

	I_ref_base (const I_ref_base& src) :
		p_ (duplicate (src.p_))
	{}

	I_ref_base (I_ref_base&& src) noexcept :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	I_ref_base (const I_ptr_base <Itf>& p) :
		p_ (duplicate (p.p_))
	{}

	~I_ref_base () noexcept
	{
		release (p_);
	}

	I_ref_base& operator = (const I_ref_base& src)
	{
		reset (src.p_);
		return *this;
	}

	I_ref_base& operator = (I_ref_base&& src) noexcept
	{
		if (&src != this) {
			release (p_);
			p_ = src.p_;
			src.p_ = nullptr;
		}
		return *this;
	}

	I_ref_base& operator = (std::nullptr_t) noexcept
	{
		release (p_);
		p_ = nullptr;
		return *this;
	}

	Itf* operator -> () const
	{
		if (!p_)
			::Nirvana::throw_INV_OBJREF ();
		return p_;
	}

	explicit operator bool () const noexcept
	{
		return p_ != nullptr;
	}

	explicit operator bool () noexcept
	{
		return p_ != nullptr;
	}

	void swap (I_ref_base& other) noexcept
	{
		Itf* tmp = p_;
		p_ = other.p_;
		other.p_ = tmp;
	}

protected:
	I_ref_base (Itf* p) :
		p_ (p)
	{}

	void reset (Itf* p)
	{
		if (p_ != p) {
			Itf* tmp = p_;
			p_ = duplicate (p);
			release (tmp);
		}
	}

	static Itf* duplicate (Itf* p)
	{
		return reinterpret_cast <Itf*> (interface_duplicate (reinterpret_cast <Interface*> (p)));
	}

	static void release (Itf* p) noexcept
	{
		// Interface may be not completely defined, so use reinterpret_cast.
		interface_release (reinterpret_cast <Interface*> (p));
	}

protected:
	template <class I1> friend class I_ptr_base;
	template <class I1> friend class I_ptr;
	template <class I1> friend class I_ref;
	friend class I_inout <Itf>;
	friend struct TypeItfBase <Itf>;

	Itf* p_;
};

}

template <class S, class ... Args>
Internal::I_ref <typename S::PrimaryInterface> make_pseudo (Args&& ... args);

namespace Internal {

/// An interface reference smart pointer.
template <class Itf>
class I_ref : public I_ref_base <Itf>
{
	typedef I_ref_base <Itf> Base;

public:
	I_ref () noexcept
	{}

	I_ref (std::nullptr_t) noexcept :
		Base (nullptr)
	{}

	I_ref (const I_ref& src) :
		Base (src)
	{}

	template <class I1>
	I_ref (const I_ref <I1>& src) :
		Base (Base::duplicate (wide (src.p_)))
	{}

	I_ref (I_ref&& src) noexcept :
		Base (std::move (src))
	{}

	template <class I1>
	I_ref (I_ref <I1>&& src) noexcept :
		Base (Base::duplicate (wide (src.p_)))
	{
		src = nullptr;
	}

	I_ref (const I_ptr <Itf>& p) :
		Base (p)
	{}

	template <class I1>
	I_ref (const I_ptr <I1>& p) :
		Base (Base::duplicate (wide (p.p_)))
	{}

	I_ref (BridgeVal <Itf>* p) :
		I_ref (I_ptr <Itf> (p))
	{}

	template <class VB, typename T>
	I_ref (ValueBox <VB, T>* p) noexcept :
		I_ref (I_ptr <Itf> (*p))
	{}

	template <class S>
	I_ref (const servant_reference <S>& sr) noexcept :
		Base (I_ptr <Itf> (sr))
	{}

	template <class S>
	I_ref (servant_reference <S>&& sr) noexcept :
		Base (I_ptr <Itf> (sr).p_)
	{
		sr.p_ = nullptr;
	}

	I_ref& operator = (const I_ref& src)
	{
		Base::operator = (src);
		return *this;
	}

	template <class I1>
	I_ref& operator = (const I_ref <I1>& src)
	{
		Base::reset (wide (src.p_));
		return *this;
	}

	I_ref& operator = (I_ref&& src) noexcept
	{
		Base::operator = (std::move (src));
		return *this;
	}

	template <class I1>
	I_ref& operator = (I_ref <I1>&& src)
	{
		Base::reset (wide (src.p_));
		src = nullptr;
		return *this;
	}

	I_ref& operator = (const I_ptr <Itf>& p)
	{
		Base::operator = (p);
		return *this;
	}

	template <class I1>
	I_ref& operator = (const I_ptr <I1>& p)
	{
		Base::reset (wide (p.p_));
		return *this;
	}

	I_ref& operator = (std::nullptr_t) noexcept
	{
		Base::operator = (nullptr);
		return *this;
	}

	I_ref& operator = (BridgeVal <Itf>* p) noexcept
	{
		return operator = (I_ptr <Itf> (p));
	}

	template <class VB, typename T>
	I_ref& operator = (ValueBox <VB, T>* p) noexcept
	{
		return operator = (I_ptr <Itf> (p));
	}

	template <class S>
	I_ref& operator = (const servant_reference <S>& sr) noexcept
	{
		return operator = (I_ptr <Itf> (sr));
	}

	template <class S>
	I_ref& operator = (servant_reference <S>&& sr) noexcept
	{
		return operator = (I_ref (std::move (sr)));
	}

protected:
	friend class I_ref <Interface>;
	friend class I_ret <Itf>;

	template <class S, class ... Args> friend
	I_ref <typename S::PrimaryInterface> CORBA::make_pseudo (Args&& ... args);

	// No add reference
	I_ref (Itf* p) :
		Base (p)
	{}

	template <class I1>
	static Itf* wide (I1* p)
	{
		return p ? static_cast <Itf*> (&static_cast <I_ptr <Itf> > (*p)) : nullptr;
	}
};

template <>
class I_ref <Interface> : public I_ref_base <Interface>
{
	typedef I_ref_base <Interface> Base;

public:
	I_ref () noexcept
	{}

	I_ref (std::nullptr_t) noexcept :
		Base (nullptr)
	{}

	I_ref (const I_ref& src) :
		Base (src)
	{}

	template <class Itf>
	I_ref (const I_ref <Itf>& src) :
		Base (Base::duplicate (src.p_))
	{}

	I_ref (I_ref&& src) noexcept :
		Base (std::move (src))
	{}

	template <class Itf>
	I_ref (I_ref <Itf>&& src) noexcept :
		Base (src.p_)
	{
		src.p_ = nullptr;
	}

	I_ref (const I_ptr <Interface>& p) :
		Base (p)
	{}

	template <class Itf>
	I_ref (const I_ptr <Itf>& p) :
		Base (Base::duplicate (&p))
	{}

	I_ref& operator = (const I_ref& src)
	{
		Base::operator = (src);
		return *this;
	}

	template <class Itf>
	I_ref& operator = (const I_ref <Itf>& src)
	{
		reset (src.p_);
		return *this;
	}

	I_ref& operator = (I_ref&& src) noexcept
	{
		Base::operator = (std::move (src));
		return *this;
	}

	template <class Itf>
	I_ref& operator = (I_ref <Itf>&& src) noexcept
	{
		release (p_);
		p_ = src.p_;
		src.p_ = nullptr;
		return *this;
	}

	I_ref& operator = (std::nullptr_t) noexcept
	{
		Base::operator = (nullptr);
		return *this;
	}

	template <class Itf>
	I_ref <Itf> downcast () noexcept
	{
		I_ref <Itf> ret (I_ptr <Interface> (p_).template downcast <Itf> ());
		p_ = nullptr;
		return ret;
	}

protected:
	friend class I_ret <Interface>;

	// No add reference
	I_ref (Interface* p) :
		Base (p)
	{}

	template <class Itf>
	static Interface* wide (Itf* p) noexcept
	{
		return static_cast <Interface*> (p);
	}
};

template <class Itf> inline
I_ptr_base <Itf>::I_ptr_base (const I_ref <Itf>& src) noexcept :
	p_ (src.p_)
{}

template <class Itf> inline
void I_ptr_base <Itf>::move_from (I_ref <Itf>& src) noexcept
{
	p_ = src.p_;
	src.p_ = nullptr;
}

template <class Itf>
template <class I1> inline
I_ptr <Itf>::I_ptr (const I_ref <I1>& src) :
	Base (wide (src.p_))
{}

}
}

#endif
