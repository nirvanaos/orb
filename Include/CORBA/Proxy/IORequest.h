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
pseudo interface IORequest
{
	/// Returns a marshaler associated with this call.
	/// If the marshaler does not exist, it will be created.
	/// Caller mustn't duplicate this interface.
	readonly attribute Marshal marshaler;

	/// Return exception to caller.
	void exception (inout any e);

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

#include "Marshal.h"

namespace CORBA {
namespace Nirvana {

class IORequest;
typedef I_ptr <IORequest> IORequest_ptr;
typedef I_var <IORequest> IORequest_var;


NIRVANA_BRIDGE_BEGIN (IORequest, CORBA_NIRVANA_REPOSITORY_ID ("IORequest"))
Interface* (*marshaler) (Bridge <IORequest>*, Interface*);
void (*exception) (Bridge <IORequest>*, Type <Any>::ABI_out);
void (*success) (Bridge <IORequest>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, IORequest> :
	public T
{
public:
	Marshal_var marshaler ();
	void exception (Exception&& e) NIRVANA_NOEXCEPT;
	void success ();
};

class IORequest : public ClientInterface <IORequest>
{};

template <class T>
Marshal_var Client <T, IORequest>::marshaler ()
{
	Environment _env;
	Bridge <IORequest>& _b (T::_get_bridge (_env));
	Type <Marshal>::C_ret _ret = (_b._epv ().epv.marshaler) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::exception (Exception&& e) NIRVANA_NOEXCEPT
{
	Any any;
	any <<= (std::move (e));
	Environment* _env = nullptr;
	Bridge <IORequest>& _b (T::_get_bridge (*_env));
	(_b._epv ().epv.exception) (&_b, &Type <Any>::C_inout (any));
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
