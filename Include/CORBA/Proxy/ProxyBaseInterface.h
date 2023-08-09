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
#ifndef NIRVANA_ORB_PROXYBASEINTERFACE_H_
#define NIRVANA_ORB_PROXYBASEINTERFACE_H_

#include "../CORBA.h"

namespace CORBA {
namespace Internal {

// Link to interface in other proxy object
template <class I>
class ProxyLink : public I_ptr <I>
{
public:
	ProxyLink (I_ptr <I> p) :
		I_ptr <I> (p)
	{
		if (!I_ptr <I>::p_)
			::Nirvana::throw_INV_OBJREF ();
		id_len_ = strlen (I_ptr <I>::p_->_epv ().header.interface_id);
	}

	Bridge <I>* get_bridge (Type <String>::ABI_in id, Interface* env) const noexcept
	{
		if (!RepId::compatible (I_ptr <I>::p_->_epv ().header.interface_id, id_len_, Type <String>::in (id)))
			set_INV_OBJREF (env);
		return I_ptr <I>::p_;
	}

private:
	size_t id_len_;
};

template <class I>
class ProxyBaseInterface : public ProxyLink <I>
{
	typedef ProxyLink <I> Base;

protected:
	ProxyBaseInterface (Object::_ptr_type obj) :
		Base (obj->_query_interface <I> ())
	{}

	ProxyBaseInterface (ValueBase::_ptr_type vb) :
		Base (vb->_query_valuetype <I> ())
	{}
};

}
}

#endif
