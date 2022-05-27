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
#ifndef NIRVANA_ORB_PSEUDOBASE_H_
#define NIRVANA_ORB_PSEUDOBASE_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include "Client.h"
#include "Type_interface.h"
#include "String.h"
#include "basic_types.h"
#include "TCKind.h"

namespace CORBA {
namespace Internal {

class PseudoBase;
#ifdef LEGACY_CORBA_CPP
typedef I_ptr <PseudoBase> PseudoBase_ptr;
typedef I_var <PseudoBase> PseudoBase_var;
typedef PseudoBase_var& PseudoBase_out;
#endif

template <>
struct Type <PseudoBase> : TypeItf <PseudoBase>
{};

NIRVANA_BRIDGE_BEGIN (PseudoBase, CORBA_INTERNAL_REPOSITORY_ID ("PseudoBase"))
Interface* (*query_interface) (Bridge <PseudoBase>*, Type <String>::ABI_in, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, PseudoBase> :
	public T
{
public:
	/// This method does not increment reference counter
	I_ptr <Interface> _query_interface (String_in type_id);

	/// This method does not increment reference counter
	template <class I>
	I_ptr <I> _query_interface ()
	{
		return static_cast <I*> (&_query_interface (Bridge <I>::repository_id_));
	}
};

template <class T>
I_ptr <Interface> Client <T, PseudoBase>::_query_interface (String_in type_id)
{
	Environment _env;
	Bridge <PseudoBase>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.query_interface) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

class PseudoBase :
	public ClientInterfacePrimary <PseudoBase>
{
};

}
}

#endif
