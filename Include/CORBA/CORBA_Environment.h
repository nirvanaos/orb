#ifndef NIRVANA_ORB_CORBA_ENVIRONMENT_H_
#define NIRVANA_ORB_CORBA_ENVIRONMENT_H_

#include "DynamicServant_c.h"

namespace CORBA {

typedef Nirvana::T_ptr <Environment> Environment_ptr;
typedef Nirvana::T_var <Environment> Environment_var;
typedef Nirvana::T_out <Environment> Environment_out;

//! CORBA::Environment
class Environment :
	public Nirvana::Bridge <Nirvana::DynamicServant>
{
public:
	typedef Environment_ptr _ptr_type;

	static Environment_ptr _duplicate (Environment_ptr obj)
	{
		return static_cast <Environment*> (Nirvana::Interface::_duplicate (obj));
	}

	static Environment_ptr _nil ()
	{
		return Environment_ptr::nil ();
	}

	virtual void exception (Exception* ex) = 0;

	virtual Exception* exception () const = 0;

	virtual void clear () = 0;

	virtual operator Nirvana::Bridge <Environment>& () = 0;

	static Environment_ptr unmarshal (Nirvana::BridgeMarshal <Environment>* bridge);

protected:
	Environment (const Bridge <Nirvana::DynamicServant>::EPV& epv) :
		Nirvana::Bridge <Nirvana::DynamicServant> (epv)
	{}
};

}

#endif

