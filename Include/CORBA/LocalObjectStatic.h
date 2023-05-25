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
#pragma once

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/OLF.h>
#include "LocalObject_s.h"
#include "local_core.h"
#include "ServantStatic.h"
#include "LifeCycleStatic.h"

namespace CORBA {
namespace Internal {

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, LocalObject> :
	public InterfaceStaticBase <S, LocalObject>,
	public ServantTraitsStatic <S>,
	public LifeCycleStatic
{
public:
	static Bridge <Object>* _get_object (Type <String>::ABI_in iid, Interface* env) NIRVANA_NOEXCEPT
	{
		return get_object_from_core (core_object (), iid, env);
	}

	// Object operations

	static Boolean _non_existent () NIRVANA_NOEXCEPT
	{
		return false;
	}

	static void _add_ref () NIRVANA_NOEXCEPT
	{}

	static void _remove_ref () NIRVANA_NOEXCEPT
	{}

	static ULong _refcount_value () NIRVANA_NOEXCEPT
	{
		return 1;
	}

	static void _delete_object () NIRVANA_NOEXCEPT
	{
		Nirvana::throw_NO_IMPLEMENT ();
	}

	static Bridge <AbstractBase>* _get_abstract_base (Type <String>::ABI_in iid,
		Interface* env) NIRVANA_NOEXCEPT
	{
		return nullptr;
	}

protected:
	static Interface* _get_proxy ()
	{
		return get_proxy (core_object ());
	}

private:
	static LocalObject::_ptr_type core_object ()
	{
		return static_cast <LocalObject*> (export_struct_.core_object);
	}

public:
	// We can't use `static const` here, because it causes the redundant optimization in CLang.
	static NIRVANA_OLF_SECTION  NIRVANA_STATIC_IMPORT ::Nirvana::ExportLocal export_struct_;
};

template <class S> NIRVANA_OLF_SECTION
NIRVANA_STATIC_IMPORT ::Nirvana::ExportLocal InterfaceStatic <S, LocalObject>::export_struct_{ ::Nirvana::OLF_EXPORT_LOCAL, ::Nirvana::StaticId <S>::static_id_
, NIRVANA_STATIC_BRIDGE (LocalObject, S) };

}
}

#endif
