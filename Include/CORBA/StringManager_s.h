#ifndef NIRVANA_ORB_STRINGMANAGER_S_H_
#define NIRVANA_ORB_STRINGMANAGER_S_H_

#include "StringManager_c.h"

namespace CORBA {
namespace Nirvana {

template <class S, class C>
class Skeleton <S, StringManager <C> >
{
public:
	static const typename Bridge <StringManager <C> >::EPV epv_;

protected:
	static C* _string_alloc (Bridge <StringManager <C> >* obj, ULong len, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).string_alloc (len);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	static C* _string_dup (Bridge <StringManager <C> >* obj, const C* s, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).string_dup (s);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	static void _string_free (Bridge <StringManager <C> >* obj, C* s, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj).string_free (s);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
	}

	static C* _at (Bridge <StringManager <C> >* obj, C** s, ULong index, EnvironmentBridge* env)
	{
		try {
			_check_pointer (s);
			return &S::_implementation (obj).at (*s, index);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}
};

template <class S, class C>
const typename Bridge <StringManager <C> >::EPV Skeleton <S, StringManager <C> >::epv_ = {
	{ // interface
		Bridge <StringManager <C> >::interface_id_,
		S::template __duplicate <StringManager <C> >,
		S::template __release <StringManager <C> >
	},
	{ // epv
		S::_string_alloc,
		S::_string_dup,
		S::_string_free,
		S::_at
	}
};

}
}

#endif
