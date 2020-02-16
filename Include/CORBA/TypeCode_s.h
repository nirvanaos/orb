#ifndef NIRVANA_ORB_TYPECODE_S_H_
#define NIRVANA_ORB_TYPECODE_S_H_

#include "TypeCode_b.h"
#include "ServantStatic.h"
#include "TypeInterface.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, TypeCode>
{
public:
	static const typename Bridge <TypeCode>::EPV epv_;

protected:
	static Boolean _equal (Bridge <TypeCode>* _b, BridgeMarshal <TypeCode>* other, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).equal (TypeI <TypeCode>::in (other));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _equivalent (Bridge <TypeCode>* _b, BridgeMarshal <TypeCode>* other, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).equivalent (TypeI <TypeCode>::in (other));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static BridgeMarshal <TypeCode>* _get_compact_typecode (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).get_compact_typecode ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static TCKind _kind (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).kind ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return tk_null;
	}

	static const char* _id (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).id ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).name ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ULong _member_count (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).member_count ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).member_name (index);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static BridgeMarshal <TypeCode>* _member_type (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).member_type (index);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Any* _member_label (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).member_label (index);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static BridgeMarshal <TypeCode>* _discriminator_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).discriminator_type ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Long _default_index (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).default_index ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ULong _length (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).length ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static BridgeMarshal <TypeCode>* _content_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).content_type ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static UShort _fixed_digits (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).fixed_digits ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Short _fixed_scale (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).fixed_scale ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Visibility _member_visibility (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).member_visibility (index);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ValueModifier _type_modifier (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).type_modifier ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static BridgeMarshal <TypeCode>* _concrete_base_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).concrete_base_type ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ULong __size (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b)._size ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void __construct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b)._construct (p);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __destruct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b)._destruct (p);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __copy (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::ConstPointer src, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b)._copy (dst, src);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __move (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b)._move (dst, src);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __local_marshal (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, ::Nirvana::Pointer dst, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b)._local_marshal (src, dst);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __local_unmarshal_in (Bridge <TypeCode>* _b, ::Nirvana::Pointer val, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b)._local_unmarshal_in (val);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __local_unmarshal_inout (Bridge <TypeCode>* _b, ::Nirvana::Pointer val, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b)._local_unmarshal_inout (val);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge <TypeCode>::EPV Skeleton <S, TypeCode>::epv_ = {
	{ // interface
		Bridge <TypeCode>::interface_id_,
		S::template __duplicate <TypeCode>,
		S::template __release <TypeCode>
	},
	{ // epv
		S::_equal,
		S::_equivalent,
		S::_get_compact_typecode,
		S::_kind,
		S::_id,
		S::_name,
		S::_member_count,
		S::_member_name,
		S::_member_type,
		S::_member_label,
		S::_discriminator_type,
		S::_default_index,
		S::_length,
		S::_content_type,
		S::_fixed_digits,
		S::_fixed_scale,
		S::_member_visibility,
		S::_type_modifier,
		S::_concrete_base_type,
		S::__size,
		S::__construct,
		S::__destruct,
		S::__copy,
		S::__move,
		S::__local_marshal,
		S::__local_unmarshal_in,
		S::__local_unmarshal_inout
	}
};

template <class S>
class ServantStatic <S, TypeCode> :
	public ImplementationStaticPseudo <S, TypeCode>
{};

}
}

#endif
