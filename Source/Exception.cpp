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
#include "../../pch/pch.h"
#include <stdexcept>

namespace CORBA {
namespace Internal {

void set_exception (Interface* environment, Exception::Code code, const Char* rep_id, void* param) noexcept
{
	if (environment && RepId::compatible (environment->_epv ().interface_id, RepIdOf < ::CORBA::Environment>::id)) {
		Bridge < ::CORBA::Environment>* b = &static_cast <Bridge < ::CORBA::Environment>&> (*environment);
		(b->_epv ().epv.exception_set) (b, code, rep_id, param);
	}
}

void set_exception (Interface* environment, Exception& e) noexcept
{
	set_exception (environment, e.__code (), e._rep_id (), e.__data ());
}

void set_unknown_exception (Interface* environment) noexcept
{
	std::exception_ptr eptr = std::current_exception ();
	if (eptr)
		try {
			std::rethrow_exception (eptr);
		} catch (const std::bad_alloc&) {
			set_exception (environment, SystemException::EC_NO_MEMORY, CORBA_REPOSITORY_ID ("NO_MEMORY"), nullptr);
			return;
		} catch (const std::invalid_argument&) {
			set_exception (environment, SystemException::EC_BAD_PARAM, CORBA_REPOSITORY_ID ("BAD_PARAM"), nullptr);
			return;
		} catch (...) {}
	set_exception (environment, SystemException::EC_UNKNOWN, CORBA_REPOSITORY_ID ("UNKNOWN"), nullptr);
}

}
}

