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

#include "StringBase.h"
#include "Bridge.h"

namespace CORBA {
namespace Internal {

struct InterfaceEntry
{
	const Char* interface_id;
	size_t id_len;
	Interface* (*cast) (void* servant);

	static Interface* find (const InterfaceEntry* begin, const InterfaceEntry* end,
		void* servant, String_in id) NIRVANA_NOEXCEPT;
};

template <class S, class Primary, class ... I>
class InterfaceFinder
{
	template <class Itf>
	static Interface* cast (void* servant)
	{
		return &static_cast <Bridge <Itf>&> (*reinterpret_cast <S*> (servant));
	}

public:
	static Interface* find (S& servant, String_in id) NIRVANA_NOEXCEPT;

private:
	static const InterfaceEntry itable_ [];
};

template <class S, class Primary, class ... I>
const InterfaceEntry InterfaceFinder <S, Primary, I...>::itable_ [] = {
	{ RepIdOf <Primary>::id_, countof (RepIdOf <Primary>::id_) - 1, cast <Primary> },
	{ RepIdOf <I>::id_, countof (RepIdOf <I>::id_) - 1, cast <I> }...
};

template <class S, class Primary, class ... I>
inline Interface* InterfaceFinder <S, Primary, I...>::find (S& servant, String_in id) NIRVANA_NOEXCEPT
{
	return InterfaceEntry::find (itable_, itable_ + countof (itable_), &servant, id);
}

template <class Primary, class ... I>
class FindInterface
{
public:
	template <class S>
	static Interface* find (S& servant, String_in id) NIRVANA_NOEXCEPT
	{
		return InterfaceFinder <S, Primary, I...>::find (servant, id);
	}
};

}
}

#endif
