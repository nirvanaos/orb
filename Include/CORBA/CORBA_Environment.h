#ifndef NIRVANA_ORB_CORBA_ENVIRONMENT_H_
#define NIRVANA_ORB_CORBA_ENVIRONMENT_H_

#include "EnvironmentImpl.h"
#include "Implementation.h"

namespace CORBA {

typedef Nirvana::T_ptr <Environment> Environment_ptr;
typedef Nirvana::T_var <Environment> Environment_var;
typedef Nirvana::T_out <Environment> Environment_out;

//! CORBA::Environment
class Environment :
	public Nirvana::EnvironmentImpl <Environment>,
	public Nirvana::LifeCycleRefCntPseudo <Environment>
{
public:
	typedef Environment_ptr _ptr_type;

	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		return Nirvana::LifeCycleRefCntPseudo <Environment>::_duplicate (itf);
	}

	static Environment_ptr _duplicate (Environment_ptr obj)
	{
		return static_cast <Environment*> (_duplicate (static_cast <Nirvana::Bridge <Environment>*> (obj)));
	}

	static Environment_ptr _nil ()
	{
		return Environment_ptr::nil ();
	}

	static Environment_ptr unmarshal (Environment_ptr p)
	{
		return p;
	}

	void clear ()
	{
		exception_free ();
	}
};

}

#endif

