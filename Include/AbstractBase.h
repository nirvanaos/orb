#ifndef NIRVANA_ORB_ABSTRACTBASE_H_
#define NIRVANA_ORB_ABSTRACTBASE_H_

#include "Interface.h"
#include "RefCount.h"

namespace CORBA {

typedef Nirvana::T_ptr <AbstractBase> AbstractBase_ptr;

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
			Bridge <Interface>* (*find_interface) (Bridge <AbstractBase>*, const Char*, Environment*);
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

namespace Nirvana {

extern void _check_pointer (const void* p);
extern void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv);

//! Servant part of an interface
template <class S, class I> class Skeleton;

template <class S>
class Skeleton <S, AbstractBase>
{
public:
	static const typename Bridge <AbstractBase>::EPV sm_epv;

protected:
	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, Environment* env)
	{
		try {
			_check_pointer (base, sm_epv.interface);
			_check_pointer (id);
			return S::_find_interface (*base, id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <AbstractBase>::EPV Skeleton <S, AbstractBase>::sm_epv = {
	{	// interface
		S::template _duplicate <AbstractBase>,
		S::template _release <AbstractBase>
	},
	{	// epv
		S::__find_interface
	}
};

}
}

#endif
