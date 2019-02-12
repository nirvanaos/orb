#ifndef NIRVANA_ORB_ABSTRACTBASE_H_
#define NIRVANA_ORB_ABSTRACTBASE_H_

#include "Interface.h"

namespace CORBA {

typedef Nirvana::T_ptr <AbstractBase> AbstractBase_ptr;
typedef Nirvana::T_var <AbstractBase> AbstractBase_var;
typedef Nirvana::T_out <AbstractBase> AbstractBase_out;

namespace Nirvana {

// AbstractBase

template <>
class Bridge <AbstractBase> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Interface>* (*find_interface) (Bridge <AbstractBase>*, const Char*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (const EPV&)Bridge <Interface>::_epv ();
	}

protected:
	Bridge (const EPV& epv) :
		Bridge <Interface> (epv.interface)
	{}

	Bridge ()
	{}
};

template <class T>
class Client <T, AbstractBase> :
	public ClientBase <T, AbstractBase>
{
public:
	Bridge <Interface>* _find_interface (const Char* type_id);
};

template <class T>
Bridge <Interface>* Client <T, AbstractBase>::_find_interface (const Char* type_id)
{
	Environment env;
	Bridge <AbstractBase>& bridge = ClientBase <T, AbstractBase>::_bridge ();
	Bridge <Interface>* ret = (bridge._epv ().epv.find_interface) (&bridge, type_id, &env);
	env.check ();
	return ret;
}

}

class AbstractBase :
	public Nirvana::ClientInterface <AbstractBase>
{
public:
	typedef AbstractBase_ptr _ptr_type;

	inline Object_ptr _to_object ();
};

// Servant part of an interface

namespace Nirvana {

extern void _check_pointer (const void* p);
extern void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv);

template <class S, class I> class Skeleton;

template <class S>
class Skeleton <S, AbstractBase>
{
public:
	static const typename Bridge <AbstractBase>::EPV epv_;

protected:
	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, EnvironmentBridge* env)
	{
		Bridge <Interface>* ret = nullptr;
		try {
			ret = S::_implementation (base)._find_interface (id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		if (ret)
			ret = (ret->_epv ().duplicate) (ret, env);
		return ret;
	}

public:
	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		try {
			_check_pointer (itf, Skeleton <S, I>::epv_.interface);
			return S::_duplicate (static_cast <Bridge <I>*> (itf));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	template <class I>
	static void __release (Bridge <Interface>* itf)
	{
		try {
			_check_pointer (itf, Skeleton <S, I>::epv_.interface);
			S::_release (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

template <class S>
const Bridge <AbstractBase>::EPV Skeleton <S, AbstractBase>::epv_ = {
	{	// interface
		&(S::template __duplicate <AbstractBase>),
		&(S::template __release <AbstractBase>)
	},
	{	// epv
		&S::__find_interface
	}
};

}
}

#endif
