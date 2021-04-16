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

#include <Nirvana/OLF.h>
#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

template <const ::Nirvana::ExportInterface* ref> class TypeCodeTypeDef;

template <const ::Nirvana::ExportInterface* ref, class Content>
class TypeCodeTypeDefImpl :
	public TypeCodeStatic <TypeCodeTypeDef <ref>, TypeCodeTK <tk_alias>, TypeCodeOps <void> >,
	public TypeCodeContentType <Content>,
	public TypeCodeName < TypeCodeTypeDef <ref> >
{
	typedef TypeCodeStatic <TypeCodeTypeDef <ref>, TypeCodeTK <tk_alias>, TypeCodeOps <void> > Base;
public:
	using TypeCodeContentType <Content>::_content_type;
	typedef TypeCodeName <TypeCodeTypeDef <ref> > Name;
	using Name::_name;

	static Boolean equal (TypeCode_ptr other)
	{
		return Base::equal (other)
			&& Name::equal (other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return content ()->equivalent (other);
	}

	static ABI <String> _id (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret_p (ref->name);
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
