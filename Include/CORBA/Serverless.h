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
#ifndef NIRVANA_ORB_SERVERLESS_H_
#define NIRVANA_ORB_SERVERLESS_H_
#pragma once

#include "primitive_types.h"
#include "servant_reference.h"
#include "Servant_var.h"
#include "Type_forward.h"

namespace CORBA {
namespace Internal {

/// Serverless dynamic pseudo object (Environment etc).
template <class T>
class Serverless
{
public:
	typedef T* _ptr_type;
#ifdef LEGACY_CORBA_CPP
	typedef PortableServer::Servant_var <T> _var_type;
	typedef _var_type& _out_type;
#else
	typedef servant_reference <T> _ref_type;
	typedef _ref_type& _out_type;
#endif

	Serverless () :
		ref_cnt_ (1)
	{}

	Serverless (const Serverless&) :
		ref_cnt_ (1)
	{}

	Serverless& operator = (const Serverless&)
	{
		return *this; // Do nothing
	}

	void _add_ref () NIRVANA_NOEXCEPT
	{
		++ref_cnt_;
	}

	void _remove_ref () NIRVANA_NOEXCEPT
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
struct TypeServerless
{
#ifdef LEGACY_CORBA_CPP
	typedef typename Serverless <T>::_var_type Var;
#else
	typedef typename Serverless <T>::_ref_type Var;
#endif
	typedef const Var& ConstRef;
	typedef ConstRef C_in;
	typedef Var C_var;
	typedef servant_out <T> C_out;
	typedef Var& C_inout;
};

}

#ifdef LEGACY_CORBA_CPP

/// CORBA::release
template <class T> inline
void release (Internal::Serverless <T>* ptr)
{
	if (ptr)
		ptr->_remove_ref ();
}

#endif

}

#endif
