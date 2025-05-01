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
template <class Itf>
class ProxyLink
{
public:
	ProxyLink (I_ptr <Itf> p) :
		itf_ (p)
	{
		if (!p)
			::Nirvana::throw_INV_OBJREF ();
		id_len_ = strlen (itf_->_epv ().header.interface_id);
	}

	Bridge <Itf>* get_bridge (Type <String>::ABI_in id, Interface* env) const noexcept
	{
		if (!RepId::compatible (itf_->_epv ().header.interface_id, id_len_, Type <String>::in (id)))
			set_INV_OBJREF (env);
		return static_cast <Bridge <Itf>*> (&itf_);
	}

	I_ptr <Itf> itf () const noexcept
	{
		return itf_;
	}

private:
	I_ptr <Itf> itf_;
	size_t id_len_;
};

template <class Itf>
class ProxyBaseInterface : public ProxyLink <Itf>
{
	typedef ProxyLink <Itf> Base;

protected:
	ProxyBaseInterface (Object::_ptr_type obj) :
		Base (obj->_query_interface <Itf> ())
	{}

	ProxyBaseInterface (ValueBase::_ptr_type vb) :
		Base (vb->_query_valuetype <Itf> ())
	{}
};

}
}

#endif
