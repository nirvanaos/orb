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
#ifndef NIRVANA_ORB_REFCOUNTBASE_H_
#define NIRVANA_ORB_REFCOUNTBASE_H_
#pragma once

#include <Nirvana/Memory_forward.h>

namespace CORBA {

template <class> class servant_reference;

template <class T, class ... Args>
servant_reference <T> make_reference (Args ... args);

namespace Internal {

template <class S>
class LifeCycleRefCnt;

template <class T>
class RefCountBase
{
#ifdef LEGACY_CORBA_CPP
public:
#else
private:
	template <class T1, class ... Args>
	friend CORBA::servant_reference <T1> CORBA::make_reference (Args ... args);
	template <class> friend class CORBA::servant_reference;
	friend class LifeCycleRefCnt <T>;
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
			delete& static_cast <T&> (*this);
	}

public:
	ULong _refcount_value () const NIRVANA_NOEXCEPT
	{
		return ref_cnt_;
	}

protected:
	RefCountBase () NIRVANA_NOEXCEPT :
		ref_cnt_ (1)
	{}

	RefCountBase (const RefCountBase&) NIRVANA_NOEXCEPT :
		ref_cnt_ (1)
	{}

	RefCountBase& operator = (const RefCountBase&) NIRVANA_NOEXCEPT
	{
		return *this;
	}

#ifdef LEGACY_CORBA_CPP
public:
#else
private:
#endif
	void operator delete (void* p, size_t size)
	{
		Nirvana::g_memory->release (p, size);
	}

protected:
	ULong ref_cnt_;
};

}
}

#endif
