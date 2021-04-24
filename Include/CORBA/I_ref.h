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

#include "I_ptr.h"

namespace CORBA {
namespace Nirvana {

/// An interface reference smart pointer.
template <class I>
class I_ref
{
public:
	I_ref () NIRVANA_NOEXCEPT :
		p_ (nullptr)
	{}

	I_ref (nullptr_t) NIRVANA_NOEXCEPT :
		p_ (nullptr)
	{}

	I_ref (const I_ref& src) :
		p_ (duplicate (src.p_))
	{}

	template <class I1>
	I_ref (const I_ref <I1>& src) :
		p_ (duplicate (wide (src.p_)))
	{}

	I_ref (I_ref <I>&& src) NIRVANA_NOEXCEPT :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	template <class I1>
	I_ref (I_ref <I1>&& src) NIRVANA_NOEXCEPT :
		p_ (duplicate (wide (src.p_)))
	{
		src = nullptr;
	}

	I_ref (const I_ptr <I>& p) :
		p_ (duplicate (p.p_))
	{}

	template <class I1>
	I_ref (const I_ptr <I1>& p) :
		p_ (duplicate (wide (p.p_)))
	{}

	~I_ref () NIRVANA_NOEXCEPT
	{
		interface_release (p_);
	}

	I_ref& operator = (const I_ref& src)
	{
		reset (src.p_);
		return *this;
	}

	template <class I1>
	I_ref& operator = (const I_ref <I1>& src)
	{
		reset (wide (src.p_));
		return *this;
	}

	I_ref& operator = (I_ref&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			interface_release (p_);
			p_ = src.p_;
			src.p_ = nullptr;
		}
		return *this;
	}

	template <class I1>
	I_ref& operator = (I_ref <I1>&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			reset (wide (src.p_));
			src = nullptr;
		}
		return *this;
	}

	I_ref& operator = (const I_ptr <I>& p)
	{
		reset (p.p_);
		return *this;
	}

	template <class I1>
	I_ref& operator = (const I_ptr <I1>& p)
	{
		reset (wide (p.p_));
		return *this;
	}

	I_ref& operator = (nullptr_t) NIRVANA_NOEXCEPT
	{
		interface_release (p_);
		p_ = nullptr;
	}

	I* operator -> () const
	{
		if (!p_)
			::Nirvana::throw_INV_OBJREF ();
		return p_;
	}

	explicit operator bool () const NIRVANA_NOEXCEPT
	{
		return p_ != nullptr;
	}

	explicit operator bool () NIRVANA_NOEXCEPT
	{
		return p_ != nullptr;
	}

protected:
	I_ref (const I_ptr <I>& p, bool) :
		p_ (p.p_)
	{}

private:
	void reset (I* p) NIRVANA_NOEXCEPT
	{
		if (p_ != p) {
			I* tmp = p_;
			p_ = duplicate (p);
			interface_release (tmp);
		}
	}

	static I* duplicate (I* p)
	{
		return static_cast <I*> (interface_duplicate (p));
	}

	template <class I1>
	static I* wide (I1* p)
	{
		return p ? static_cast <I*> (&static_cast <I_ptr <I> > (*p)) : nullptr;
	}

protected:
	template <class I1>
	friend class I_ptr_base;
	friend class I_inout <I>;

	I* p_;
};

template <class I> inline
I_ptr_base <I>::I_ptr_base (const I_ref <I>& src) NIRVANA_NOEXCEPT :
	p_ (src.p_)
{}

template <class I> inline
void I_ptr_base <I>::move_from (I_ref <I>& src) NIRVANA_NOEXCEPT
{
	p_ = src.p_;
	src.p_ = nullptr;
}

template <class I>
template <class I1> inline
I_ptr_base <I>::I_ptr_base (const I_ref <I1>& src) :
	p_ (wide (src.p_))
{}

}
}

#endif

