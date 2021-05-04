/// \file IORequest.h
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

/*
~~~{.idl}
module CORBA {
module Nirvana {

/// \brief Interoperable Object Request.
///        Interface to the implementation of the some Inter-ORB protocol.
abstract valuetype IORequest
{
	/// Returns a marshaler associated with this call.
	/// If the marshaler does not exist, it will be created.
	/// Caller mustn't duplicate this interface.
	readonly attribute Marshal marshaler;

	/// Return exception to caller.
	void set_exception (inout any e);

	/// Marks request as successful.
	/// If request procedure return without the explicit call of success (), request will
	/// return UNKNOWN exception to caller.
	void success ();
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_IOREQUEST_H_
#define NIRVANA_ORB_IOREQUEST_H_

namespace CORBA {
namespace Nirvana {

class Marshal;

class IORequest;


NIRVANA_BRIDGE_BEGIN (IORequest, CORBA_NIRVANA_REPOSITORY_ID ("IORequest"))
Interface* (*marshaler) (Bridge <IORequest>*, Interface*);
void (*set_exception) (Bridge <IORequest>*, Type <Any>::ABI_out, Interface*);
void (*success) (Bridge <IORequest>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, IORequest> :
	public T
{
public:
	TypeItf <Marshal>::ConstRef marshaler ();
	void set_exception (Type <Any>::C_inout e);
	void success ();
};

class IORequest : public ClientInterface <IORequest>
{};

template <class T>
TypeItf <Marshal>::ConstRef Client <T, IORequest>::marshaler ()
{
	Environment _env;
	Bridge <IORequest>& _b (T::_get_bridge (_env));
	Type <Marshal>::C_VT_ret _ret = (_b._epv ().epv.marshaler) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::set_exception (Type <Any>::C_inout e)
{
	Environment _env;
	Bridge <IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.set_exception) (&_b, &Type <Any>::C_inout (e), &_env);
}

template <class T>
void Client <T, IORequest>::success ()
{
	Environment _env;
	Bridge <IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.success) (&_b, &_env);
	_env.check ();
}

}
}

#endif
