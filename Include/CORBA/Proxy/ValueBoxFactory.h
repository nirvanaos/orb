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
#ifndef NIRVANA_ORB_VALUEBOXFACTORY_H_
#define NIRVANA_ORB_VALUEBOXFACTORY_H_
#pragma once

#include "../ServantStatic.h"
#include "../ValueFactoryBase_s.h"
#include "TypeCodeValueBox.h"

namespace CORBA {
namespace Internal {

template <class VB>
class ValueBoxFactory :
	public TypeCodeValueBox <VB>,
	public InterfaceStaticBase <ValueBoxFactory <VB>, PseudoBase>,
	public InterfaceStaticBase <ValueBoxFactory <VB>, ValueFactoryBase>,
	public ServantTraitsStatic <ValueBoxFactory <VB> >
{
public:
	using ServantTraitsStatic <ValueBoxFactory <VB> >::_implementation;

	// PseudoBase
	static I_ptr <Interface> _query_interface (String_in id)
	{
		return FindInterface <ValueFactoryBase, TypeCode>::find (*(ValueBoxFactory <VB>*)nullptr, id);
	}

	static I_ptr <Interface> _query_factory (String_in id) noexcept
	{
		return nullptr;
	}

	static Type <ValueBase>::VRet create_for_unmarshal ()
	{
		return create_value <VB> ();
	}

};

}
}

#endif
