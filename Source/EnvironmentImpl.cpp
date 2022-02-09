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
#include <CORBA/CORBA.h>

namespace CORBA {
namespace Internal {

void EnvironmentBase::exception_free () NIRVANA_NOEXCEPT
{
	if (data_.is_small) {
		((Exception*)&data_)->~Exception ();
	} else if (data_.ptr) {
		data_.ptr->~Exception ();
		::Nirvana::g_memory->release (data_.ptr, data_.size);
	}
	data_.is_small = 0;
	data_.ptr = nullptr;
}

void EnvironmentBase::check () const
{
	const Exception* ex = exception ();
	if (ex)
		ex->_raise ();
}

const Char* EnvironmentBase::exception_id () const NIRVANA_NOEXCEPT
{
	const Exception* ex = exception ();
	if (ex)
		return ex->_rep_id ();
	else
		return nullptr;
}

const void* EnvironmentBase::exception_value () const NIRVANA_NOEXCEPT
{
	const Exception* ex = exception ();
	if (ex)
		return ex->__data ();
	else
		return nullptr;
}

void EnvironmentBase::exception (Exception* ex) NIRVANA_NOEXCEPT
{
	exception_free ();
	data_.ptr = ex;
}

void EnvironmentBase::exception_set (Short code, String_in rep_id, void* param,
	const ExceptionEntry* user_exceptions) NIRVANA_NOEXCEPT
{
	exception_free ();
	if (code > Exception::EC_NO_EXCEPTION) {
		const ExceptionEntry* ee;
		if (!static_cast <const String&> (rep_id).empty ()) {
			if (code < Exception::EC_SYSTEM_EXCEPTION && user_exceptions) {
				if (
					Exception::EC_UNKNOWN_USER_EXCEPTION == code
					&&
					RepId::compatible (RepIdOf <UnknownUserException>::repository_id_, rep_id)
					&& 
					param
				) {
					Any* pa = (Any*)param;
					I_ptr <TypeCode> tc = pa->type ();
					if (tc) {
						try {
							assert (tc->kind () == TCKind::tk_except);
							if (tc->kind () == TCKind::tk_except && set_user (tc->id (), pa->data (), user_exceptions))
								return;
						} catch (...) {
						}
					}
				}
				if (set_user (rep_id, param, user_exceptions))
					return;
				code = Exception::EC_SYSTEM_EXCEPTION; // Will set UNKNOWN
			}
			ee = SystemException::_get_exception_entry (rep_id, (Exception::Code)code);
		} else
			ee = SystemException::_get_exception_entry (code);
		assert (ee && ee->size <= sizeof (data_));
		set_system (*ee, param);
	}
}

bool EnvironmentBase::set_user (String_in rep_id, void* param,
	const ExceptionEntry* user_exceptions) NIRVANA_NOEXCEPT
{
	for (const ExceptionEntry* p = user_exceptions; p->rep_id; ++p) {
		if (RepId::compatible (p->rep_id, rep_id)) {
			set_user (*p, param);
			return true;
		}
	}
	return false;
}

bool EnvironmentBase::set (const ExceptionEntry& ee) NIRVANA_NOEXCEPT
{
	size_t size = ee.size;
	try {
		if (size <= sizeof (data_))
			ee.construct (&data_);
		else {
			Exception* p = (Exception*)::Nirvana::g_memory->allocate (nullptr, size, 0);
			try {
				ee.construct (p);
			} catch (...) {
				::Nirvana::g_memory->release (p, size);
				throw;
			}
			data_.ptr = (Exception*)p;
			data_.size = size;
		}
		return true;
	} catch (...) {
		new (&data_) NO_MEMORY ();
		return false;
	}
}

void EnvironmentBase::set_user (const ExceptionEntry& ee, void* data) NIRVANA_NOEXCEPT
{
	if (set (ee) && data && ee.size > sizeof (UserException)) {
		try {
			Exception& e = *exception ();
			I_ptr <TypeCode> tc = e.__type_code ();
			tc->n_move (e.__data (), data);
		} catch (...) {
			new (&data_) NO_MEMORY ();
		}
	}
}

void EnvironmentBase::set_system (const ExceptionEntry& ee, const void* data) NIRVANA_NOEXCEPT
{
	if (set (ee) && data) {
		SystemException& e = static_cast <SystemException&> (*exception ());
		e.minor (((SystemException::_Data*)data)->minor);
		e.completed (((SystemException::_Data*)data)->completed);
	}
}

void EnvironmentBase::move_from (EnvironmentBase& src) NIRVANA_NOEXCEPT
{
	exception_free ();
	data_ = src.data_;
	src.data_.reset ();
}

}
}
