/// \file AbstractBaseImpl.h
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
#ifndef NIRVANA_ORB_ABSTRACTBASEIMPL_H_
#define NIRVANA_ORB_ABSTRACTBASEIMPL_H_
#pragma once

#include "AbstractBase_s.h"

namespace CORBA {
namespace Internal {

/// AbstractBase dynamic implementation.
/// Used in value types only.
template <class S>
class InterfaceImpl <S, AbstractBase> :
	public InterfaceImplBase <S, AbstractBase>
{
public:
	static Type <Object>::VRet _to_object () noexcept
	{
		return nullptr;
	}
/*
	Type <ValueBase>::VRet _to_value () noexcept
	{
		return I_ptr <ValueBase> (&static_cast <ValueBase&> (static_cast <Bridge <ValueBase>&> (static_cast <S&> (*this))));
	}
	*/
	Bridge <AbstractBase>* _get_abstract_base (Type <String>::ABI_in iid,
		Interface* env) noexcept
	{
		if (!RepId::compatible (RepIdOf <AbstractBase>::id, Type <String>::in (iid)))
			set_INV_OBJREF (env);
		return this;
	}

};

}
}

#endif
