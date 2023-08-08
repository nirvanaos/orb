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

	IOReference::OperationIndex _make_op_idx (UShort op_idx) const
	{
		return IOReference::OperationIndex (interface_idx_, op_idx);
	}

protected:
	PollerRoot (Messaging::Poller::_ptr_type aggregate, UShort interface_idx) :
		aggregate_ (aggregate),
		interface_idx_ (interface_idx)
	{}

private:
	Messaging::Poller::_ptr_type aggregate_;
	UShort interface_idx_;
};

template <class I>
class PollerBase
{
public:
	void init (Messaging::Poller::_ptr_type aggregate)
	{
		val_ = static_cast <Bridge <I>*> (&aggregate->_query_valuetype (RepIdOf <I>::id));
		if (!val_)
			throw OBJ_ADAPTER ();
	}

	Bridge <I>* get () const noexcept
	{
		return val_;
	}

private:
	Bridge <I>* val_;
};



}
}

#endif
