#ifndef NIRVANA_ORB_OBJECT_S_H_
#define NIRVANA_ORB_OBJECT_S_H_

#include "Object_c.h"
#include "Interface_s.h"

namespace CORBA {
namespace Nirvana {

// Object skeleton

template <>
class FindInterface <Object>
{
public:
	template <class S>
	static Bridge <Interface>* find (S& servant, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <Object>::interface_id_, id))
			return Interface::_duplicate (&static_cast <Bridge <Object>&> (servant));
		else
			return nullptr;
	}
};

template <class S>
class Skeleton <S, Object>
{
public:
	static const typename Bridge <Object>::EPV epv_;

protected:
	static ClientBridge <ImplementationDef>* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
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

	static ClientBridge <InterfaceDef>* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
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

	static Boolean __is_equivalent (Bridge <Object>* obj, ClientBridge <Object>* other_object, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._is_equivalent (other_object);
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
		Bridge <Object>::interface_id_,
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

}
}

#endif