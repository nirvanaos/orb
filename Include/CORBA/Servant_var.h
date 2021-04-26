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
#ifndef NIRVANA_ORB_SERVANTVAR_H_
#define NIRVANA_ORB_SERVANTVAR_H_

#include "servant_reference.h"

#ifdef LEGACY_CORBA_CPP

namespace PortableServer {

template <typename T>
using Servant_out = CORBA::servant_out <T>;

template <typename T>
class Servant_var : public CORBA::servant_reference <T>
{
	typedef CORBA::servant_reference <T> Base;
public:
	Servant_var ()
	{}

	Servant_var (T* p) :
		Base (p, false)
	{}

	Servant_var (const Servant_var& src) :
		Base (src)
	{}

	Servant_var (Servant_var&& src) :
		Base (std::move (src))
	{}

	Servant_var& operator = (T* p)
	{
		if (Base::p_ != p)
			reset (p);
		return *this;
	}

	Servant_var& operator = (const Servant_var& src)
	{
		Base::operator = (src);
		return *this;
	}

	template <class T1>
	Servant_var& operator = (Servant_var <T1>&& src)
	{
		Base::operator = (std::move (src));
		return *this;
	}

	T* in () const
	{
		return Base::p_;
	}

	Servant_var <T>& inout ()
	{
		return *this;
	}

	Servant_out <T> out ()
	{
		return Servant_out <T> (*this);
	}

	T* _retn ()
	{
		T* retval = Base::p_;
		Base::p_ = nullptr;
		return retval;
	}

private:
	void reset (T* p) NIRVANA_NOEXCEPT
	{
		T* tmp = Base::p_;
		Base::p_ = p;
		if (tmp)
			tmp->_remove_ref ();
	}
};

}

#endif

#endif
