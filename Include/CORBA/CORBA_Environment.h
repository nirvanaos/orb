#ifndef NIRVANA_ORB_CORBA_ENVIRONMENT_H_
#define NIRVANA_ORB_CORBA_ENVIRONMENT_H_

#include "EnvironmentImpl.h"
#include "Implementation.h"

namespace CORBA {

typedef Nirvana::I_ptr <Environment> Environment_ptr;
typedef Nirvana::I_var <Environment> Environment_var;
typedef Nirvana::I_out <Environment> Environment_out;

//! CORBA::Environment
class Environment :
	public Nirvana::EnvironmentImpl <Environment>,
	public Nirvana::LifeCycleRefCntPseudo <Environment>
{
public:
	typedef Environment_ptr _ptr_type;

	template <class I>
	static Bridge <I>* _duplicate_impl (Bridge <I>* itf)
	{
		return Nirvana::LifeCycleRefCntPseudo <Environment>::_duplicate_impl (itf);
	}

	static Environment_ptr _duplicate (Environment_ptr obj)
	{
		return static_cast <Environment*> (
			static_cast <Nirvana::Bridge <Environment>*> (Interface::_duplicate (obj)));
	}

	static Environment_ptr _nil ()
	{
		return Environment_ptr::nil ();
	}

	static Environment_ptr _check (Environment_ptr p)
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

