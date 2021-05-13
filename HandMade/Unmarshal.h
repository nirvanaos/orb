/// \file Unmarshal.h
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
module Internal {

/// Unmarshals data between objects in the same platform domain.
pseudo interface Unmarshal
{
	/// Returns marshal context.
	/// Used for proxy code optimization.
	attribute readonly MarshalContext context;

	void adopt_memory (::Nirvana::ConstPointer p, ::Nirvana::Size size);
	Interface unmarshal_interface (::Nirvana::ConstPointer marshal_data, RepositoryId iid);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_UNMARSHAL_H_
#define NIRVANA_ORB_UNMARSHAL_H_

#include "MarshalContext.h"

namespace CORBA {

class Object;

namespace Internal {

class Unmarshal;
typedef I_ptr <Unmarshal> Unmarshal_ptr;
typedef I_var <Unmarshal> Unmarshal_var;

template <>
struct Type <Unmarshal> : TypeItf <Unmarshal>
{};

/// \interface Unmarshal
template <class T>
class Client <T, Unmarshal> :
	public T
{
public:
	MarshalContext context ();

	void adopt_memory (::Nirvana::ConstPointer p, ::Nirvana::Size size);
	TypeItf <Interface>::Var unmarshal_interface (Type < ::Nirvana::ConstPointer>::C_in marshal_data, Type <String>::C_in interface_id);
	template <class I>
	typename TypeItf <I>::Var unmarshal_interface (Type < ::Nirvana::ConstPointer>::C_in marshal_data)
	{
		return unmarshal_interface (marshal_data, I::repository_id_).downcast <I> ();
	}
};

NIRVANA_BRIDGE_BEGIN (Unmarshal, CORBA_NIRVANA_REPOSITORY_ID ("Unmarshal"))
ABI_enum (*_get_context) (Bridge <Unmarshal>*, Interface*);
void (*adopt_memory) (Bridge <Unmarshal>*, ::Nirvana::ConstPointer, ::Nirvana::Size, Interface*);
Interface* (*unmarshal_interface) (Bridge <Unmarshal>*, ::Nirvana::ConstPointer, const ABI <StringT <Char> >*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
MarshalContext Client <T, Unmarshal>::context ()
{
	Environment _env;
	Bridge <Marshal>& _b (T::_get_bridge (_env));
	Type <MarshalContext>::C_ret _ret = (_b._epv ().epv._get_context) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, Unmarshal>::adopt_memory (::Nirvana::ConstPointer p, ::Nirvana::Size size)
{
	Environment _env;
	Bridge <Unmarshal>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.adopt_memory) (&_b, p, size, &_env);
	_env.check ();
}

template <class T>
TypeItf <Interface>::Var Client <T, Unmarshal>::unmarshal_interface (Type < ::Nirvana::ConstPointer>::C_in marshal_data, Type <String>::C_in interface_id)
{
	Environment _env;
	Bridge < Unmarshal>& _b (T::_get_bridge (_env));
	TypeItf <Interface>::C_ret _ret = (_b._epv ().epv.unmarshal_interface) (&_b, &marshal_data, &interface_id, &_env);
	_env.check ();
	return _ret;
}

class Unmarshal : public ClientInterface <Unmarshal>
{};

}
}

#endif
