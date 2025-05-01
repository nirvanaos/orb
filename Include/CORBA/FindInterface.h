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
#ifndef NIRVANA_ORB_FINDINTERFACE_H_
#define NIRVANA_ORB_FINDINTERFACE_H_
#pragma once

#include "StringView.h"
#include "Bridge.h"

namespace CORBA {
namespace Internal {

struct InterfaceEntry
{
	const Char* interface_id;
	size_t id_len;
	Interface* (*cast) (void* servant);

	static Interface* find (const InterfaceEntry* begin, const InterfaceEntry* end,
		void* servant, String_in id);
};

template <class S, class Primary, class ... Itfs>
class InterfaceFinder
{
	template <class Itf>
	static constexpr Interface* cast (void* servant)
	{
		return &static_cast <Bridge <Itf>&> (*reinterpret_cast <S*> (servant));
	}

public:
	static Interface* find (S& servant, String_in id);

private:
	static const InterfaceEntry itable_ [];
};

template <class S, class Primary, class ... Itfs>
const InterfaceEntry InterfaceFinder <S, Primary, Itfs...>::itable_ [] = {
	{ RepIdOf <Primary>::id, countof (RepIdOf <Primary>::id) - 1, cast <Primary> },
	{ RepIdOf <Itfs>::id, countof (RepIdOf <Itfs>::id) - 1, cast <Itfs> }...
};

template <class S, class Primary, class ... Itfs>
inline Interface* InterfaceFinder <S, Primary, Itfs...>::find (S& servant, String_in id)
{
	return InterfaceEntry::find (itable_, itable_ + 1 + sizeof ... (Itfs), &servant, id);
}

template <class Primary, class ... Itf>
class FindInterface
{
public:
	template <class S>
	static Interface* find (S& servant, String_in id)
	{
		return InterfaceFinder <S, Primary, Itf...>::find (servant, id);
	}
};

}
}

#endif
