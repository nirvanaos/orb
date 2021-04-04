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
#ifndef NIRVANA_ORB_REFERENCECOUNTER_H_
#define NIRVANA_ORB_REFERENCECOUNTER_H_

#include "Client.h"
#include "Type_interface.h"

namespace CORBA {
namespace Nirvana {

class ReferenceCounter;
typedef I_ptr <ReferenceCounter> ReferenceCounter_ptr;
typedef I_var <ReferenceCounter> ReferenceCounter_var;
typedef I_out <ReferenceCounter> ReferenceCounter_out;

BRIDGE_BEGIN (ReferenceCounter, CORBA_NIRVANA_REPOSITORY_ID ("ReferenceCounter"))
void (*add_ref) (Bridge <ReferenceCounter>*, Interface*);
void (*remove_ref) (Bridge <ReferenceCounter>*, Interface*);
ULong (*refcount_value) (Bridge <ReferenceCounter>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, ReferenceCounter> :
	public T
{
public:
	void _add_ref ();
	void _remove_ref ();
	ULong _refcount_value ();
};

class ReferenceCounter : public ClientInterface <ReferenceCounter>
{};

template <class T>
void Client <T, ReferenceCounter>::_add_ref ()
{
	Environment _env;
	Bridge <ReferenceCounter>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.add_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
void Client <T, ReferenceCounter>::_remove_ref ()
{
	Environment _env;
	Bridge <ReferenceCounter>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.remove_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
ULong Client <T, ReferenceCounter>::_refcount_value ()
{
	Environment _env;
	Bridge <ReferenceCounter>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.refcount_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

#endif
