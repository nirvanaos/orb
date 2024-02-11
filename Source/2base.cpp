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
#include "../../pch/pch.h"

namespace CORBA {
namespace Internal {

struct EPV_Header
{
	Internal::Interface::EPV header;
	struct
	{
		Internal::Interface* (*to_base) (Internal::Interface*, const Internal::ABI <IDL::String>*, Internal::Interface*);
	}
	base;
};

Object::_ptr_type interface2object (Interface::_ptr_type itf)
{
	assert (itf);
	Object::_ptr_type obj;
	const StringView <Char> interface_id = itf->_epv ().interface_id;
	if (RepId::compatible (interface_id, RepIdOf <Object>::id))
		obj = (Object*)&itf;
	else {
		Environment env;
		Interface* p = ((const EPV_Header&)itf->_epv ()).base.to_base (&itf,
			&Type <IDL::String>::C_in (RepIdOf <Object>::id), &env);
		env.check ();
		obj = Object::_check (p);
	}
	return obj;
}

ValueBase::_ptr_type value_type2base (Interface::_ptr_type val)
{
	assert (val);
	ValueBase::_ptr_type base;
	const StringView <Char> interface_id = val->_epv ().interface_id;
	if (RepId::compatible (interface_id, RepIdOf <ValueBase>::id))
		base = (ValueBase*)&val;
	else {
		Environment env;
		Interface* p = ((const EPV_Header&)val->_epv ()).base.to_base (&val,
			&Type <IDL::String>::C_in (RepIdOf <ValueBase>::id), &env);
		env.check ();
		base = ValueBase::_check (p);
	}
	return base;
}

AbstractBase::_ptr_type abstract_interface2base (Interface::_ptr_type itf)
{
	assert (itf);
	AbstractBase::_ptr_type base;
	const StringView <Char> interface_id = itf->_epv ().interface_id;
	if (RepId::compatible (interface_id, RepIdOf <ValueBase>::id))
		base = (AbstractBase*)&itf;
	else {
		Environment env;
		Interface* p = ((const EPV_Header&)itf->_epv ()).base.to_base (&itf,
			&Type <IDL::String>::C_in (RepIdOf <AbstractBase>::id), &env);
		env.check ();
		base = AbstractBase::_check (p);
	}
	return base;
}

}
}
