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
#ifndef NIRVANA_ORB_TYPECODENATIVE_H_
#define NIRVANA_ORB_TYPECODENATIVE_H_

#include "../TypeCodeImpl.h"

namespace CORBA {
namespace Internal {

template <class T>
class TypeCodeNative :
	public TypeCodeStatic <TypeCodeNative <T>, TypeCodeWithId <TCKind::tk_native, T>, TypeCodeOps <void> >,
	public TypeCodeName <T>
{
	typedef TypeCodeStatic <TypeCodeNative <T>, TypeCodeWithId <TCKind::tk_native, T>, TypeCodeOps <void> > Base;
public:
	using TypeCodeName <T>::_s_name;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equal (TCKind::tk_native, Base::RepositoryType::id, TypeCodeName <T>::name_, other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return Base::_bridge () == &other ||
			Base::equivalent (other);
	}

	static I_ref <TypeCode> get_compact_typecode ()
	{
		return g_ORB->create_native_tc (Base::RepositoryType::id, nullptr);
	}
};

}
}

#endif
