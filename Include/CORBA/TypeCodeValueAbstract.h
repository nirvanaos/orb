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
#ifndef NIRVANA_ORB_TYPECODEVALUEABSTRACT_H_
#define NIRVANA_ORB_TYPECODEVALUEABSTRACT_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Internal {

template <class Itf>
class TypeCodeValueAbstract :
	public TypeCodeStatic <TypeCodeValueAbstract <Itf>,
		TypeCodeTK <TCKind::tk_value>, TypeCodeOps <Itf> >,
	public TypeCodeORB,
	public TypeCodeValueAbstractBase,
	public TypeCodeStaticIdName
{
public:
	using TypeCodeORB::_s_equal;
	using TypeCodeORB::_s_equivalent;
	using TypeCodeORB::_s_get_compact_typecode;
	using TypeCodeValueAbstractBase::_s_member_count;
	using TypeCodeValueAbstractBase::_s_member_name;
	using TypeCodeValueAbstractBase::_s_member_type;
	using TypeCodeValueAbstractBase::_s_member_visibility;
	using TypeCodeValueAbstractBase::_s_type_modifier;
	using TypeCodeValueAbstractBase::_s_concrete_base_type;
	using TypeCodeStaticIdName::_s_id;
	using TypeCodeStaticIdName::_s_name;
};

}
}

#endif
