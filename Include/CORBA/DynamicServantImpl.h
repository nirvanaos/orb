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
#ifndef NIRVANA_ORB_DYNAMICSERVANTIMPL_H_
#define NIRVANA_ORB_DYNAMICSERVANTIMPL_H_
#pragma once

#include "ServantImpl.h"
#include "ImplementationPseudo.h"
#include "ImplementationPseudoStatic.h"
#include "TraitsServantPseudo.h"
#include "DynamicServant_s.h"
#include "LifeCycleNone.h"

namespace CORBA {
namespace Internal {

class DynamicServantBridge
{
public:
	DynamicServant::_ptr_type _dynamic_servant () noexcept
	{
		return &bridge_;
	}

protected:
	DynamicServantBridge (const Bridge <DynamicServant>::EPV& epv) noexcept :
		bridge_ (epv)
	{}

private:
	class BridgeImpl : public BridgeVal <DynamicServant>
	{
	public:
		BridgeImpl (const Bridge <DynamicServant>::EPV& epv) :
			BridgeVal <DynamicServant> (epv)
		{}
	};

	BridgeImpl bridge_;
};


template <class S>
class DynamicServantSkel :
	private LifeCycleNone
{
private:
	static void _s_delete_object (Bridge <DynamicServant>* _b, Interface* _env)
	{
		try {
			check_pointer (_b, epv_.header);
			static_cast <S&> (reinterpret_cast <DynamicServantBridge&> (*_b))._delete_object ();
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

protected:
	static const Bridge <DynamicServant>::EPV epv_;
};

template <class S>
const Bridge <DynamicServant>::EPV DynamicServantSkel <S>::epv_ = {
	{ // header
		RepIdOf <DynamicServant>::id,
		LifeCycleNone::__dup,
		LifeCycleNone::__rel
	},
	{ // EPV
		_s_delete_object
	}
};

template <class S>
class InterfaceImpl <S, DynamicServant> :
	public DynamicServantBridge,
	public DynamicServantSkel <S>
{
protected:
	InterfaceImpl () :
		DynamicServantBridge (DynamicServantSkel <S>::epv_)
	{}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
	{}
};

}
}

#endif
