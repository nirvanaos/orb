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
#include "../../pch/pch.h"
#include <CORBA/Receptacle.h>
#include <CORBA/ccm/CCM_Cookie_s.h>

namespace CORBA {
namespace Internal {

void ReceptacleSimplexBase::connect (I_ptr <Interface> p)
{
	if (connection_)
		throw Components::AlreadyConnected ();
	connection_ = p;
}

void ReceptacleSimplexBase::disconnect (I_ref <Interface>& conn)
{
	if (!connection_)
		throw Components::NoConnection ();
	conn = std::move (connection_);
}

void ReceptacleSimplexBase::get_connection (I_ref <Interface>& conn) const noexcept
{
	conn = connection_;
}

NIRVANA_IMPORT (cookie_factory, RepIdOf <Components::Cookie>::id, ValueFactoryBase)

class CCMCookie : public IDL::traits <Components::Cookie>::Servant <CCMCookie>
{
	typedef IDL::traits <Components::Cookie>::Servant <CCMCookie> Base;

public:
	static Interface* __factory (Bridge <ValueBase>*, Interface*) noexcept
	{
		return interface_duplicate (&cookie_factory.operator I_ptr <ValueFactoryBase> ());
	}

	CCMCookie (size_t i) :
		Base (CORBA::OctetSeq ((const Octet*)&i, (const Octet*)(&i + 1)))
	{}

	size_t index () const
	{
		if (cookieValue ().size () != sizeof (size_t))
			throw Components::InvalidConnection ();
		return *(size_t*)cookieValue ().data ();
	}
};

inline
Components::Cookie::_ref_type index2cookie (size_t i)
{
	return make_reference <CCMCookie> (i);
}

inline
size_t cookie2index (Components::Cookie::_ptr_type cookie)
{
	if (!cookie)
		throw Components::InvalidConnection ();
	return static_cast <const CCMCookie*> (static_cast <Bridge <Components::Cookie>*> (&cookie))->index ();
}

inline
Connection::Connection (I_ptr <Interface> p, size_t idx) :
	objref_ (p),
	ck_ (index2cookie (idx))
{}

void ReceptacleMultiplexBase::get_connections (Connections& connections) const
{
	for (size_t i = 0, cnt = connections_.size (); i < cnt; ++i) {
		I_ptr <Interface> c = connections_ [i];
		if (c)
			connections.emplace_back (c, i);
	}
}

void ReceptacleMultiplexBase::connect (I_ptr <Interface> p, I_ref <Components::Cookie>& ck)
{
	size_t i = 0;;
	for (size_t cnt = connections_.size (); i < cnt; ++i) {
		if (!connections_ [i])
			break;
	}
	if (i >= connections_.size ())
		connections_.emplace_back (p);
	else
		connections_ [i] = p;
	ck = index2cookie (i);
}

void ReceptacleMultiplexBase::disconnect (I_ptr <Components::Cookie> ck, I_ref <Interface>& cxn)
{
	size_t i = cookie2index (ck);
	if (i >= connections_.size () || !connections_ [i])
		throw Components::InvalidConnection ();
	cxn = std::move (connections_ [i]);
	if (i == connections_.size () - 1) {
		do {
			connections_.pop_back ();
		} while (!(connections_.empty () || connections_.back ()));
	}
}

}
}
