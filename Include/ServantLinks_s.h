#ifndef NIRVANA_ORB_SERVANTLINKS_S_H_
#define NIRVANA_ORB_SERVANTLINKS_S_H_

#include "ServantLinks_c.h"
#include "Interface_s.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ServantLinks>
{
public:
	static const typename Bridge <ServantLinks>::EPV epv_;

protected:
	static Bridge <ServantBase>* _servant_base (Bridge <ServantLinks>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).servant_base ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Bridge <ServantBase>* _object (Bridge <ServantLinks>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).object ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean _is_active (Bridge <ServantLinks>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).is_active ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <ServantLinks>::EPV Skeleton <S, ServantLinks>::epv_ = {
	{ // interface
		Bridge <ServantLinks>::interface_id_,
		S::template __duplicate <Object>,
		S::template __release <Object>
	},
	{ // epv
		S::_servant_base,
		S::_object,
		S::_is_active
	}
};

}
}

#endif
