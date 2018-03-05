#include "PortableServer.h"

namespace PortableServer {

using namespace CORBA;

POA_ptr ServantBase::_default_POA ()
{
	return 0;
}

InterfaceDef_ptr ServantBase::_get_interface ()
{
	return Base::_get_interface ();
}

Boolean ServantBase::__is_a (Bridge <Object>* obj, const Char* type_id, Environment* env)
{
	try {
		Nirvana::_check_pointer (type_id);
		return _implementation (obj)._is_a (type_id);
	} catch (const Exception& e) {
		env->set_exception (e);
	} catch (...) {
		env->set_unknown_exception ();
	}
	return 0;
}

Boolean ServantBase::_is_a (const Char* type_id)
{
	return Nirvana::Bridge <Object>::___is_a (type_id);
}

Boolean ServantBase::_non_existent ()
{
	return Base::_non_existent ();
}

void ServantBase::_add_ref ()
{
	Base::_add_ref ();
}

void ServantBase::_remove_ref ()
{
	Base::_remove_ref ();
}

ULong ServantBase::_refcount_value ()
{
	return Base::_refcount_value ();
}

Bridge <Interface>* ServantBase::_POA_find_interface (const Char* id)
{
	return Nirvana::Skeleton <ServantBase, Object>::_find_interface (*this, id);
}

}
