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
#include "RefCountBase.h"

namespace Messaging {

class ExceptionHolder;

#ifdef LEGACY_CORBA_CPP
typedef CORBA::Internal::I_ptr <ExceptionHolder> ExceptionHolder_ptr;
typedef CORBA::Internal::I_var <ExceptionHolder> ExceptionHolder_var;
#endif

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

	const EPV& _epv () const noexcept
	{
		return (const EPV&)Interface::_epv ();
	}

	operator I_ptr <ValueBase> ()
	{
		Environment env;
		return static_cast <ValueBase*> (this->_get_bridge_ptr (env));
	}

protected:
	Bridge (const EPV& epv) noexcept :
		Interface (epv.header)
	{}

	Bridge <ValueBase>* _get_bridge_ptr (EnvironmentBase& env)
	{
		Bridge <ValueBase>* ret = (_epv ().base.CORBA_ValueBase) (this, &StringView <Char> (RepIdOf <ValueBase>::id), &env);
		env.check ();
		return ret;
	}

	Bridge <ValueBase>& _get_bridge (EnvironmentBase& env)
	{
		Bridge <ValueBase>* ret = _get_bridge_ptr (env);
		if (!ret)
			::Nirvana::throw_INV_OBJREF ();
		return *ret;
	}
};

class ExceptionHolderImpl :
	public BridgeVal <Messaging::ExceptionHolder>,
	public RefCountBase <ExceptionHolderImpl>,
	public LifeCycleRefCnt <ExceptionHolderImpl>,
	public ValueImplBase <ExceptionHolderImpl, ValueBase>,
	public ValueBaseCopy <ExceptionHolderImpl>,
	public ValueNonTruncatable,
	public ValueBaseNoFactory
{
public:
	static ExceptionHolderImpl& _implementation (Bridge <ValueBase>* bridge)
	{
		check_pointer (bridge, Skeleton <ExceptionHolderImpl, ValueBase>::epv_.header);
		return static_cast <ExceptionHolderImpl&> (*bridge);
	}

	static ExceptionHolderImpl& _implementation (Bridge <Messaging::ExceptionHolder>* bridge)
	{
		check_pointer (bridge, epv_.header);
		return static_cast <ExceptionHolderImpl&> (*bridge);
	}

	static Bridge <ValueBase>* _CORBA_ValueBase (Bridge <Messaging::ExceptionHolder>* bridge,
		Type <String>::ABI_in id, Interface* env);

	ExceptionHolderImpl (Any&& exc) :
		BridgeVal <Messaging::ExceptionHolder> (epv_),
		exception_ (std::move (exc)),
		user_exceptions_ (nullptr),
		user_exceptions_cnt_ (0)
	{}

	Interface* _query_valuetype (String_in id) noexcept
	{
		if (id.empty () || RepId::compatible (RepIdOf <Messaging::ExceptionHolder>::id, id))
			return &static_cast <Bridge <Messaging::ExceptionHolder>&> (*this);
		return nullptr;
	}

	void raise_exception ()
	{
		raise_exception (user_exceptions_, user_exceptions_cnt_);
	}

	void raise_exception_with_list (const Dynamic::ExceptionList& exc_list)
	{
		raise_exception (exc_list.data (), exc_list.size ());
	}

protected:
	ExceptionHolderImpl (Any&& exc, const ExceptionEntry* user_exceptions, size_t user_exceptions_cnt) :
		BridgeVal <Messaging::ExceptionHolder> (epv_),
		exception_ (std::move (exc)),
		user_exceptions_ (user_exceptions),
		user_exceptions_cnt_ (user_exceptions_cnt)
	{}

	void raise_exception (const ExceptionEntry* user_exceptions, size_t user_exceptions_cnt) const;

private:
	Any exception_;
	const ExceptionEntry* user_exceptions_;
	size_t user_exceptions_cnt_;

	static const Bridge <Messaging::ExceptionHolder>::EPV epv_;
};

template <class ... Exceptions>
class ExceptionHolderEx : public ExceptionHolderImpl
{
public:
	ExceptionHolderEx (Any&& exc) :
		ExceptionHolderImpl (std::move (exc), ExceptionSet <Exceptions...>::entries (),
			ExceptionSet <Exceptions...>::count ())
	{}
};

}
}

namespace Messaging {

class ExceptionHolder : public CORBA::Internal::Client <CORBA::Internal::ExceptionHolderImpl, CORBA::ValueBase>
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

#else
	typedef CORBA::Internal::I_ref <ExceptionHolder> _ref_type;
#endif

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

private:
	friend class CORBA::Internal::I_ptr <CORBA::Internal::Interface>;
	friend class CORBA::Internal::I_ref <CORBA::Internal::Interface>;
	friend class CORBA::Internal::I_ref_base <ExceptionHolder>;

	static ExceptionHolder* _unsafe_cast (CORBA::Internal::Interface* itf) noexcept
	{
		assert (!itf || CORBA::Internal::RepId::compatible (itf->_epv ().interface_id, CORBA::Internal::RepIdOf <ExceptionHolder>::id));
		return static_cast <ExceptionHolder*> (static_cast <CORBA::Internal::Bridge <ExceptionHolder>*> (itf));
	}
};

}

#endif
