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

#include "TypeAlias.h"
#include "Proxy/TypeCodeImpl.h"

namespace CORBA {
namespace Internal {

template <const Alias* ref> class TypeDef;

template <const Alias* ref, class Content>
class TypeCodeTypeDef :
	public TypeCodeStatic <TypeDef <ref>, TypeCodeTK <TCKind::tk_alias>,
		TypeCodeOps <void> >,
	public TypeCodeContentType <Content>
{
public:
	static Type <String>::ABI_ret _s_id (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret_p (ref->id);
	}

	static Type <String>::ABI_ret _s_name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret_p (ref->name);
	}

	using TypeCodeContentType <Content>::_s_content_type;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equal (TCKind::tk_alias, ref->id, ref->name, content (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return content ()->equivalent (other);
	}

	static size_t _s_n_size (Bridge <TypeCode>*, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		return (b->_epv ().epv.n_size) (b, _env);
	}

	static size_t _s_n_align (Bridge <TypeCode>*, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		return (b->_epv ().epv.n_align) (b, _env);
	}

	static Type <Boolean>::ABI_ret _s_n_is_CDR (Bridge <TypeCode>*, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		return (b->_epv ().epv.n_is_CDR) (b, _env);
	}

	static void _s_n_construct (Bridge <TypeCode>*, void* p, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		(b->_epv ().epv.n_construct) (b, p, _env);
	}

	static void _s_n_destruct (Bridge <TypeCode>*, void* p, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		(b->_epv ().epv.n_destruct) (b, p, _env);
	}

	static void _s_n_copy (Bridge <TypeCode>*, void* dst, const void* src,
		Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		(b->_epv ().epv.n_copy) (b, dst, src, _env);
	}

	static void _s_n_move (Bridge <TypeCode>*, void* dst, void* src,
		Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		(b->_epv ().epv.n_move) (b, dst, src, _env);
	}

	static void _s_n_marshal_in (Bridge <TypeCode>*, const void* src, size_t size,
		Interface* call, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		(b->_epv ().epv.n_marshal_in) (b, src, size, call, _env);
	}

	static void _s_n_marshal_out (Bridge <TypeCode>*, void* src, size_t size,
		Interface* call, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		(b->_epv ().epv.n_marshal_out) (b, src, size, call, _env);
	}

	static void _s_n_unmarshal (Bridge <TypeCode>*, Interface* call, size_t size,
		void* dst, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		(b->_epv ().epv.n_unmarshal) (b, call, size, dst, _env);
	}

	static void _s_n_byteswap (Bridge <TypeCode>*, void* p, size_t size, Interface* _env)
	{
		Bridge <TypeCode>* b = content_bridge ();
		(b->_epv ().epv.n_byteswap) (b, p, size, _env);
	}

private:
	static I_ptr <TypeCode> content () NIRVANA_NOEXCEPT
	{
		return TypeCodeContentType <Content>::ptr ();
	}

	static Bridge <TypeCode>* content_bridge () NIRVANA_NOEXCEPT
	{
		return &static_cast <Bridge <TypeCode>&> (*&content ());
	}
};

template <const Alias* alias>
struct Type <TypeDef <alias> >
{
	static I_ptr <TypeCode> type_code ()
	{
		return alias->operator I_ptr <TypeCode> ();
	}
};

}
}

#endif
