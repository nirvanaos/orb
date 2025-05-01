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
#ifndef NIRVANA_ORB_TYPECODEVALUEBOX_H_
#define NIRVANA_ORB_TYPECODEVALUEBOX_H_

#include "../TypeCodeImpl.h"

namespace CORBA {
namespace Internal {

template <class Itf>
class TypeCodeValueBox : 
	public TypeCodeStatic <TypeCodeValueBox <Itf>,
	TypeCodeWithId <TCKind::tk_value_box, Itf>, TypeCodeOps <Itf> >,
	public TypeCodeName <Itf>,
	public TypeCodeContentType <typename Itf::BoxedType>
{
	typedef TypeCodeStatic <TypeCodeValueBox <Itf>,
		TypeCodeWithId <TCKind::tk_value_box, Itf>, TypeCodeOps <Itf> > Base;
	typedef TypeCodeContentType <typename Itf::BoxedType> Content;
	typedef TypeCodeName <Itf> Name;
public:
	using Name::_s_name;
	using Content::_s_content_type;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equal (TCKind::tk_value_box, Base::RepositoryType::id, Name::name_, content (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equivalent (TCKind::tk_value_box, Base::RepositoryType::id, content (), other);
	}

	static Type <CORBA::TypeCode>::VRet get_compact_typecode ()
	{
		return the_orb->create_value_box_tc (Base::RepositoryType::id, IDL::String (), content ());
	}

private:
	static I_ptr <TypeCode> content () noexcept
	{
		return Content::ptr ();
	}
};

}
}

#endif
