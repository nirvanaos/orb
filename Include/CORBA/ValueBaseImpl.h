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
	public ValueImplBase <S, ValueBase>
{
#ifdef LEGACY_CORBA_CPP
public:
#else
	template <class T, class ... Args>
	friend CORBA::servant_reference <T> CORBA::make_reference (Args ... args);
	template <class> friend class CORBA::Internal::LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
#endif
	void* operator new (size_t size)
	{
		return Nirvana::g_memory->allocate (nullptr, size, 0);
	}

	void _add_ref () NIRVANA_NOEXCEPT
	{
		++ref_cnt_;
	}

	void _remove_ref () NIRVANA_NOEXCEPT
	{
		assert (ref_cnt_);
		if (!--ref_cnt_)
			delete& static_cast <S&> (*this);
	}

	ULong _refcount_value () const NIRVANA_NOEXCEPT
	{
		return ref_cnt_;
	}

protected:
	ValueImpl () :
		ref_cnt_ (1)
	{}

	ValueImpl (const ValueImpl&) :
		ref_cnt_ (1)
	{}

	ValueImpl& operator = (const ValueImpl&) NIRVANA_NOEXCEPT
	{
		return *this; // Do nothing
	}

private:
	void operator delete (void* p, size_t size)
	{
		Nirvana::g_memory->release (p, size);
	}

private:
	ULong ref_cnt_;
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
