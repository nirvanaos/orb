#ifndef NIRVANA_ORB_TYPECODE_S_H_
#define NIRVANA_ORB_TYPECODE_S_H_

#include "TypeCode_c.h"
#include "Servant.h"

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
			return S::_implementation (_b).equal (_unmarshal_in (other));
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean _equivalent (Bridge <TypeCode>* _b, BridgeMarshal <TypeCode>* other, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).equivalent (_unmarshal_in (other));
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static BridgeMarshal <TypeCode>* _get_compact_typecode (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).get_compact_typecode ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static TCKind kind (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).kind ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return tk_null;
	}

	static const char* _id (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).id ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static const char* _name (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).name ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static ULong _member_count (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).member_count ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static const char* _member_name (Bridge <TypeCode>*, ULong index, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).member_name (index);
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static TypeCode _member_type (Bridge <TypeCode>*, ULong index, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).member_type (index);
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Any* _member_label (Bridge <TypeCode>*, ULong index, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).member_label (index);
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static TypeCode _discriminator_type (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).discriminator_type ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Long _default_index (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).default_index ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static ULong _length (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).length ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static TypeCode _content_type (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).content_type ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static UShort _fixed_digits (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).fixed_digits ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Short _fixed_scale (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).fixed_scale ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Visibility _member_visibility (Bridge <TypeCode>*, ULong index, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).member_visibility ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static ValueModifier _type_modifier (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).type_modifier ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static TypeCode _concrete_base_type (Bridge <TypeCode>*, EnvironmentBridge*)
	{
		try {
			return S::_implementation (_b).type_modifier ();
		} catch (const Exception & e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
class Servant <S, TypeCode> :
	public ImplementationPseudo <S, TypeCode>
{};

template <class S>
class ServantStatic <S, TypeCode> :
	public ImplementationStaticPseudo <S, TypeCode>
{};

}
}

#endif
