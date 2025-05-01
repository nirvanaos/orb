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
#ifndef NIRVANA_ORB_VALUEFACTORYIMPL_H_
#define NIRVANA_ORB_VALUEFACTORYIMPL_H_
#pragma once

#include "ServantStatic.h"
#include "ValueFactoryBase_s.h"
#include "TypeCodeValue.h"

namespace CORBA {
namespace Internal {

template <class Itf>
class ValueFactoryImpl :
	public TypeCodeValue <Itf>,
	public InterfaceStaticBase <ValueFactoryImpl <Itf>, PseudoBase>,
	public ServantTraitsStatic <ValueFactoryImpl <Itf> >
{
public:
	using ServantTraitsStatic <ValueFactoryImpl <Itf> >::_implementation;

	// PseudoBase
	static I_ptr <Interface> _query_interface (String_in id)
	{
		I_ptr <ValueFactoryBase> fb = get_factory <Itf> ();
		if (RepId::compatible (RepIdOf <ValueFactoryBase>::id, id))
			return fb;
		I_ptr <Interface> itf = fb->_query_factory (id);
		if (!itf && RepId::compatible (RepIdOf <TypeCode>::id, id))
			itf = InterfaceStaticBase <TypeCodeValue <Itf>, TypeCode>::_bridge ();
		return itf;
	}

};

}
}

#endif
