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

#include "ProxyHolder.h"
#include "ProxyBaseInterface.h"
#include "../ServantImpl.h"
#include "../ObjectFactoryInc.h"
#include "../LifeCycleRefCnt.h"
#include "IOReference.h"
#include "OperationIndex.h"
#include "RqProcWrapper.h"

namespace CORBA {
namespace Internal {

template <class I> class Proxy;

class ProxyRoot : public DynamicExport
{
	void* operator new (size_t size) = delete;

public:
	void operator delete (void* p, size_t size)
	{
		g_object_factory->memory_release (p, size);
	}

	void* operator new (size_t, void* p) noexcept
	{
		return p;
	}

	void operator delete (void*, void*) noexcept
	{}

	// For make_stateless()
	static void _create_proxy () noexcept
	{}

	Bridge <Object>* _get_object (Type <String>::ABI_in id, Interface* env) const noexcept
	{
		return object_.get_bridge (id, env);
	}

	Bridge <AbstractBase>* _get_abstract_base (Type <String>::ABI_in id, Interface* env) const noexcept
	{
		return abstract_base_.get_bridge (id, env);
	}

	void _add_ref ()
	{
		interface_duplicate (&proxy_manager_);
	}

	void _remove_ref () noexcept
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

	OperationIndex _make_op_idx (UShort op_idx) const noexcept
	{
		return make_op_idx (interface_idx_, op_idx);
	}

	static void check_request (IORequest::_ptr_type rq);

	static void set_marshal_local (Interface* env);

protected:
	ProxyRoot (Object::_ptr_type obj, AbstractBase::_ptr_type ab, IOReference::_ptr_type proxy_manager, UShort interface_idx) :
		object_ (obj),
		abstract_base_ (ab),
		proxy_manager_ (proxy_manager),
		interface_idx_ (interface_idx)
	{}

private:
	ProxyLink <Object> object_;
	ProxyLink <AbstractBase> abstract_base_;
	IOReference::_ptr_type proxy_manager_;
	UShort interface_idx_;
};

template <class I, class ... Bases>
class ProxyBase :
	public InterfaceImplBase <Proxy <I>, I>,
	public ProxyHolderImpl <Proxy <I> >,
	public ProxyRoot,
	public ProxyBaseInterface <Bases>...,
	public ServantTraits <Proxy <I> >,
	public LifeCycleRefCnt <Proxy <I> >,
	public RqProcWrapper <I>
{
public:
	// Wide interface
	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <I>* derived, Type <String>::ABI_in id, Interface* env)
	{
		return static_cast <ProxyBaseInterface <Base>&> (Proxy <I>::_implementation (derived)).get_bridge (id, env);
	}

	// Override dummy implementation of ServantTraits::_wide_object.
	template <class Derived>
	static Bridge <Object>* _wide_object (Bridge <Derived>* derived, Type <String>::ABI_in id, Interface* env)
	{
		return Proxy <I>::_implementation (derived)._get_object (id, env);
	}

protected:
	ProxyBase (Object::_ptr_type obj, AbstractBase::_ptr_type ab,
		IOReference::_ptr_type proxy_manager, UShort interface_idx, Interface*& servant) :
		ProxyRoot (obj, ab, proxy_manager, interface_idx),
		ProxyBaseInterface <Bases> (obj)...
	{}
};

template <class I, class ... Bases>
class ProxyBaseStateless : public ProxyBase <I, Bases...>
{
	typedef ProxyBase <I, Bases...> Base;

protected:
	ProxyBaseStateless (Object::_ptr_type obj, AbstractBase::_ptr_type ab,
		IOReference::_ptr_type proxy_manager, UShort interface_idx, Interface*& servant) :
		Base (obj, ab, proxy_manager, interface_idx, servant),
		servant_ (reinterpret_cast <I*&> (servant))
	{}

	I* servant () const noexcept
	{
		return servant_;
	}

private:
	I*& servant_;
};

}
}

#endif
