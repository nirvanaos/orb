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

#include "Components_s.h"

namespace CORBA {
namespace Internal {

class ReceptacleBase
{
protected:
	void connect (I_ptr <Interface> p);
	void disconnect (I_ref <Interface>& conn);
	void get_connection (I_ref <Interface>& conn) const noexcept;

	I_ptr <Interface> connection () const noexcept
	{
		return connection_;
	}

private:
	I_ref <Interface> connection_;
};

template <class I>
class Receptacle : private ReceptacleBase
{
public:
	void connect (I_ptr <I> p)
	{
		ReceptacleBase::connect (p);
	}

	typename Type <I>::VRet disconnect ()
	{
#ifndef LEGACY_CORBA_CPP
		I_ref <I> ret;
		ReceptacleBase::disconnect (reinterpret_cast <I_ref <Interface>&> (ret));
		return ret;
#else
		I_var <I> ret;
		ReceptacleBase::disconnect (reinterpret_cast <I_ref <Interface>&> (ret));
		return ret._retn ();
#endif
	}

	typename Type <I>::VRet get_connection () const noexcept
	{
#ifndef LEGACY_CORBA_CPP
		I_ref <I> ret;
		ReceptacleBase::get_connection (reinterpret_cast <I_ref <Interface>&> (ret));
		return ret;
#else
		I_var <I> ret;
		ReceptacleBase::get_connection (reinterpret_cast <I_ref <Interface>&> (ret));
		return ret._retn ();
#endif
	}

	operator I_ptr <I> () const noexcept
	{
		return reinterpret_cast <I*> (&ReceptacleBase::connection ());
	}
};

class ConnectionBase
{
public:
	ConnectionBase (I_ptr <Interface> p, size_t idx);

private:
	I_ref <Interface> objref_;
	I_ref <Components::Cookie> ck_;
};

typedef IDL::Sequence <ConnectionBase> ConnectionsBase;

class ReceptaclesBase
{
public:
	void get_connections (ConnectionsBase& connections) const;

protected:
	void connect (I_ptr <Interface> p, I_ref <Components::Cookie>& ck);
	void disconnect (I_ptr <Components::Cookie> ck, I_ref <Interface>& cxn);

protected:
	std::vector <I_ref <Interface> > connections_;
};

template <class I>
class Receptacles : public ReceptaclesBase
{
public:
	Type <Components::Cookie>::VRet connect (I_ptr <I> p)
	{
#ifndef LEGACY_CORBA_CPP
		I_ref <Components::Cookie> ret;
		ReceptaclesBase::connect (p, ret);
		return ret;
#else
		I_var <Components::Cookie> ret;
		ReceptaclesBase::connect (p, ret);
		return ret._retn ();
#endif
	}

	typename Type <I>::VRet disconnect (I_ptr <Components::Cookie> ck)
	{
#ifndef LEGACY_CORBA_CPP
		I_ref <I> ret;
		ReceptaclesBase::disconnect (ck, reinterpret_cast <I_ref <Interface>&> (ret));
		return ret;
#else
		I_var <I> ret;
		ReceptaclesBase::disconnect (ck, reinterpret_cast <I_ref <Interface>&> (ret));
		return ret._retn ();
#endif
	}

	operator const std::vector <I_ref <I> >& () const noexcept
	{
		return reinterpret_cast <const std::vector <I_ref <I> >&> (connections_);
	}
};

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

	static Type <CORBA::Object>::VRet provide_facet (const Components::FeatureName&)
	{
		throw Components::InvalidName ();
	}

	static Type <Components::Cookie>::VRet connect (const Components::FeatureName&, Object::_ptr_type)
	{
		throw Components::InvalidName ();
	}

	static Type <CORBA::Object>::VRet disconnect (const Components::FeatureName&, Components::Cookie::_ptr_type)
	{
		throw Components::InvalidName ();
	}
};

template <class S, class I> class CCMObjectImpl;

template <class I>
class NIRVANA_NOVTABLE CCMObjectImplPOA : 
	public virtual ServantPOA <Components::CCMObject>,
	public CCMObjectImpl <CCMObjectImplPOA <I>, I>
{
public:
	virtual Type < ::Components::CCMHome>::VRet get_ccm_home () = 0;
	virtual void configuration_complete () = 0;
	virtual void remove () = 0;
	virtual Type <CORBA::Object>::VRet provide_facet (const Type < ::Components::FeatureName>::Var& name) = 0;
	virtual Type < ::Components::Cookie>::VRet connect (const Type < ::Components::FeatureName>::Var& name, Type <CORBA::Object>::ConstRef connection) = 0;
	virtual Type <CORBA::Object>::VRet disconnect (const Type < ::Components::FeatureName>::Var& name, Type < ::Components::Cookie>::ConstRef ck) = 0;
};

}
}

#endif
