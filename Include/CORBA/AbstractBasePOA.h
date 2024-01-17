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
#ifndef NIRVANA_ORB_ABSTRACTBASEPOA_H_
#define NIRVANA_ORB_ABSTRACTBASEPOA_H_
#pragma once

#include "ServantPOA.h"
#include "AbstractBase_s.h"
#include "LifeCycleRefCnt.h"

namespace CORBA {
namespace Internal {

/// POA implementation of AbstractBase
template <>
class NIRVANA_NOVTABLE ServantPOA <AbstractBase> :
	public ServantTraitsPOA,
	public LifeCycleRefCnt <ServantPOA <AbstractBase> >,
	public InterfaceImplBase <ServantPOA <AbstractBase>, AbstractBase>
{
public:
	/// This method is called only for value types.
	/// The core proxy manager uses it's own implementation of AbstractBase.
	Type <Object>::VRet _to_object () noexcept
	{
		return nullptr;
	}

	/// Returns nil by default.
	/// Overridden in value types.
	virtual Type <ValueBase>::VRet _to_value () noexcept
	{
		return nullptr;
	}

	Bridge <AbstractBase>* _get_abstract_base (Type <String>::ABI_in iid,
		Interface* env) noexcept
	{
		if (!RepId::compatible (RepIdOf <AbstractBase>::id, Type <String>::in (iid)))
			set_INV_OBJREF (env);
		return this;
	}

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class LifeCycleRefCnt;
#endif

	virtual void _add_ref () = 0;
	virtual void _remove_ref () = 0;

public:
	virtual ULong _refcount_value () = 0;

protected:
	ServantPOA ()
	{}

	virtual ~ServantPOA ()
	{}
};

}
}

#endif
