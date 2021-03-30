/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_ORB_I_VAR_H_
#define NIRVANA_ORB_I_VAR_H_

#include "I_ptr.h"

namespace CORBA {
namespace Nirvana {

template <class I> class I_out;

//! I_var helper class for interface
template <class I>
class I_var
{
public:
	I_var () NIRVANA_NOEXCEPT :
		p_ (nullptr)
	{}

	I_var (const I_ptr <I>& p) NIRVANA_NOEXCEPT :
		p_ (p.p_)
	{}

	I_var (const I_var <I>& src) :
		p_ (static_cast <I*> (interface_duplicate (src.p_)))
	{}

	I_var (I_var <I>&& src) NIRVANA_NOEXCEPT :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	~I_var () NIRVANA_NOEXCEPT
	{
		interface_release (p_);
	}

	I_var& operator = (const I_ptr <I>& p) NIRVANA_NOEXCEPT
	{
		reset (p.p_);
		return *this;
	}

	I_var& operator = (const I_var& src)
	{
		if (src.p_ != this->p_)
			operator = (I_ptr <I> (static_cast <I*> (interface_duplicate (src.p_))));
		return *this;
	}

	I_var& operator = (I_var&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			reset (src.p_);
			src.p_ = nullptr;
		}
		return *this;
	}

	inline I_inout <I> inout ();
	inline I_out <I> out ();

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

	I_ptr <I> in () const
	{
		return p_;
	}

	I_ptr <I> _retn () NIRVANA_NOEXCEPT
	{
		I_ptr <I> p (p_);
		p_ = nullptr;
		return p;
	}

protected:
	void reset (I* p) NIRVANA_NOEXCEPT
	{
		I* tmp = p_;
		p_ = p;
		interface_release (tmp);
	}

protected:
	friend class I_ptr_base <I>;
	friend class I_inout <I>;

	I* p_;
};

template <class I> inline
I_ptr_base <I>::I_ptr_base (const I_var <I>& var) NIRVANA_NOEXCEPT
{
	p_ = var.p_;
}

template <class I> inline
void I_ptr_base <I>::move_from (I_var <I>& var) NIRVANA_NOEXCEPT
{
	p_ = var.p_;
	var.p_ = 0;
}

}
}

#endif
