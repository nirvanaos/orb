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

#include "../ServantStatic.h"
#include "../ValueFactoryBase_s.h"
#include "TypeCodeValue.h"
#include "TypeCodeValueBox.h"

namespace CORBA {
namespace Internal {

template <class I>
class ValueFactoryImpl :
	public TypeCodeValue <I>,
	public InterfaceStaticBase <ValueFactoryImpl <I>, PseudoBase>,
	public ServantTraitsStatic <ValueFactoryImpl <I> >
{
public:
	using ServantTraitsStatic <ValueFactoryImpl <I> >::_implementation;

	// PseudoBase
	static I_ptr <Interface> _query_interface (String_in id)
	{
		I_ptr <ValueFactoryBase> fb = get_factory <I> ();
		if (RepId::compatible (RepIdOf <ValueFactoryBase>::id, id))
			return fb;
		I_ptr <Interface> itf = fb->_query_factory (id);
		if (!itf && RepId::compatible (RepIdOf <TypeCode>::id, id))
			itf = InterfaceStaticBase <TypeCodeValue <I>, TypeCode>::_bridge ();
		return itf;
	}

};

template <class I>
class ValueBoxFactory :
	public TypeCodeValueBox <I>,
	public InterfaceStaticBase <ValueBoxFactory <I>, PseudoBase>,
	public InterfaceStaticBase <ValueBoxFactory <I>, ValueFactoryBase>,
	public ServantTraitsStatic <ValueBoxFactory <I> >
{
public:
	using ServantTraitsStatic <ValueBoxFactory <I> >::_implementation;

	// PseudoBase
	static I_ptr <Interface> _query_interface (String_in id)
	{
		return FindInterface <ValueFactoryBase, TypeCode>::find (*(ValueBoxFactory <I>*)nullptr, id);
	}

	static I_ptr <Interface> _query_factory (String_in id) noexcept
	{
		return nullptr;
	}

#ifdef LEGACY_CORBA_CPP

	static ValueBase_ptr create_for_unmarshal ()
	{
		return new I;
	}

#else

	static ValueBase::_ref_type create_for_unmarshal ()
	{
		return make_reference <I> ();
	}

#endif

};

}
}

#endif
