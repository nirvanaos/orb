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
#ifndef NIRVANA_ORB_EXCEPTION_H_
#define NIRVANA_ORB_EXCEPTION_H_

#include <Nirvana/NirvanaBase.h>
#include "primitive_types.h"
#include <exception>
#include <new>

namespace CORBA {

namespace Internal {
template <class I> class I_ptr;
class Interface;
}

class TypeCode;

class NIRVANA_NOVTABLE Exception : public std::exception
{
public:
	typedef int_fast16_t Code;
	typedef void _Data;

	virtual ~Exception ()
	{}
	virtual void _raise () const = 0;
	virtual const Char* _name () const NIRVANA_NOEXCEPT = 0;
	virtual const Char* _rep_id () const NIRVANA_NOEXCEPT = 0;

	// Nirvana specific
	virtual Code __code () const NIRVANA_NOEXCEPT = 0;
	virtual Internal::I_ptr <TypeCode> __type_code () const NIRVANA_NOEXCEPT = 0;

	virtual void* __data () NIRVANA_NOEXCEPT
	{
		return nullptr;
	}

	const void* __data () const NIRVANA_NOEXCEPT
	{
		return const_cast <Exception&> (*this).__data ();
	}

	enum : Code {
		EC_NO_EXCEPTION = -3,
		EC_USER_EXCEPTION = -2,
		EC_SYSTEM_EXCEPTION = -1
	};
};

namespace Internal {

struct ExceptionEntry
{
	const Char* rep_id;
	size_t size;
	void (*construct) (void*);
};

template <class E>
void construct (void* p)
{
	new (p) E ();
}

extern void set_exception (Interface* environment, Exception::Code code, const Char* rep_id, void* param) NIRVANA_NOEXCEPT;
extern void set_exception (Interface* environment, Exception& e) NIRVANA_NOEXCEPT;
extern void set_unknown_exception (Interface* environment) NIRVANA_NOEXCEPT;

}
} // namespace CORBA

#define NIRVANA_EXCEPTION_DCL(E) public:\
virtual void _raise () const { throw *this; }\
virtual const char* _rep_id () const NIRVANA_NOEXCEPT;\
virtual const char* _name () const NIRVANA_NOEXCEPT { return __name (); }\
static constexpr const char* __name () NIRVANA_NOEXCEPT { return #E; }\
virtual ::CORBA::Internal::I_ptr <::CORBA::TypeCode> __type_code () const NIRVANA_NOEXCEPT;\
static const E* _downcast (const ::CORBA::Exception* ep) NIRVANA_NOEXCEPT;\
static E* _downcast (::CORBA::Exception* ep) NIRVANA_NOEXCEPT { return const_cast <E*> (_downcast ((const ::CORBA::Exception*)ep)); }\
static const E* _narrow (const ::CORBA::Exception* ep) NIRVANA_NOEXCEPT { return _downcast (ep); }\
static E* _narrow (::CORBA::Exception* ep) NIRVANA_NOEXCEPT { return _downcast (ep); }\
virtual const char* what () const NIRVANA_NOEXCEPT { return __name (); }

#endif
