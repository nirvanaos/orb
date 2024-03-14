/// \file SystemException implementation details.
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
#ifndef NIRVANA_ORB_SYSTEMEXCEPTION_IMPL_H_
#define NIRVANA_ORB_SYSTEMEXCEPTION_IMPL_H_
#pragma once

#include "SystemException.h"
#include "TypeEnum.h"
#include "TypeFixLen.h"
#include <Nirvana/ImportInterface.h>
#include <Nirvana/bitutils.h>

namespace CORBA {

extern NIRVANA_STATIC_IMPORT::Nirvana::ImportInterfaceT <TypeCode> _tc_CompletionStatus;

namespace Internal {

template <>
const Char RepIdOf <CompletionStatus>::id [] = CORBA_REPOSITORY_ID ("CompletionStatus");

template <>
struct Type <CompletionStatus> :
	public TypeEnum <CompletionStatus, CompletionStatus::COMPLETED_MAYBE>
{
	static I_ptr <TypeCode> type_code ()
	{
		return _tc_CompletionStatus;
	}
};

template <>
struct Type <SystemException::_Data> : TypeFixLen <SystemException::_Data,
	false // Do not check system exception data
> {
	static const bool is_CDR = true;
	static const bool is_CDR_seq = true;

	static void byteswap (Var& v) noexcept
	{
		v.minor = Nirvana::byteswap (v.minor);
		v.completed = (CompletionStatus)Nirvana::byteswap ((ABI_enum)v.completed);
	}
};

}
}

#endif
