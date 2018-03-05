#pragma once
#include "Object.h"

namespace PortableServer {

using namespace CORBA;
using namespace CORBA::Nirvana;

class POA;
//typedef CORBA::Nirvana::T_ptr <POA> POA_ptr;
typedef CORBA::Nirvana::Bridge<POA>* POA_ptr; // Not defined yet

class ServantBase :
	public Nirvana::Servant <ServantBase, Object>
{
	typedef Nirvana::Servant <ServantBase, Object> Base;
public:
	virtual ~ServantBase ()
	{}

	virtual POA_ptr _default_POA ();

	virtual InterfaceDef_ptr _get_interface ();

	virtual Boolean _is_a (const Char* type_id);

	virtual Boolean _non_existent ();

	virtual void _add_ref ();
	virtual void _remove_ref ();
	virtual ULong _refcount_value ();

	static Bridge <Interface>* _find_interface (Bridge <AbstractBase>& base, const Char* id)
	{
		return _implementation (base)._POA_find_interface (id);
	}

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, Environment* env);

protected:
	ServantBase ()
	{}

	virtual Bridge <Interface>* _POA_find_interface (const Char* id);
};

}
