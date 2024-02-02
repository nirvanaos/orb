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
#ifndef NIRVANA_ORB_VALUESTATIC_H_
#define NIRVANA_ORB_VALUESTATIC_H_
#pragma once

#include "ValueBaseStatic.h"
#include "AbstractBaseStatic.h"
#include "FindInterface.h"
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Internal {

template <class S, class Primary, class ... Bases>
class ValueStatic :
	public InterfaceStatic <S, ValueBase>,
	public ServantTraitsStatic <S>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>
{
public:
	typedef Primary PrimaryInterface;

	static I_ptr <Interface> _query_valuetype (const String& id)
	{
		return FindInterface <Primary, Bases...>::find (*(S*)0, id);
	}

};

template <class S, class Primary, class ... Bases>
class ValueStaticAbstractBase :
	public ValueStatic <S, Primary, Bases...>,
	public InterfaceStatic <S, AbstractBase>
{};

template <class S, class Primary, class Itf, class ... Bases>
class ValueStaticSupports :
	public InterfaceStatic <S, ValueBase>,
	public ServantStatic <S, Itf>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>
{
public:
	typedef Primary PrimaryInterface;

	static I_ptr <Interface> _query_valuetype (const String& id)
	{
		return FindInterface <Primary, Bases...>::find (*(S*)0, id);
	}
};

}
}

#if !defined (NIRVANA_PROCESS) && !defined (NIRVANA_SINGLETON)

#define NIRVANA_EXPORT_VALUE(uname, Impl) NIRVANA_EXPORT (uname, CORBA::Internal::StaticId <Impl>::id, CORBA::ValueBase, Impl)

#endif

#endif
