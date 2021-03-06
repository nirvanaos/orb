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
#ifndef NIRVANA_ORB_SERVANTMEMORY_H_
#define NIRVANA_ORB_SERVANTMEMORY_H_
#pragma once

#include "core_objects.h"
#include "Client.h"
#include "ObjectFactoryInc.h"

namespace CORBA {

template <class> class servant_reference;
template <class T, class ... Args>
servant_reference <T> make_reference (Args ... args);

namespace Internal {

class ServantMemory
{
#ifdef LEGACY_CORBA_CPP
public:
#else
	template <class T, class ... Args>
	friend CORBA::servant_reference <T> CORBA::make_reference (Args ... args);
#endif

	void* operator new (size_t size)
	{
		return g_object_factory->memory_allocate (size);
	}

public:
	void operator delete (void* p, size_t size)
	{
		g_object_factory->memory_release (p, size);
	}

	void* operator new (size_t, void* p)
	{
		return p;
	}

	void operator delete (void*, void*)
	{}
};

}
}

#endif
