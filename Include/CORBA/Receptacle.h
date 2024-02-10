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
#ifndef NIRVANA_ORB_RECEPTACLE_H_
#define NIRVANA_ORB_RECEPTACLE_H_
#pragma once

#include "ccm/CCM_Receptacle.h"

namespace CORBA {
namespace Internal {

class ReceptacleSimplexBase
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
class ReceptacleSimplex : private ReceptacleSimplexBase
{
public:
	void connect (I_ptr <I> p)
	{
		ReceptacleSimplexBase::connect (p);
	}

	typename Type <I>::VRet disconnect ()
	{
#ifndef LEGACY_CORBA_CPP
		I_ref <I> ret;
		ReceptacleSimplexBase::disconnect (reinterpret_cast <I_ref <Interface>&> (ret));
		return ret;
#else
		I_var <I> ret;
		ReceptacleSimplexBase::disconnect (reinterpret_cast <I_ref <Interface>&> (ret));
		return ret._retn ();
#endif
	}

	typename Type <I>::VRet get_connection () const noexcept
	{
#ifndef LEGACY_CORBA_CPP
		I_ref <I> ret;
		ReceptacleSimplexBase::get_connection (reinterpret_cast <I_ref <Interface>&> (ret));
		return ret;
#else
		I_var <I> ret;
		ReceptacleSimplexBase::get_connection (reinterpret_cast <I_ref <Interface>&> (ret));
		return ret._retn ();
#endif
	}

	operator I_ptr <I> () const noexcept
	{
		return reinterpret_cast <I*> (&ReceptacleSimplexBase::connection ());
	}
};

class Connection
{
public:
	Connection (I_ptr <Interface> p, size_t idx);

private:
	I_ref <Interface> objref_;
	I_ref <Components::Cookie> ck_;
};

typedef IDL::Sequence <Connection> Connections;

class ReceptacleMultiplexBase
{
public:
	void get_connections (Connections& connections) const;

protected:
	void connect (I_ptr <Interface> p, I_ref <Components::Cookie>& ck);
	void disconnect (I_ptr <Components::Cookie> ck, I_ref <Interface>& cxn);

protected:
	std::vector <I_ref <Interface> > connections_;
};

template <class I>
class ReceptacleMultiplex : public ReceptacleMultiplexBase
{
public:
	Type <Components::Cookie>::VRet connect (I_ptr <I> p)
	{
#ifndef LEGACY_CORBA_CPP
		I_ref <Components::Cookie> ret;
		ReceptacleMultiplexBase::connect (p, ret);
		return ret;
#else
		I_var <Components::Cookie> ret;
		ReceptacleMultiplexBase::connect (p, ret);
		return ret._retn ();
#endif
	}

	typename Type <I>::VRet disconnect (I_ptr <Components::Cookie> ck)
	{
#ifndef LEGACY_CORBA_CPP
		I_ref <I> ret;
		ReceptacleMultiplexBase::disconnect (ck, reinterpret_cast <I_ref <Interface>&> (ret));
		return ret;
#else
		I_var <I> ret;
		ReceptacleMultiplexBase::disconnect (ck, reinterpret_cast <I_ref <Interface>&> (ret));
		return ret._retn ();
#endif
	}

	operator const std::vector <I_ref <I> >& () const noexcept
	{
		return reinterpret_cast <const std::vector <I_ref <I> >&> (connections_);
	}
};

}
}

#endif
