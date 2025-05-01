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

#include "ProxyHolder.h"
#include "ProxyBaseInterface.h"
#include "../ServantImpl.h"
#include "../LifeCycleRefCnt.h"
#include "../ExceptionSet.h"
#include "IOReference.h"
#include "OperationIndex.h"

namespace CORBA {
namespace Internal {

template <class Itf> class Poller;

class PollerRoot : public ValueMemory
{
public:
	void _add_ref ()
	{
		interface_duplicate (&aggregate_.itf ());
	}

	void _remove_ref () noexcept
	{
		interface_release (&aggregate_.itf ());
	}

	OperationIndex _make_op_idx (UShort op_idx) const noexcept
	{
		return make_op_idx (interface_idx_, op_idx);
	}

	Bridge <Messaging::Poller>* messaging_poller (Type <String>::ABI_in id, Interface* env) const noexcept;
	Bridge <Pollable>* pollable (Type <String>::ABI_in id, Interface* env) const noexcept;
	Bridge <ValueBase>* value_base (Type <String>::ABI_in id, Interface* env) const noexcept;

	template <class ... Ex>
	IORequest::_ref_type _get_reply (uint32_t timeout, UShort op_idx) const
	{
		IORequest::_ref_type rq = aggregate_.itf ()->n_get_reply (timeout, _make_op_idx (op_idx));
		check_request (rq, ExceptionSet <Ex...>::entries (), sizeof... (Ex));
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

template <class Itf, class ... Bases>
class PollerBase :
	public InterfaceImplBase <Poller <Itf>, Itf>,
	public ProxyHolderImpl <Poller <Itf> >,
	public PollerRoot,
	public ProxyBaseInterface <Bases>...,
	public ServantTraits <Poller <Itf> >,
	public LifeCycleRefCnt <Poller <Itf> >
{
public:
	// Wide interface
	template <class Base, class Derived>
	static Bridge <Base>* _wide_val (Bridge <Derived>* derived, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		Bridge <Base>* ret;
		_wide_val (derived, ret, id, env);
		return ret;
	}

protected:
	PollerBase (ValueBase::_ptr_type vb, Messaging::Poller::_ptr_type aggregate, UShort interface_idx) :
		PollerRoot (vb, aggregate, interface_idx),
		ProxyBaseInterface <Bases> (vb)...
	{}

private:
	template <class Base, class Derived>
	static void _wide_val (Bridge <Derived>* derived, Bridge <Base>*& base, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		base = static_cast <ProxyBaseInterface <Base>&> (
			Poller <Itf>::_implementation (derived)).get_bridge (id, env);
	}

	static void _wide_val (Bridge <Itf>* derived, Bridge <::Messaging::Poller>*& base, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		base = static_cast <PollerRoot&> (
			Poller <Itf>::_implementation (derived)).messaging_poller (id, env);
	}

	static void _wide_val (Bridge <Itf>* derived, Bridge <::CORBA::Pollable>*& base, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		base = static_cast <PollerRoot&> (
			ServantTraits <Poller <Itf> >::_implementation (derived)).pollable (id, env);
	}

	static void _wide_val (Bridge <Itf>* derived, Bridge <::CORBA::ValueBase>*& base, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		base = static_cast <PollerRoot&> (
			ServantTraits <Poller <Itf> >::_implementation (derived)).value_base (id, env);
	}

};

}
}

#endif
