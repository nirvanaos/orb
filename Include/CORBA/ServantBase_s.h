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
#ifndef NIRVANA_ORB_SERVANTBASE_S_H_
#define NIRVANA_ORB_SERVANTBASE_S_H_

#include "ServantBase.h"
#include "Type_interface.h"

namespace CORBA {
namespace Nirvana {

// ServantBase skeleton

template <class S>
class Skeleton <S, ::PortableServer::ServantBase>
{
public:
	static const typename Bridge <::PortableServer::ServantBase>::EPV epv_;

protected:
	static Interface* __default_POA (Bridge <::PortableServer::ServantBase>* obj, Interface* env)
	{
		try {
			return Type <::PortableServer::POA>::ret (S::_implementation (obj)._default_POA ());
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Interface* __get_interface (Bridge <::PortableServer::ServantBase>* obj, Interface* env)
	{
		try {
			return Type <InterfaceDef>::ret (S::_implementation (obj)._get_interface ());
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Type <Boolean>::ABI_ret __is_a (Bridge <::PortableServer::ServantBase>* obj, Type <String>::ABI_in type_id, Interface* env)
	{
		try {
			return S::_implementation (obj)._is_a (Type <String>::in (type_id));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Type <Boolean>::ABI_ret __non_existent (Bridge <::PortableServer::ServantBase>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Interface* ___core_servant (Bridge <::PortableServer::ServantBase>* obj, Interface* env)
	{
		try {
			return Type <::PortableServer::ServantBase>::VT_ret (S::_implementation (obj).__core_servant ());
		} catch (Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <::PortableServer::ServantBase>::EPV Skeleton <S, ::PortableServer::ServantBase>::epv_ = {
	{ // header
		Bridge <::PortableServer::ServantBase>::repository_id_,
		S::template __duplicate <::PortableServer::ServantBase>,
		S::template __release <::PortableServer::ServantBase>
	},
	{ // base
		S::template _wide <AbstractBase, ::PortableServer::ServantBase>,
		S::template _wide <ReferenceCounter, ::PortableServer::ServantBase>
	},
	{ // epv
		S::__default_POA,
		S::__get_interface,
		S::__is_a,
		S::__non_existent,
		S::___core_servant
	}
};

}
}

#endif
