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
#ifndef NIRVANA_ORB_SYSTEMEXCEPTION_H_
#define NIRVANA_ORB_SYSTEMEXCEPTION_H_

#include "Exception.h"
#include "system_exceptions.h"
#include "TypeEnum.h"
#include <Nirvana/ImportInterface.h>

#define OMGVMCID 0x4f4d0000
#define MAKE_MINOR(vmcid, c) (vmcid | c)
#define MAKE_OMG_MINOR(c) (MAKE_MINOR (OMGVMCID, c))

namespace CORBA {

enum CompletionStatus : uint32_t
{
	COMPLETED_YES,
	COMPLETED_NO,
	COMPLETED_MAYBE
};

class NIRVANA_NOVTABLE SystemException : public Exception
{
public:
	struct _Data
	{
		uint32_t minor;
		CompletionStatus completed;
	};

	virtual void* __data () NIRVANA_NOEXCEPT;

	ULong minor () const
	{
		return _data.minor;
	}

	void minor (ULong minor)
	{
		_data.minor = minor;
	}

	CompletionStatus completed () const
	{
		return _data.completed;
	}

	void completed (CompletionStatus status)
	{
		_data.completed = status;
	}

	static const SystemException* _downcast (const Exception* ep)
	{
		return (ep && (ep->__code () >= 0)) ? static_cast <const SystemException*> (ep) : 0;
	}

	static SystemException* _downcast (Exception* ep)
	{
		return const_cast <SystemException*> (_downcast ((const Exception*)ep));
	}

	static const SystemException* _narrow (const Exception* ep)
	{
		return _downcast (ep);
	}

	static SystemException* _narrow (Exception* ep)
	{
		return _downcast (ep);
	}

	static const Nirvana::ExceptionEntry* _get_exception_entry (String_in rep_id, Code hint = Exception::EC_USER_EXCEPTION);
	static const Nirvana::ExceptionEntry* _get_exception_entry (TypeCode_ptr tc);
	
	static const Nirvana::ExceptionEntry* _get_exception_entry (Code code)
	{
		if (code < 0 || code >= KNOWN_SYSTEM_EXCEPTIONS)
			code = EC_UNKNOWN;
		return &exception_entries_ [code].ee;
	}

	static void _raise_by_code (Code ec);

#define EXCEPTION_CODE(e) EC_##e,

	enum : Code
	{
		// System exception codes
		SYSTEM_EXCEPTIONS (EXCEPTION_CODE)

		KNOWN_SYSTEM_EXCEPTIONS
	};

#undef EXCEPTION_CODE

protected:
	SystemException ()
	{
		_data.minor = 0;
		_data.completed = COMPLETED_NO;
	}

	SystemException (const SystemException& src) :
		_data (src._data)
	{}

	SystemException (ULong minor, CompletionStatus status = COMPLETED_NO)
	{
		_data.minor = minor;
		_data.completed = status;
	}

	SystemException& operator = (const SystemException& src)
	{
		_data = src._data;
		return *this;
	}

private:
	_Data _data;

	struct ExceptionEntry
	{
		Nirvana::ExceptionEntry ee;
		size_t rep_id_len;
	};

	static const ExceptionEntry exception_entries_ [KNOWN_SYSTEM_EXCEPTIONS];
};

namespace Nirvana {

template <>
struct Type <CompletionStatus> :
	public TypeEnum <CompletionStatus, COMPLETED_MAYBE>
{};

}

extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_CompletionStatus;

}

#endif
