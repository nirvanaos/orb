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
#ifndef NIRVANA_ORB_LOCALIMPL_H_
#define NIRVANA_ORB_LOCALIMPL_H_

#include "primitive_types.h"
#include "Servant_var.h"
#include "Type_forward.h"

namespace CORBA {
namespace Nirvana {

/// Locally implemented dynamic pseudo object (Environment etc).
template <class T>
class LocalImpl
{
public:
	typedef T* _ptr_type;
	typedef PortableServer::Servant_var <T> _var_type;
	typedef _var_type& _out_type;

	LocalImpl () :
		ref_cnt_ (1)
	{}

	LocalImpl (const LocalImpl&) :
		ref_cnt_ (1)
	{}

	LocalImpl& operator = (const LocalImpl&)
	{
		return *this; // Do nothing
	}

	void _add_ref ()
	{
		++ref_cnt_;
	}

	void _remove_ref ()
	{
		assert (ref_cnt_);
		if (!--ref_cnt_)
			delete& static_cast <T&> (*this);
	}

	ULong _refcount_value () const
	{
		return ref_cnt_;
	}

	static T* _duplicate (T* obj)
	{
		if (obj)
			obj->_add_ref ();
		return obj;
	}

	static T* _nil ()
	{
		return 0;
	}

private:
	ULong ref_cnt_;
};

template <class T>
struct TypeLocal
{
	typedef typename LocalImpl <T>::_var_type Var;
	typedef const Var& ConstRef;
	typedef ConstRef C_in;
	typedef typename PortableServer::Servant_out <T> C_out;
	typedef Var& C_inout;
};

}

/// CORBA::release
template <class T> inline
void release (Nirvana::LocalImpl <T>* ptr)
{
	if (ptr)
		ptr->_remove_ref ();
}

}

#endif
