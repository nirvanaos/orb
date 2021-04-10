//! \interface DynamicServant
//! \brief The interface for dynamically allocated servant garbage collection.
//! \fn DynamicServant::_delete()
//! \brief Called by the garbage collector.

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
#ifndef NIRVANA_ORB_DYNAMICSERVANT_H_
#define NIRVANA_ORB_DYNAMICSERVANT_H_

#include "Client.h"
#include "Type_interface.h"

namespace CORBA {
namespace Nirvana {

class DynamicServant;
typedef I_ptr <DynamicServant> DynamicServant_ptr;
typedef I_var <DynamicServant> DynamicServant_var;
typedef I_out <DynamicServant> DynamicServant_out;

template <>
struct Type <I_var <DynamicServant> > : TypeItf <DynamicServant>
{};

BRIDGE_BEGIN (DynamicServant, CORBA_NIRVANA_REPOSITORY_ID ("DynamicServant"))
void (*_delete) (Bridge <DynamicServant>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, DynamicServant> :
	public T
{
public:
	void _delete ();
};

class DynamicServant : public ClientInterface <DynamicServant>
{};

template <class T>
void Client <T, DynamicServant>::_delete ()
{
	Environment _env;
	Bridge <DynamicServant>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._delete) (&_b, &_env);
	_env.check ();
}

}
}

#endif
