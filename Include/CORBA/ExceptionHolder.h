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
#ifndef NIRVANA_ORB_EXCEPTIONHOLDER_H_
#define NIRVANA_ORB_EXCEPTIONHOLDER_H_
#pragma once

#include "ValueBaseImpl.h"
#include "ExceptionList.h"
#include "ExceptionSet.h"
#include "Any.h"

namespace Messaging {
class ExceptionHolder;
}

namespace CORBA {
namespace Internal {

template <>
const Char RepIdOf <Messaging::ExceptionHolder>::id [] = "IDL:omg.org/Messaging/ExceptionHolder:1.0";

template <>
class Bridge <Messaging::ExceptionHolder> : public Interface
{
public:
	/// Entry-point vector
	struct EPV
	{
		Interface::EPV header;
		struct
		{
			Bridge <ValueBase>* (*CORBA_ValueBase) (Bridge <Messaging::ExceptionHolder>*, Type <String>::ABI_in, Interface*);
		}
		base;
	};

protected:
	Bridge <Messaging::ExceptionHolder> () noexcept :
		Interface (epv_.header)
	{}

	Bridge <Messaging::ExceptionHolder> (const Bridge <Messaging::ExceptionHolder>&) noexcept :
		Interface (epv_.header)
	{}

protected:
	static const Bridge <Messaging::ExceptionHolder>::EPV epv_;
};

}
}

namespace Messaging {

#ifdef LEGACY_CORBA_CPP
typedef CORBA::Internal::I_ptr <ExceptionHolder> ExceptionHolder_ptr;
typedef CORBA::Internal::I_var <ExceptionHolder> ExceptionHolder_var;
#endif

class ExceptionHolder :
	public CORBA::Internal::Bridge <ExceptionHolder>,
	public CORBA::Internal::RefCountBase <ExceptionHolder>,
	public CORBA::Internal::LifeCycleRefCnt <ExceptionHolder>,
	public CORBA::Internal::ValueImplBase <ExceptionHolder, CORBA::ValueBase>,
	public CORBA::Internal::ValueNonTruncatable,
	public CORBA::Internal::ValueBaseNoFactory
{
public:
	typedef CORBA::Internal::I_ptr <ExceptionHolder> _ptr_type;

#ifdef LEGACY_CORBA_CPP
	typedef CORBA::Internal::I_var <ExceptionHolder> _var_type;
	typedef _var_type& _out_type;

	NIRVANA_NODISCARD static _ptr_type _duplicate (_ptr_type obj)
	{
		return _unsafe_cast (interface_duplicate (&obj));
	}

	using LifeCycleRefCnt <ExceptionHolder>::_duplicate;

#else
	typedef CORBA::Internal::I_ref <ExceptionHolder> _ref_type;
#endif

	ExceptionHolder (CORBA::Any&& exc) :
		exception_ (std::move (exc))
	{}

	Interface* _query_valuetype (CORBA::Internal::String_in id) noexcept
	{
		if (id.empty () || CORBA::Internal::RepId::compatible (CORBA::Internal::RepIdOf <ExceptionHolder>::id, id))
			return &static_cast <CORBA::Internal::Bridge <ExceptionHolder>&> (*this);
		return nullptr;
	}

	static ExceptionHolder& _implementation (CORBA::Internal::Bridge <CORBA::ValueBase>* bridge)
	{
		CORBA::Internal::check_pointer (bridge, CORBA::Internal::Skeleton <ExceptionHolder, CORBA::ValueBase>::epv_.header);
		return static_cast <ExceptionHolder&> (*bridge);
	}

	static ExceptionHolder& _implementation (CORBA::Internal::Bridge <ExceptionHolder>* bridge)
	{
		CORBA::Internal::check_pointer (bridge, CORBA::Internal::Bridge <ExceptionHolder>::epv_.header);
		return reinterpret_cast <ExceptionHolder&> (*bridge);
	}

	static void __marshal (CORBA::Internal::Bridge <CORBA::ValueBase>*, CORBA::Internal::Interface*,
		CORBA::Internal::Interface* _env);
	static void __unmarshal (CORBA::Internal::Bridge <CORBA::ValueBase>*, CORBA::Internal::Interface*,
		CORBA::Internal::Interface* _env);

	static CORBA::Internal::Bridge <CORBA::ValueBase>* _CORBA_ValueBase (
		CORBA::Internal::Bridge <ExceptionHolder>* bridge,
		CORBA::Internal::Type <IDL::String>::ABI_in id, Interface* env);

	static _ptr_type _check (CORBA::Internal::Interface* bridge)
	{
		return _unsafe_cast (CORBA::Internal::Interface::_check (bridge, CORBA::Internal::RepIdOf <ExceptionHolder>::id));
	}

	static _ptr_type _downcast (CORBA::ValueBase::_ptr_type val)
	{
		if (val)
			return val->_query_valuetype (CORBA::Internal::RepIdOf <ExceptionHolder>::id)
			.template downcast <ExceptionHolder> ();
		return nullptr;
	}

	static _ptr_type _nil () noexcept
	{
		return _ptr_type (nullptr);
	}

	virtual CORBA::ValueBase::_ref_type _copy_value ();

	virtual void raise_exception ();

	void raise_exception_with_list (const Dynamic::ExceptionList& exc_list)
	{
		raise_exception (exc_list.data (), exc_list.size ());
	}

protected:
	void raise_exception (const CORBA::Internal::ExceptionEntry* user_exceptions,
		size_t user_exceptions_cnt) const;

private:
	friend class CORBA::Internal::I_ptr <CORBA::Internal::Interface>;
	friend class CORBA::Internal::I_ref <CORBA::Internal::Interface>;
	friend class CORBA::Internal::I_ref_base <ExceptionHolder>;

	static ExceptionHolder* _unsafe_cast (CORBA::Internal::Interface* itf) noexcept
	{
		assert (!itf || CORBA::Internal::RepId::compatible (itf->_epv ().interface_id, CORBA::Internal::RepIdOf <ExceptionHolder>::id));
		return static_cast <ExceptionHolder*> (static_cast <CORBA::Internal::Bridge <ExceptionHolder>*> (itf));
	}

private:
	CORBA::Any exception_;
};

template <class ... Exceptions>
class ExceptionHolderEx : public ExceptionHolder
{
	typedef ExceptionHolderEx <Exceptions...> ThisClass;

public:
	virtual void raise_exception () override
	{
		raise_exception (CORBA::Internal::ExceptionSet <Exceptions...>::exceptions_,
			CORBA::Internal::ExceptionSet <Exceptions...>::count ());
	}

	virtual CORBA::ValueBase::_ref_type _copy_value ()
	{
		return CORBA::make_reference <ThisClass> (std::ref (*this));
	}
};

}

#endif
