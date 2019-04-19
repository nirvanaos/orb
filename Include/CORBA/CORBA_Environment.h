#ifndef NIRVANA_ORB_CORBA_ENVIRONMENT_H_
#define NIRVANA_ORB_CORBA_ENVIRONMENT_H_

#include "Interface_c.h"

namespace CORBA {

typedef Nirvana::T_ptr <Environment> Environment_ptr;
typedef Nirvana::T_var <Environment> Environment_var;
typedef Nirvana::T_out <Environment> Environment_out;

//! CORBA::Environment
class Environment :
	public Nirvana::EnvironmentBase
{
public:
	typedef Environment_ptr _ptr_type;
/*
	static Environment_ptr _duplicate (Environment_ptr obj)
	{
		return static_cast <Environment*> (Nirvana::Interface::_duplicate (obj));
	}
*/
	static Environment_ptr _nil ()
	{
		return Environment_ptr::nil ();
	}

	virtual void exception (Exception* ex)
	{
		delete exception_;
		exception_ = ex;
	}

	Exception* exception () const
	{
		return exception_;
	}

	virtual operator Nirvana::Bridge <Environment>& () = 0;

	static Environment_ptr unmarshal (Nirvana::BridgeMarshal <Environment>* bridge);
};

}

#endif

