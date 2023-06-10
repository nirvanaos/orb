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
#ifndef NIRVANA_ORB_ENVIRONMENTIMPL_H_
#define NIRVANA_ORB_ENVIRONMENTIMPL_H_
#pragma once

#include "ServantImpl.h"
#include "Environment_s.h"
#include "SystemException.h"
#include "set_exception.h"

namespace CORBA {
namespace Internal {

class EnvironmentBase :
	public Bridge <CORBA::Environment>
{
public:
	void exception_set (Short code, String_in rep_id, void* param,
		const ExceptionEntry* user_exceptions = 0) noexcept;

	const Char* exception_id () const noexcept;

	const void* exception_value () const noexcept;

	void exception_free () noexcept;

	void clear () noexcept
	{
		exception_free ();
	}

	Exception* exception () const noexcept
	{
		if (data_.is_small)
			return (Exception*)(&data_);
		else
			return data_.ptr;
	}

	void exception (Exception* ex) noexcept;

	void check () const;

protected:
	EnvironmentBase (const EPV& epv) noexcept :
		Bridge < ::CORBA::Environment> (epv)
	{
		data_.reset ();
	}

	~EnvironmentBase () noexcept
	{
		if (data_.is_small || data_.ptr)
			exception_free ();
	}

	void move_from (EnvironmentBase& src) noexcept;

private:
	bool set (const ExceptionEntry& ee) noexcept;
	void set_system (const ExceptionEntry& ee, const void* data) noexcept;
	void set_user (const ExceptionEntry& ee, void* data) noexcept;
	bool set_user (String_in rep_id, void* param, const ExceptionEntry* user_exceptions) noexcept;

private:
	union Data
	{
		uintptr_t small [(sizeof (SystemException) + sizeof (uintptr_t) - 1) / sizeof (uintptr_t)];
		struct
		{
			// If small data contains Exception object, the first word of it
			// contains pointer to the virtual table and can't be zero.
			uintptr_t is_small;
			Exception* ptr;
			size_t size;
		};

		void reset ()
		{
			is_small = 0;
			ptr = 0;
		}
	} data_;
};

template <class S>
class EnvironmentImpl :
	public EnvironmentBase,
	public Skeleton <S, ::CORBA::Environment>,
	public ServantTraits <S>
{
public:
	template <class I>
	static Interface* __duplicate (Interface*, Interface* env)
	{
		set_NO_IMPLEMENT (env);
		return 0;
	}

	template <class I>
	static void __release (Interface*)
	{}

protected:
	EnvironmentImpl () :
		EnvironmentBase (Skeleton <S, ::CORBA::Environment>::epv_)
	{}
};

}
}

#endif
