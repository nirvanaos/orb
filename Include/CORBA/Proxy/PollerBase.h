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
#ifndef NIRVANA_ORB_POLLERBASE_H_
#define NIRVANA_ORB_POLLERBASE_H_

#include "ProxyBaseInterface.h"
#include "../ServantImpl.h"
#include "../ObjectFactoryInc.h"
#include "../DynamicServantImpl.h"
#include "../LifeCycleRefCnt.h"
#include "IOReference.h"

namespace CORBA {
namespace Internal {

template <class I> class Poller;

class PollerRoot
{
public:
	void* operator new (size_t size)
	{
		return Nirvana::g_memory->allocate (nullptr, size, 0);
	}

	void operator delete (void* p, size_t size)
	{
		Nirvana::g_memory->release (p, size);
	}

	void _add_ref ()
	{
		interface_duplicate (&aggregate_);
	}

	void _remove_ref () noexcept
	{
		interface_release (&aggregate_);
	}

	IOReference::OperationIndex _make_op_idx (UShort op_idx) const noexcept
	{
		return IOReference::OperationIndex (interface_idx_, op_idx);
	}

	Bridge <Messaging::Poller>* messaging_poller (Type <String>::ABI_in id, Interface* env) const noexcept
	{
		return aggregate_.get_bridge (id, env);
	}

	Bridge <Pollable>* pollable (Type <String>::ABI_in id, Interface* env) const noexcept
	{
		return pollable_.get_bridge (id, env);
	}

	Bridge <ValueBase>* value_base (Type <String>::ABI_in id, Interface* env) const noexcept
	{
		return value_base_.get_bridge (id, env);
	}

	template <class ... Ex>
	IORequest::_ref_type _get_reply (uint32_t timeout, UShort op_idx) const
	{
		IORequest::_ref_type rq = aggregate_->get_reply (timeout, _make_op_idx (op_idx));
		check_request (rq, ExceptionSet <Ex...>::entries (), ExceptionSet <Ex...>::count ());
		return rq;
	}

protected:
	PollerRoot (ValueBase::_ptr_type vb, Messaging::Poller::_ptr_type aggregate,
		UShort interface_idx) :
		value_base_ (vb),
		aggregate_ (aggregate),
		pollable_ (aggregate),
		interface_idx_ (interface_idx)
	{}

private:
	static void check_request (IORequest::_ptr_type rq, const ExceptionEntry* user_exceptions, size_t user_exceptions_cnt);

private:
	ProxyLink <ValueBase> value_base_;
	ProxyLink < ::Messaging::Poller> aggregate_;
	ProxyLink <Pollable> pollable_;
	UShort interface_idx_;
};

template <class I, class ... Bases>
class PollerBase :
	public InterfaceImplBase <Poller <I>, I>,
	public PollerRoot,
	public ProxyBaseInterface <Bases>...,
	public ServantTraits <Poller <I> >,
	public LifeCycleRefCnt <Poller <I> >
{
public:
	// Wide interface
	template <class Base, class Derived>
	static Bridge <Base>* _wide_val (Bridge <I>* derived, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		return static_cast <ProxyBaseInterface <Base>&> (
			Poller <I>::_implementation (derived)).get_bridge (id, env);
	}

	template <>
	static Bridge <::Messaging::Poller>* _wide_val <::Messaging::Poller, I> (Bridge <I>* derived,
		Type <String>::ABI_in id, Interface* env) noexcept
	{
		return static_cast <PollerRoot&> (
			Poller <I>::_implementation (derived)).messaging_poller (id, env);
	}

	template <>
	static Bridge <::CORBA::Pollable>* _wide_val <::CORBA::Pollable, I> (Bridge <I>* derived,
		Type <String>::ABI_in id, Interface* env) noexcept
	{
		return static_cast <PollerRoot&> (
			ServantTraits <Poller <I> >::_implementation (derived)).pollable (id, env);
	}

	template <>
	static Bridge <::CORBA::ValueBase>* _wide_val <::CORBA::ValueBase, I> (Bridge <I>* derived, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		return static_cast <PollerRoot&> (
			ServantTraits <Poller <I> >::_implementation (derived)).value_base (id, env);
	}

protected:
	PollerBase (ValueBase::_ptr_type vb, Messaging::Poller::_ptr_type aggregate, UShort interface_idx) :
		PollerRoot (vb, aggregate, interface_idx),
		ProxyBaseInterface <Bases> (vb)...
	{}
};

}
}

#endif
