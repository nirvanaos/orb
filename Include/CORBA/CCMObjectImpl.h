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
#ifndef NIRVANA_ORB_CCMOBJECTIMPL_H_
#define NIRVANA_ORB_CCMOBJECTIMPL_H_
#pragma once

#include "ccm/CCM_Object.h"

namespace CORBA {
namespace Internal {

NIRVANA_NORETURN void throw_InvalidName ();

class CCM_ReceptaclesBase
{
public:
	static Type <Components::Cookie>::VRet connect (const Components::FeatureName&, Object::_ptr_type)
	{
		throw_InvalidName ();
	}

	static Type <CORBA::Object>::VRet disconnect (const Components::FeatureName&, Components::Cookie::_ptr_type)
	{
		throw_InvalidName ();
	}

};

template <class S>
class InterfaceImpl <S, Components::Receptacles> :
	public InterfaceImplBase <S, Components::Receptacles>,
	public CCM_ReceptaclesBase
{};

class CCM_NavigationBase
{
public:
	static Type <CORBA::Object>::VRet provide_facet (const Type < ::Components::FeatureName>::Var& name)
	{
		throw_InvalidName ();
	}

};

template <class S>
class InterfaceImpl <S, Components::Navigation> :
	public InterfaceImplBase <S, Components::Navigation>,
	public CCM_NavigationBase
{};

class CCM_ObjectBase
{
public:
	static Type <Components::CCMHome>::VRet get_ccm_home () noexcept
	{
		return nullptr;
	}

	static void configuration_complete () noexcept
	{}

	static void remove ()
	{
		throw Components::RemoveFailure ();
	}

};

template <class S>
class InterfaceImpl <S, Components::CCMObject> :
	public InterfaceImplBase <S, Components::CCMObject>,
	public CCM_ObjectBase
{};

template <class I> class CCM_ObjectConnections;

template <class S, class I> class CCM_ObjectFeatures;

}
}

#endif
