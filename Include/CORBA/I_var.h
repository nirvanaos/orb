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
#ifndef NIRVANA_ORB_I_VAR_H_
#define NIRVANA_ORB_I_VAR_H_

#include "I_ref.h"

#ifdef LEGACY_CORBA_CPP

namespace CORBA {
namespace Nirvana {

template <class I> class I_out;

/// Helper class to support legacy interface smart pointer.
template <class I>
class I_var : public I_ref <I>
{
	typedef I_ref <I> Base;
public:
	I_var () NIRVANA_NOEXCEPT
	{}

	// No add reference
	I_var (const I_ptr <I>& p) NIRVANA_NOEXCEPT :
		Base (p.p_)
	{}

	I_var (const I_ref <I>& src) :
		Base (src)
	{}

	I_var (const I_var <I>& src) :
		Base (src)
	{}

	I_var (I_ref <I>&& src) NIRVANA_NOEXCEPT :
		Base (std::move (src))
	{}

	I_var (I_var <I>&& src) NIRVANA_NOEXCEPT :
		Base (std::move (src))
	{}

	// No add reference
	I_var& operator = (const I_ptr <I>& p) NIRVANA_NOEXCEPT
	{
		if (p.p_ != Base::p_) {
			I* tmp = Base::p_;
			Base::p_ = p.p_;
			interface_release (tmp);
		}
		return *this;
	}

	I_var& operator = (const I_var& src)
	{
		Base::operator = (src);
		return *this;
	}

	I_var& operator = (I_var&& src) NIRVANA_NOEXCEPT
	{
		Base::operator = (std::move (src));
		return *this;
	}

	inline I_inout <I> inout ();
	inline I_out <I> out ();

	I_ptr <I> in () const
	{
		return Base::p_;
	}

	I_ptr <I> _retn () NIRVANA_NOEXCEPT
	{
		I_ptr <I> p (Base::p_);
		Base::p_ = nullptr;
		return p;
	}
};

}
}

#endif

#endif
