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
#ifndef NIRVANA_ORB_ENVIRONMENT_H_
#define NIRVANA_ORB_ENVIRONMENT_H_
#pragma once

#include "EnvironmentImpl.h"
#include "set_exception.h"

namespace CORBA {
namespace Internal {

class Environment :
	public EnvironmentImpl <Environment>
{
public:
	template <class Itf>
	static Interface* __duplicate (Interface*, Interface* env) noexcept
	{
		set_NO_IMPLEMENT (env);
		return 0;
	}

	template <class Itf>
	static void __release (Interface*) noexcept
	{}

	Environment () noexcept :
		user_exceptions_ (nullptr),
		user_exceptions_cnt_ (0)
	{}

	void exception_set (Short code, const char* rep_id, void* param) noexcept
	{
		EnvironmentBase::exception_set (code, rep_id, param, user_exceptions_, user_exceptions_cnt_);
	}

protected:
	Environment (const ExceptionEntry* user_exceptions, size_t user_exceptions_cnt) noexcept :
		user_exceptions_ (user_exceptions),
		user_exceptions_cnt_ (user_exceptions_cnt)
	{}

private:
	const ExceptionEntry* user_exceptions_;
	size_t user_exceptions_cnt_;
};

}
}

#endif
