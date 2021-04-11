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
#ifndef NIRVANA_ORB_TYPECODETYPEDEF_H_
#define NIRVANA_ORB_TYPECODETYPEDEF_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

template <class TC, class Content>
class TypeCodeTypeDef :
	public TypeCodeStatic <TypeCodeTypeDef <TC, Content>, TypeCodeWithId <tk_alias, RepIdOf <TC> >, TypeCodeOps <void> >,
	public TypeCodeContentType <Content>,
	public TypeCodeName <TC>
{
public:
	using TypeCodeContentType <Content>::_content_type;
	using TypeCodeName <TC>::_name;

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeBase::dereference_alias (TypeCodeTypeDef <TC, Content>::_get_ptr ())->equivalent (other);
	}

	static size_t __size (Bridge <TypeCode>* _b, Interface* _env)
	{
		return (content ()->_epv ().epv._size) (_b, _env);
	}

	static void __construct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, Interface* _env)
	{
		(content ()->_epv ().epv._construct) (_b, p, _env);
	}

	static void __destruct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, Interface* _env)
	{
		(content ()->_epv ().epv._destruct) (_b, p, _env);
	}

	static void __copy (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::ConstPointer src, Interface* _env)
	{
		(content ()->_epv ().epv._copy) (_b, dst, src, _env);
	}

	static void __move (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, Interface* _env)
	{
		(content ()->_epv ().epv._move) (_b, dst, src, _env);
	}

	static ABI_boolean __has_marshal (Bridge <TypeCode>* _b, Interface* _env)
	{
		return (content ()->_epv ().epv._has_marshal) (_b, _env);
	}

	static void __marshal_in (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* marshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		(content ()->_epv ().epv._marshal_in) (_b, src, marshaler, dst, _env);
	}

	static void __marshal_out (Bridge <TypeCode>* _b, ::Nirvana::Pointer src, Interface* marshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		(content ()->_epv ().epv._marshal_out) (_b, src, marshaler, dst, _env);
	}

	static void __unmarshal (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* unmarshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		(content ()->_epv ().epv._unmarshal) (_b, src, unmarshaler, dst, _env);
	}

private:
	static TypeCode_ptr content ()
	{
		return TypeCodeContentType <Content>::ptr ();
	}
};

}
}

#endif
