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
#ifndef NIRVANA_ORB_SERVANTBASESTATIC_H_
#define NIRVANA_ORB_SERVANTBASESTATIC_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/OLF.h>
#include "ServantBase_s.h"
#include "servant_core.h"
#include "ServantStatic.h"
#include "LifeCycleStatic.h"

namespace CORBA {
namespace Internal {

//! Static implementation of PortableServer::ServantBase.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceStatic <S, PortableServer::ServantBase> :
	public InterfaceStaticBase <S, PortableServer::ServantBase>,
	public ServantTraitsStatic <S>,
	public LifeCycleStatic
{
public:
	static Bridge <Object>* _get_object (String_in iid)
	{
		return get_object_from_core (servant_base (), iid);
	}

	// ServantBase operations

	static I_ref <PortableServer::POA> _default_POA ()
	{
		return servant_base ()->_default_POA ();
	}

	static I_ref <InterfaceDef> _get_interface ()
	{
		return servant_base ()->_get_interface ();
	}

	static Boolean _is_a (const String& type_id)
	{
		return servant_base ()->_is_a (type_id);
	}

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

	static PortableServer::Servant _core_servant ()
	{
		return servant_base ();
	}

protected:
	static Interface* _get_proxy ()
	{
		return get_proxy (servant_base ());
	}

private:
	static PortableServer::Servant servant_base ()
	{
		return static_cast <PortableServer::ServantBase*> (export_struct_.core_object);
	}

public:
	// We can't use `static const` here, because it causes the redundant optimization in CLang.
	NIRVANA_OLF_SECTION static  NIRVANA_STATIC_IMPORT ::Nirvana::ExportObject export_struct_;
};

template <class S> NIRVANA_OLF_SECTION
NIRVANA_STATIC_IMPORT ::Nirvana::ExportObject InterfaceStatic <S, PortableServer::ServantBase>::export_struct_{ ::Nirvana::OLF_EXPORT_OBJECT, ::Nirvana::StaticId <S>::static_id_
, NIRVANA_STATIC_BRIDGE (PortableServer::ServantBase, S) };

}
}

#endif
