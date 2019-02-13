#ifndef NIRVANA_ORB_OBJECT_S_H_
#define NIRVANA_ORB_OBJECT_S_H_

#include "Object_c.h"
#include "Interface_s.h"

namespace CORBA {
namespace Nirvana {

// Object skeleton
template <class S>
class Skeleton <S, Object>
{
public:
	static const typename Bridge <Object>::EPV epv_;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <Object>::interface_id_, id))
			return &static_cast <Bridge <Object>&> (base);
		else
			return nullptr;
	}

protected:
	static Bridge <ImplementationDef>* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._get_implementation ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Bridge <InterfaceDef>* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._get_interface ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._is_a (type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __is_equivalent (Bridge <Object>* obj, Bridge <Object>* other_object, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._is_equivalent ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static ULong __hash (Bridge <Object>* obj, ULong maximum, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._hash (maximum);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <Object>::EPV Skeleton <S, Object>::epv_ = {
	{ // interface
		S::template __duplicate <Object>,
		S::template __release <Object>
	},
	{ // base
		S::template _wide <AbstractBase, Object>
	},
	{ // epv
		S::__get_implementation,
		S::__get_interface,
		S::__is_a,
		S::__non_existent,
		S::__is_equivalent,
		S::__hash
	}
};

// POA implementation
/*
template <>
class ServantPOA < ::CORBA::Object> :
	public ImplementationPOA < ::CORBA::Object>
{
public:
	virtual POA_ptr _default_POA ()
	{
		return ObjectBase::_default_POA ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return ObjectBase::_get_interface (_primary_interface ());
	}

	virtual Boolean _non_existent ()
	{
		return ObjectBase::_non_existent ();
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		return Skeleton <ServantPOA < ::CORBA::Object>, ::CORBA::Object>::_is_a (type_id);
	}
};
/*
// Tied implementation

template <class S>
class InterfaceTied <S, Object> :
	public InterfaceTiedBase <Object>,
	public Skeleton <S, Object>
{
public:
	Boolean _non_existent () const
	{
		return !static_cast <const S&> (*this)._tied_object ();
	}

	POA_ptr _default_POA () const
	{
		return 0;	// TODO: Implement.
	}

	void _default_POA (POA_ptr);	// TODO: Implement.

protected:
	InterfaceTied <S, Object> () :
		InterfaceTiedBase <Object> (Skeleton <S, Object>::epv_),
		poa_ (0)
	{}

	POA_ptr poa_;
};
*/

}
}

#endif
