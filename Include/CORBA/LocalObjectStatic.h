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
#ifndef NIRVANA_ORB_LOCALOBJECTSTATIC_H_
#define NIRVANA_ORB_LOCALOBJECTSTATIC_H_

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/OLF.h>
#include "AbstractBaseStatic.h"
#include "ReferenceCounterStatic.h"
#include "LocalObject_s.h"
#include "local_core.h"

namespace CORBA {
namespace Nirvana {

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, LocalObject> :
	public InterfaceStatic <S, AbstractBase>,
	public InterfaceStatic <S, ReferenceCounter>,
	public InterfaceStaticBase <S, LocalObject>
{
public:
	static Bridge <Object>* _get_object (String_in iid)
	{
		return get_object_from_core (core_object (), iid);
	}

	// Object operations

	static Boolean _non_existent ()
	{
		return false;
	}

protected:
	static Interface* _get_proxy ()
	{
		return get_proxy (core_object ());
	}

private:
	static LocalObject_ptr core_object ()
	{
		return static_cast <LocalObject*> (export_struct_.core_object);
	}

public:
	// We can't use `static const` here, because it causes the redundant optimization in CLang.
	static NIRVANA_OLF_SECTION volatile ::Nirvana::ExportLocal export_struct_;
};

template <class S> NIRVANA_OLF_SECTION
volatile ::Nirvana::ExportLocal InterfaceStatic <S, LocalObject>::export_struct_{ ::Nirvana::OLF_EXPORT_LOCAL, ::Nirvana::StaticId <S>::static_id_
, STATIC_BRIDGE (LocalObject, S), STATIC_BRIDGE (AbstractBase, S) };

}
}

#endif
