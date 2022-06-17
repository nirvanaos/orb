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
#ifndef NIRVANA_ORB_VALUEBASEIMPL_H_
#define NIRVANA_ORB_VALUEBASEIMPL_H_
#pragma once

#include "ReferenceCounterLink.h"
#include "LifeCycleRefCnt.h"
#include "ValueBase_s.h"

namespace CORBA {

template <class> class servant_reference;
template <class T, class ... Args>
servant_reference <T> make_reference (Args ... args);

namespace Internal {

//! Implementation of ValueBase.
//! \tparam S Servant class implementing operations.
template <class S>
class ValueImpl <S, ValueBase> :
	public LifeCycleRefCnt <S>,
	public ServantTraits <S>,
	public ValueImplBase <S, ValueBase>,
	public ReferenceCounterLink
{
#ifdef LEGACY_CORBA_CPP
public:
#else
	template <class T, class ... Args>
	friend CORBA::servant_reference <T> CORBA::make_reference (Args ... args);
#endif
	void* operator new (size_t size)
	{
		return Nirvana::g_memory->allocate (nullptr, size, 0);
	}

public:
	void operator delete (void* p, size_t size)
	{
		Nirvana::g_memory->release (p, size);
	}

	void _delete_object () NIRVANA_NOEXCEPT
	{
		delete& static_cast <S&> (*this);
	}

protected:
	ValueImpl ()
	{
		_construct (*this);
	}

	ValueImpl (const ValueImpl&)
	{
		_construct (*this);
	}
};

template <class S>
class ValueTraits
{
public:
	I_ref <ValueBase> _copy_value () const
	{
		return make_reference <S> (std::ref (static_cast <const S&> (*this)));
	}
};

}
}

#endif
