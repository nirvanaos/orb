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

namespace CORBA {
namespace Internal {

template <class I> class I_ref;
template <> class I_ref <Interface>;

template <class I>
struct TypeItfBase;

template <class I>
class I_ref_base
{
public:
	typedef I ItfType;

	I_ref_base () noexcept :
		p_ (nullptr)
	{}

	I_ref_base (nullptr_t) noexcept :
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

	I_ref_base (const I_ptr_base <I>& p) :
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

	I_ref_base& operator = (nullptr_t) noexcept
	{
		release (p_);
		p_ = nullptr;
		return *this;
	}

	I* operator -> () const
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
		I* tmp = p_;
		p_ = other.p_;
		other.p_ = tmp;
	}

protected:
	I_ref_base (I* p) :
		p_ (p)
	{}

	void reset (I* p)
	{
		if (p_ != p) {
			I* tmp = p_;
			p_ = duplicate (p);
			release (tmp);
		}
	}

	static I* duplicate (I* p)
	{
		return reinterpret_cast <I*> (interface_duplicate (reinterpret_cast <Interface*> (p)));
	}

	static void release (I* p) noexcept
	{
		// Interface may be not completely defined, so use reinterpret_cast.
		interface_release (reinterpret_cast <Interface*> (p));
	}

protected:
	template <class I1> friend class I_ptr_base;
	template <class I1> friend class I_ptr;
	template <class I1> friend class I_ref;
	friend class I_inout <I>;
	friend struct TypeItfBase <I>;

	I* p_;
};

}

template <class S, class ... Args>
Internal::I_ref <typename S::PrimaryInterface> make_pseudo (Args ... args);

namespace Internal {

/// An interface reference smart pointer.
template <class I>
class I_ref : public I_ref_base <I>
{
	typedef I_ref_base <I> Base;
public:
	I_ref () noexcept
	{}

	I_ref (nullptr_t) noexcept :
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

	I_ref (const I_ptr <I>& p) :
		Base (p)
	{}

	template <class I1>
	I_ref (const I_ptr <I1>& p) :
		Base (Base::duplicate (wide (p.p_)))
	{}

	I_ref (BridgeVal <I>* p) :
		Base (I_ptr <I> (p))
	{}

	template <class S>
	I_ref (const servant_reference <S>& sr) noexcept :
		Base (I_ptr <I> (static_cast <S*> (sr)))
	{}

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
		if (&src != this) {
			Base::reset (wide (src.p_));
			src = nullptr;
		}
		return *this;
	}

	I_ref& operator = (const I_ptr <I>& p)
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

	I_ref& operator = (nullptr_t) noexcept
	{
		Base::operator = (nullptr);
		return *this;
	}

protected:
	friend class I_ref <Interface>;
	friend class I_ret <I>;

	template <class S, class ... Args> friend
	I_ref <typename S::PrimaryInterface> CORBA::make_pseudo (Args ... args);

	I_ref (I* p) :
		Base (p)
	{}

	template <class I1>
	static I* wide (I1* p)
	{
		return p ? static_cast <I*> (&static_cast <I_ptr <I> > (*p)) : nullptr;
	}
};

template <>
class I_ref <Interface> : public I_ref_base <Interface>
{
	typedef I_ref_base <Interface> Base;
public:
	I_ref () noexcept
	{}

	I_ref (nullptr_t) noexcept :
		Base (nullptr)
	{}

	I_ref (const I_ref& src) :
		Base (src)
	{}

	template <class I>
	I_ref (const I_ref <I>& src) :
		Base (Base::duplicate (src.p_))
	{}

	I_ref (I_ref&& src) noexcept :
		Base (std::move (src))
	{}

	template <class I>
	I_ref (I_ref <I>&& src) noexcept :
		Base (src.p_)
	{
		src.p_ = nullptr;
	}

	I_ref (const I_ptr <Interface>& p) :
		Base (p)
	{}

	template <class I>
	I_ref (const I_ptr <I>& p) :
		Base (Base::duplicate (&p))
	{}

	I_ref& operator = (const I_ref& src)
	{
		Base::operator = (src);
		return *this;
	}

	template <class I>
	I_ref& operator = (const I_ref <I>& src)
	{
		reset (src.p_);
		return *this;
	}

	I_ref& operator = (I_ref&& src) noexcept
	{
		Base::operator = (std::move (src));
		return *this;
	}

	template <class I>
	I_ref& operator = (I_ref <I>&& src) noexcept
	{
		release (p_);
		p_ = src.p_;
		src.p_ = nullptr;
		return *this;
	}

	I_ref& operator = (nullptr_t) noexcept
	{
		Base::operator = (nullptr);
		return *this;
	}

	template <class I>
	I_ref <I> downcast () noexcept
	{
		I_ref <I> ret (I::_unsafe_cast (p_));
		p_ = nullptr;
		return ret;
	}

protected:
	friend class I_ret <Interface>;
	I_ref (Interface* p) :
		Base (p)
	{}
};

template <class I> inline
I_ptr_base <I>::I_ptr_base (const I_ref <I>& src) noexcept :
	p_ (src.p_)
{}

template <class I> inline
void I_ptr_base <I>::move_from (I_ref <I>& src) noexcept
{
	p_ = src.p_;
	src.p_ = nullptr;
}

template <class I>
template <class I1> inline
I_ptr <I>::I_ptr (const I_ref <I1>& src) :
	Base (wide (src.p_))
{}

}
}

#endif
