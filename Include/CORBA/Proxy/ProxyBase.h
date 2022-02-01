/// \file ProxyBase.h
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
#ifndef NIRVANA_ORB_PROXYBASE_H_
#define NIRVANA_ORB_PROXYBASE_H_

#include "../CORBA.h"
#include "../ServantImpl.h"
#include "../ServantMemory.h"
#include "../DynamicServant_s.h"
#include "../LifeCycleRefCnt.h"
#include "IOReference.h"

namespace CORBA {
namespace Internal {

inline OperationIndex make_op_idx (UShort itf_idx, UShort op_idx)
{
	return (ULong)itf_idx << 16 | op_idx;
}

inline UShort interface_idx (OperationIndex oi)
{
	return oi >> 16;
}

inline UShort operation_idx (OperationIndex oi)
{
	return (UShort)oi;
}

template <class I> class Proxy;

class ProxyRoot : public ServantMemory
{
public:
	Bridge <Object>* _get_object (String_in iid) const
	{
		return &proxy_manager_->object ();
	}

	void _add_ref ()
	{
		interface_duplicate (&proxy_manager_);
	}

	void _remove_ref () NIRVANA_NOEXCEPT
	{
		interface_release (&proxy_manager_);
	}

	IOReference::_ptr_type _target () const
	{
		return proxy_manager_;
	}

	UShort _interface_idx () const
	{
		return interface_idx_;
	}

	ULong _make_op_idx (UShort op_idx) const
	{
		return make_op_idx (interface_idx_, op_idx);
	}

protected:
	ProxyRoot (IOReference::_ptr_type proxy_manager, UShort interface_idx) :
		proxy_manager_ (proxy_manager),
		interface_idx_ (interface_idx)
	{}

private:
	IOReference::_ptr_type proxy_manager_;
	UShort interface_idx_;
};

template <class I>
class ProxyBaseInterface
{
public:
	void init (AbstractBase::_ptr_type ab)
	{
		proxy_ = static_cast <Bridge <I>*> (&ab->_query_interface (I::repository_id_));
		if (!proxy_)
			throw OBJ_ADAPTER ();
	}

	Bridge <I>* get () const
	{
		return proxy_;
	}

private:
	Bridge <I>* proxy_;
};

template <class S>
class ProxyLifeCycle :
	public ServantTraits <S>,
	public LifeCycleRefCnt <S>,
	public InterfaceImplBase <S, DynamicServant>
{
public:
	DynamicServant* _dynamic_servant ()
	{
		return &static_cast <DynamicServant&> (static_cast <Bridge <DynamicServant>&> (*this));
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, String_in id, Interface* env)
	{
		try {
			if (!RepId::compatible (Bridge <Base>::repository_id_, id))
				::Nirvana::throw_INV_OBJREF ();
			return static_cast <ProxyBaseInterface <Base>&> (S::_implementation (derived)).get ();
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}
};

template <class I>
class ProxyBase :
	public ProxyRoot,
	public ProxyLifeCycle <Proxy <I> >,
	public InterfaceImplBase <Proxy <I>, I>
{
public:
	Interface* _proxy ()
	{
		return &static_cast <Bridge <I>&> (*this);
	}

protected:
	ProxyBase (IOReference::_ptr_type proxy_manager, UShort interface_idx) :
		ProxyRoot (proxy_manager, interface_idx)
	{}
};

}
}

#endif
