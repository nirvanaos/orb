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

namespace CORBA {
namespace Internal {

class EnvironmentBase
{
public:
	void exception_set (Short code, String_in rep_id, void* param,
		const ExceptionEntry* user_exceptions = nullptr, size_t user_exceptions_cnt = 0) noexcept;

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

	EnvironmentBase& operator = (EnvironmentBase&& src) noexcept;

protected:
	EnvironmentBase () noexcept
	{
		data_.reset ();
	}

	~EnvironmentBase () noexcept
	{
		if (data_.is_small || data_.ptr)
			exception_free ();
	}

	EnvironmentBase (EnvironmentBase&& src) noexcept :
		data_ (src.data_)
	{
		src.data_.reset ();
	}

private:
	bool set (const ExceptionEntry& ee) noexcept;
	void set_unknown (uint32_t minor) noexcept;
	void set_user (const ExceptionEntry& ee, void* data) noexcept;
	bool set_user (String_in rep_id, void* param, const ExceptionEntry* user_exceptions, size_t user_exceptions_cnt) noexcept;

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
	public InterfaceImplBase <S, ::CORBA::Environment>,
	public ServantTraits <S>,
	public EnvironmentBase
{
protected:
	EnvironmentImpl ()
	{}
};

}
}

#endif
