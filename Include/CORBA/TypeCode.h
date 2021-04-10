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
#ifndef NIRVANA_ORB_TYPECODE_H_
#define NIRVANA_ORB_TYPECODE_H_

#include "Any.h"
#include "Boolean.h"

// TODO: Remove redundant includes
#include "String.h"
#include "Client.h"
#include "UserException.h"
#include "EnvironmentEx.h"
#include "TCKind.h"

namespace CORBA {

class Any;

typedef Short Visibility;
const Visibility PRIVATE_MEMBER = 0;
const Visibility PUBLIC_MEMBER = 1;

typedef Short ValueModifier;
const ValueModifier VM_NONE = 0;
const ValueModifier VM_CUSTOM = 1;
const ValueModifier VM_ABSTRACT = 2;
const ValueModifier VM_TRUNCATABLE = 3;

namespace Nirvana {

class Marshal;
typedef I_ptr <Marshal> Marshal_ptr;

class Unmarshal;
typedef I_ptr <Unmarshal> Unmarshal_ptr;

template <>
struct Definitions <TypeCode>
{
	static const ::Nirvana::ImportInterfaceT <TypeCode> _tc_BadKind;

	class BadKind : public UserException
	{
	public:
		DECLARE_EXCEPTION (BadKind);
	};

	static const ::Nirvana::ImportInterfaceT <TypeCode> _tc_Bounds;

	class Bounds : public UserException
	{
	public:
		DECLARE_EXCEPTION (Bounds);
	};
};

template <>
const Char RepIdOf <Definitions <TypeCode>::BadKind>::repository_id_ [] = CORBA_REPOSITORY_ID ("TypeCode/BadKind");

template <>
const Char RepIdOf <Definitions <TypeCode>::Bounds>::repository_id_ [] = CORBA_REPOSITORY_ID ("TypeCode/Bounds");

BRIDGE_BEGIN (TypeCode, CORBA_REPOSITORY_ID ("TypeCode"))
ABI_boolean (*equal) (Bridge <TypeCode>*, Interface*, Interface*);
ABI_boolean (*equivalent) (Bridge <TypeCode>*, Interface*, Interface*);
Interface* (*get_compact_typecode) (Bridge <TypeCode>*, Interface*);
ABI_enum (*kind) (Bridge <TypeCode>*, Interface*);
Type <String>::ABI_ret (*id) (Bridge <TypeCode>*, Interface*);
Type <String>::ABI_ret (*name) (Bridge <TypeCode>*, Interface*);
ULong (*member_count) (Bridge <TypeCode>*, Interface*);
Type <String>::ABI_ret (*member_name) (Bridge <TypeCode>*, ULong index, Interface*);
Interface* (*member_type) (Bridge <TypeCode>*, ULong index, Interface*);
const Any* (*member_label) (Bridge <TypeCode>*, ULong index, Interface*);
Interface* (*discriminator_type) (Bridge <TypeCode>*, Interface*);
Long (*default_index) (Bridge <TypeCode>*, Interface*);
ULong (*length) (Bridge <TypeCode>*, Interface*);
Interface* (*content_type) (Bridge <TypeCode>*, Interface*);
UShort (*fixed_digits) (Bridge <TypeCode>*, Interface*);
Short (*fixed_scale) (Bridge <TypeCode>*, Interface*);
Visibility (*member_visibility) (Bridge <TypeCode>*, ULong index, Interface*);
ValueModifier (*type_modifier) (Bridge <TypeCode>*, Interface*);
Interface* (*concrete_base_type) (Bridge <TypeCode>*, Interface*);

::Nirvana::Size (*_size) (Bridge <TypeCode>*, Interface*);
void (*_construct) (Bridge <TypeCode>*, ::Nirvana::Pointer, Interface*);
void (*_destruct) (Bridge <TypeCode>*, ::Nirvana::Pointer, Interface*);
void (*_copy) (Bridge <TypeCode>*, ::Nirvana::Pointer, ::Nirvana::ConstPointer, Interface*);
void (*_move) (Bridge <TypeCode>*, ::Nirvana::Pointer, ::Nirvana::Pointer, Interface*);
ABI_boolean (*_has_marshal) (Bridge <TypeCode>*, Interface*);
void (*_marshal_in) (Bridge <TypeCode>*, ::Nirvana::ConstPointer, Interface*, ::Nirvana::Pointer, Interface*);
void (*_marshal_out) (Bridge <TypeCode>*, ::Nirvana::Pointer, Interface*, ::Nirvana::Pointer, Interface*);
void (*_unmarshal) (Bridge <TypeCode>*, ::Nirvana::ConstPointer, Interface*, ::Nirvana::Pointer, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, TypeCode> :
	public T,
	public Definitions <TypeCode>
{
public:
	Boolean equal (TypeCode_ptr other);
	Boolean equivalent (TypeCode_ptr other);
	TypeCode_var get_compact_typecode ();
	TCKind kind ();

	// for tk_objref, tk_struct, tk_union, tk_enum, tk_alias,
	// tk_value, tk_value_box, tk_native, tk_abstract_interface
	// tk_local_interface and tk_except
	String id (); // raises (BadKind);
	String name (); // raises (BadKind);

	// for tk_struct, tk_union, tk_enum, tk_value,
	// and tk_except
	ULong member_count (); // raises (BadKind);
	String member_name (ULong index); // raises (BadKind, Bounds);
	TypeCode_var member_type (ULong index); // raises (BadKind, Bounds);

	// for tk_union
	const Any& member_label (ULong index); // raises (BadKind, Bounds);
	TypeCode_var discriminator_type (); // raises (BadKind);
	Long default_index (); // raises (BadKind);

	// for tk_string, tk_sequence, and tk_array
	ULong length (); // raises (BadKind);

	// for tk_sequence, tk_array, tk_value_box and tk_alias
	TypeCode_var content_type (); // raises (BadKind);

	// for tk_fixed
	UShort fixed_digits (); // raises (BadKind);
	Short fixed_scale (); // raises (BadKind);

	// for tk_value
	Visibility member_visibility (ULong index); // raises (BadKind, Bounds);
	ValueModifier type_modifier (); // raises (BadKind);
	TypeCode_var concrete_base_type (); // raises (BadKind);

	// Nirvana extensions
	// NOTE: For exceptions, all methods below are related to the exception data, not the exception itself.

	// Size of the object.
	::Nirvana::Size _size ();

	// Call the default constructor.
	void _construct (::Nirvana::Pointer p);

	// Destroy the object.
	void _destruct (::Nirvana::Pointer p);

	// Call copy constructor.
	void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src);

	// Call move constructor. Fallbacks to copy constructor if no move constructor exists.
	void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src);

	// If returns `false` then `marshaler` and `unmarshaler` parameters may be `nil`.
	Boolean _has_marshal ();

	void _marshal_in (::Nirvana::ConstPointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst);
	void _marshal_out (::Nirvana::Pointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst);
	void _unmarshal (::Nirvana::ConstPointer src, Unmarshal_ptr unmarshaler, ::Nirvana::Pointer dst);
};

template <class T>
Boolean Client <T, TypeCode>::equal (TypeCode_ptr other)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.equal) (&_b, &other, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, TypeCode>::equivalent (TypeCode_ptr other)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.equivalent) (&_b, &other, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::get_compact_typecode ()
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.get_compact_typecode) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TCKind Client <T, TypeCode>::kind ()
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Type <TCKind>::C_ret _ret = (_b._epv ().epv.kind) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
String Client <T, TypeCode>::id ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret = (_b._epv ().epv.id) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
String Client <T, TypeCode>::name ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret = (_b._epv ().epv.name) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, TypeCode>::member_count ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.member_count) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
String Client <T, TypeCode>::member_name (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret = (_b._epv ().epv.member_name) (&_b, index, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::member_type (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.member_type) (&_b, index, &_env);
	_env.check ();
	return _ret;
}

template <class T>
const Any& Client <T, TypeCode>::member_label (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	typename Type <Any>::C_VT_ret _ret = (_b._epv ().epv.member_label) (&_b, index, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::discriminator_type ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.discriminator_type) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Long Client <T, TypeCode>::default_index ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.default_index) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, TypeCode>::length ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.length) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::content_type ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.content_type) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
UShort Client <T, TypeCode>::fixed_digits ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	UShort _ret = (_b._epv ().epv.fixed_digits) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Short Client <T, TypeCode>::fixed_scale ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Short _ret = (_b._epv ().epv.fixed_scale) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Visibility Client <T, TypeCode>::member_visibility (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Visibility _ret = (_b._epv ().epv.member_visibility) (&_b, index, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ValueModifier Client <T, TypeCode>::type_modifier ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	ValueModifier _ret = (_b._epv ().epv.type_modifier) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::concrete_base_type ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.concrete_base_type) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::Size Client <T, TypeCode>::_size ()
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	::Nirvana::Size _ret = (_b._epv ().epv._size) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, TypeCode>::_construct (::Nirvana::Pointer p)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._construct) (&_b, p, &_env);
	_env.check ();
}

template <class T>
void Client <T, TypeCode>::_destruct (::Nirvana::Pointer p)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._destruct) (&_b, p, &_env);
	_env.check ();
}

template <class T>
void Client <T, TypeCode>::_copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._copy) (&_b, dst, src, &_env);
	_env.check ();
}

template <class T>
void Client <T, TypeCode>::_move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._move) (&_b, dst, src, &_env);
	_env.check ();
}

template <class T>
Boolean Client <T, TypeCode>::_has_marshal ()
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret ((_b._epv ().epv._has_marshal) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, TypeCode>::_marshal_in (::Nirvana::ConstPointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._marshal_in) (&_b, src, &marshaler, dst, &_env);
	_env.check ();
}

template <class T>
void Client <T, TypeCode>::_marshal_out (::Nirvana::Pointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._marshal_out) (&_b, src, &marshaler, dst, &_env);
	_env.check ();
}

template <class T>
void Client <T, TypeCode>::_unmarshal (::Nirvana::ConstPointer src, Unmarshal_ptr unmarshaler, ::Nirvana::Pointer dst)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._unmarshal) (&_b, src, &unmarshaler, dst, &_env);
	_env.check ();
}

template <>
struct Type <I_var <TypeCode> > : TypeItf <TypeCode>
{
	static TypeCode_ptr type_code ()
	{
		return _tc_TypeCode;
	}
};

}

class TypeCode : public Nirvana::ClientInterface <TypeCode>
{
public:
	using Nirvana::Definitions <TypeCode>::_tc_BadKind;
	using Nirvana::Definitions <TypeCode>::BadKind;

	using Nirvana::Definitions <TypeCode>::_tc_Bounds;
	using Nirvana::Definitions <TypeCode>::Bounds;
};

}

#endif
