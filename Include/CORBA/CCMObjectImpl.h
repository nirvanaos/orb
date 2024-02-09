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

#include "Components.h"

namespace CORBA {
namespace Internal {

class ConnectionBase
{
public:
	I_ptr <Components::Cookie> ck () const noexcept
	{
		return ck_;
	}

private:
	I_ref <Interface> objref_;
	I_ref <Components::Cookie> ck_;
};

class ConnectionsBase
{
private:
	IDL::Sequence <ConnectionBase> connections_;
};

template <class I>
class Connection : public ConnectionBase
{
public:
	I_ptr <I> objref () const noexcept;
};

template <class I>
class Connections
{
public:
private:
	IDL::Sequence <Connection <I> > connections_;
};

class ReceptaclesBase
{
protected:
	Components::Cookie::_ptr_type connect (I_ptr <Interface> p);
	ConnectionsBase get_connections () const;

protected:
	std::vector <I_ref <Interface> > connections_;
};

template <class I>
class Receptacles : private ReceptaclesBase
{
public:
	Type <Components::Cookie>::VRet connect (I_ptr <I> p);
	typename Type <I>::VRet disconnect (I_ptr <Components::Cookie> ck);
	Connections <I> get_connections () const;
};

class ReceptacleBase
{
private:
	I_ref <Interface> connection_;
};

template <class I>
class Receptacle : public ReceptacleBase
{};

class CCMObjectImplBase
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

	Type <CORBA::Object>::VRet provide_facet (const Components::FeatureName&)
	{
		throw Components::InvalidName ();
	}

	Type <Components::Cookie>::VRet connect (const Components::FeatureName&, Object::_ptr_type)
	{
		throw Components::InvalidName ();
	}

	Type <CORBA::Object>::VRet disconnect (const Components::FeatureName&, Components::Cookie::_ptr_type)
	{
		throw Components::InvalidName ();
	}
};

template <class S, class I> class CCMObjectImpl;

}
}

#endif
