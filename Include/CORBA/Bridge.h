/// \file Bridge.h

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
#ifndef NIRVANA_ORB_BRIDGE_H_
#define NIRVANA_ORB_BRIDGE_H_

#include "Interface.h"

namespace CORBA {
namespace Nirvana {

/// The ABI for a particular interface I.
/// A "bridge" between the client and servant sides.
template <class I>
class Bridge :
	public Interface
{
public:
	/// Entry-point vector
	struct EPV;

	const EPV& _epv () const
	{
		return (const EPV&)Interface::_epv ();
	}

	/// Interface repository id
	static const Char repository_id_ [];

	/// Helper for widening to a base interface
	template <class Base>
	struct Wide
	{
		typedef Bridge <Base>* (*Func) (Bridge <I>*, String_in base_id, Interface* environment);
	};

protected:
	Bridge (const EPV& epv) :
		Interface (epv.header)
	{
#ifdef NIRVANA_C11
		static_assert (offsetof(EPV, header) == 0, "header must be at the beginning of EPV.");
#endif
	}
};

#define BASE_STRUCT_ENTRY(type, name) MyBridge::Wide <type>::Func name;\
operator const MyBridge::Wide < type>::Func () const { return name; }

#define BRIDGE_BEGIN(I, id) template <> const Char Bridge <I>::repository_id_ [] = id;\
template <> struct Bridge < I>::EPV { typedef Bridge <I> MyBridge; Interface::EPV header; struct {
#define BRIDGE_EPV } base; struct {
#define BRIDGE_END() } epv;};

}
}

#define CORBA_REPOSITORY_ID(t) "IDL:omg.org/CORBA/" t ":1.0"
#define NIRVANA_REPOSITORY_ID(t) "IDL:Nirvana/" t ":1.0"
#define CORBA_NIRVANA_REPOSITORY_ID(t) "IDL:CORBA/Nirvana/" t ":1.0"
#define PORTABLESERVER_REPOSITORY_ID(t) "IDL:omg.org/PortableServer/" t ":1.0"

#endif
