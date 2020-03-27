#ifndef NIRVANA_ORB_TYPECODE_H_
#define NIRVANA_ORB_TYPECODE_H_

#include "TypeCode_forward.h"
#include "Any.h"

namespace CORBA {
namespace Nirvana {

template <> struct Type <TCKind> : TypeEnum <TCKind, tk_local_interface>
{};

template <>
const Char RepIdOf <TCKind>::repository_id_ [] = CORBA_REPOSITORY_ID ("TCKind");

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
const char* Client <T, TypeCode>::id ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	const char* _ret = (_b._epv ().epv.id) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
const char* Client <T, TypeCode>::name ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	const char* _ret = (_b._epv ().epv.name) (&_b, &_env);
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
const char* Client <T, TypeCode>::member_name (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	const char* _ret = (_b._epv ().epv.member_name) (&_b, index, &_env);
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
ULong Client <T, TypeCode>::_size ()
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv._size) (&_b, &_env);
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
void Client <T, TypeCode>::_unmarshal (::Nirvana::Pointer src, Unmarshal_ptr unmarshaler, ::Nirvana::Pointer dst)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._unmarshal) (&_b, src, &unmarshaler, dst, &_env);
	_env.check ();
}

}
}

#endif
