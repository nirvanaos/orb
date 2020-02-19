#ifndef NIRVANA_ORB_SERVANTBASE_S_H_
#define NIRVANA_ORB_SERVANTBASE_S_H_

#include "ServantBase.h"

namespace CORBA {
namespace Nirvana {

// ServantBase skeleton

template <class S>
class Skeleton <S, ServantBase>
{
public:
	static const typename Bridge <ServantBase>::EPV epv_;

protected:
	static Interface* __default_POA (Bridge <ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._default_POA ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Interface* __get_interface (Bridge <ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._get_interface ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Boolean __is_a (Bridge <ServantBase>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._is_a (type_id);
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Boolean __non_existent (Bridge <ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <ServantBase>::EPV Skeleton <S, ServantBase>::epv_ = {
	{ // interface
		ServantBase::check_interface_id_,
		S::template __duplicate <ServantBase>,
		S::template __release <ServantBase>
	},
	{ // base
		S::template _wide < ::CORBA::AbstractBase, ServantBase>,
		S::template _wide < ::CORBA::Object, ServantBase>
	},
	{ // epv
		S::__default_POA,
		S::__get_interface,
		S::__is_a,
		S::__non_existent
	}
};

}
}

#endif
