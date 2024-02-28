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
#ifndef NIRVANA_ORB_SYSTEMEXCEPTION_H_
#define NIRVANA_ORB_SYSTEMEXCEPTION_H_
#pragma once

#include "Exception.h"
#include "StringView.h"
#include "system_exceptions.h"

#define MAKE_MINOR(vmcid, c) ((vmcid << 12) | (c))
#define MAKE_OMG_MINOR(c) (MAKE_MINOR (0x4F4D0, (c)))

namespace CORBA {

enum CompletionStatus : Internal::ABI_enum
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

	virtual void* __data () noexcept;

	uint32_t minor () const
	{
		return data_.minor;
	}

	void minor (uint32_t minor)
	{
		data_.minor = minor;
	}

	CompletionStatus completed () const
	{
		return data_.completed;
	}

	void completed (CompletionStatus status)
	{
		data_.completed = status;
	}

	static const SystemException* _downcast (const Exception* ep)
	{
		return (ep && (ep->__code () >= 0)) ? static_cast <const SystemException*> (ep) : nullptr;
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

	const _Data& _data () const noexcept
	{
		return data_;
	}

	static const Internal::ExceptionEntry* _get_exception_entry (Internal::String_in rep_id, Code hint = Exception::EC_USER_EXCEPTION);
	static const Internal::ExceptionEntry* _get_exception_entry (Internal::I_ptr <TypeCode> tc);
	
	static const Internal::ExceptionEntry* _get_exception_entry (Code code)
	{
		if (code < 0 || code >= KNOWN_SYSTEM_EXCEPTIONS)
			return nullptr;
		return &exception_entries_ [code].ee;
	}

	static void _raise_by_code (Code ec, unsigned minor = 0, CompletionStatus completed = COMPLETED_NO);

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
		data_.minor = 0;
		data_.completed = COMPLETED_NO;
	}

	SystemException (const SystemException& src) :
		data_ (src.data_)
	{}

	SystemException (ULong minor, CompletionStatus status = COMPLETED_NO)
	{
		data_.minor = minor;
		data_.completed = status;
	}

	SystemException& operator = (const SystemException& src)
	{
		data_ = src.data_;
		return *this;
	}

private:
	_Data data_;

	struct ExceptionEntry
	{
		Internal::ExceptionEntry ee;
		size_t rep_id_len;
	};

	static const ExceptionEntry exception_entries_ [KNOWN_SYSTEM_EXCEPTIONS];
};

}

#endif
