/// \file
/// PortableServer::POA skeleton

/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_ORB_POA_S_H_
#define NIRVANA_ORB_POA_S_H_

#include "POA.h"
#include "Object_s.h"
#include "ImplementationLocal.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, PortableServer::POA>
{
public:
	static const typename Bridge <PortableServer::POA>::EPV epv_;

protected:
	static Type <String>::ABI_ret _activate_object (Bridge <PortableServer::POA>* obj, Interface* servant, Interface* env)
	{
		try {
			return Type <String>::ret (S::_implementation (obj).activate_object (TypeI <PortableServer::ServantBase>::in (servant)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <String>::ABI_ret ();
	}

	static void _deactivate_object (Bridge <PortableServer::POA>* obj, Type <String>::ABI_in objid, Interface* env)
	{
		try {
			S::_implementation (obj).deactivate_object (Type <String>::in (objid));
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}
};

template <class S>
const Bridge <PortableServer::POA>::EPV Skeleton <S, PortableServer::POA>::epv_ = {
	{ // header
		Bridge <PortableServer::POA>::repository_id_,
		S::template __duplicate <PortableServer::POA>,
		S::template __release <PortableServer::POA>
	},
	{ // base
		S::template _wide_object <PortableServer::POA>
	},
	{ // epv
		S::_activate_object,
		S::_deactivate_object
	}
};

template <class S>
class Servant <S, PortableServer::POA> :
	public ImplementationLocal <S, PortableServer::POA>
{};

}
}

#endif
