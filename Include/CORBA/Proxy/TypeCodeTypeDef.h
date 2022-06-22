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
namespace Internal {

template <const ::Nirvana::ExportInterface* ref> class TypeCodeTypeDef;

template <const ::Nirvana::ExportInterface* ref, class Content>
class TypeCodeTypeDefImpl :
	public TypeCodeStatic <TypeCodeTypeDef <ref>, TypeCodeTK <TCKind::tk_alias>,
		TypeCodeOps <void> >,
	public TypeCodeContentType <Content>,
	public TypeCodeName < TypeCodeTypeDef <ref> >
{
	typedef TypeCodeStatic <TypeCodeTypeDef <ref>, TypeCodeTK <TCKind::tk_alias>,
		TypeCodeOps <void> > Base;
public:
	using TypeCodeContentType <Content>::_s_content_type;
	typedef TypeCodeName <TypeCodeTypeDef <ref> > Name;
	using Name::_s_name;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equal (TCKind::tk_alias, ref->name, Name::name_, content (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return content ()->equivalent (other);
	}

	static ABI <String> _s_id (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret_p (ref->name);
	}

	static size_t _s_n_size (Bridge <TypeCode>* _b, Interface* _env)
	{
		return (content ()->_epv ().epv.n_size) (_b, _env);
	}

	static Type <Boolean>::ABI_ret _s_n_fixed_len (Bridge <TypeCode>* _b,
		Interface* _env)
	{
		return (content ()->_epv ().epv.n_fixed_len) (_b, _env);
	}

	static void _s_n_construct (Bridge <TypeCode>* _b, void* p, Interface* _env)
	{
		(content ()->_epv ().epv.n_construct) (_b, p, _env);
	}

	static void _s_n_destruct (Bridge <TypeCode>* _b, void* p, Interface* _env)
	{
		(content ()->_epv ().epv.n_destruct) (_b, p, _env);
	}

	static void _s_n_copy (Bridge <TypeCode>* _b, void* dst, const void* src,
		Interface* _env)
	{
		(content ()->_epv ().epv.n_copy) (_b, dst, src, _env);
	}

	static void _s_n_move (Bridge <TypeCode>* _b, void* dst, void* src,
		Interface* _env)
	{
		(content ()->_epv ().epv.n_move) (_b, dst, src, _env);
	}

	static void _s_n_marshal_in (Bridge <TypeCode>* _b, const void* src, size_t size,
		Interface* call, Interface* _env)
	{
		(content ()->_epv ().epv.n_marshal_in) (_b, src, size, call, _env);
	}

	static void _s_n_marshal_out (Bridge <TypeCode>* _b, void* src, size_t size,
		Interface* call, Interface* _env)
	{
		(content ()->_epv ().epv.n_marshal_out) (_b, src, size, call, _env);
	}

	static void _s_n_unmarshal (Bridge <TypeCode>* _b, Interface* call, size_t size,
		void* dst, Interface* _env)
	{
		(content ()->_epv ().epv.n_unmarshal) (_b, call, size, dst, _env);
	}

private:
	static I_ptr <TypeCode> content () NIRVANA_NOEXCEPT
	{
		return TypeCodeContentType <Content>::ptr ();
	}
};

}
}

#endif
