#ifndef NIRVANA_ORB_NAMEDVALUE_S_H_
#define NIRVANA_ORB_NAMEDVALUE_S_H_

#include "NamedValue.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, NamedValue>
{
public:
	static const typename Bridge <NamedValue>::EPV epv_;

protected:
	static const ABI_VT_ret <String> _name (Bridge <NamedValue>* obj, Interface* env)
	{
		try {
			return Type <String>::VT_ret (S::_implementation (obj).name ());
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static const ABI_VT_ret <Any> _value (Bridge <NamedValue>* obj, Interface* env)
	{
		try {
			return Type <Any>::VT_ret (S::_implementation (obj).value ());
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Flags _flags (Bridge <NamedValue>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj).flags ();
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <NamedValue>::EPV Skeleton <S, NamedValue>::epv_ = {
	{ // header
		Bridge <NamedValue>::repository_id_,
		S::template __duplicate <NamedValue>,
		S::template __release <NamedValue>
	},
	{ // base
		S::template _wide <AbstractBase, Object>
	},
	{ // epv
		S::_name,
		S::_value,
		S::_flags
	}
};

}
}

#endif
