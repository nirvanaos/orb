//! \interface DynamicServant
//! \brief The interface for dynamically allocated servant garbage collection.
//! \fn DynamicServant::_delete()
//! \brief Called by the garbage collector.

#ifndef NIRVANA_ORB_DYNAMICSERVANT_H_
#define NIRVANA_ORB_DYNAMICSERVANT_H_

#include "Client.h"
#include "Type_interface.h"

namespace CORBA {
namespace Nirvana {

class DynamicServant;
typedef I_ptr <DynamicServant> DynamicServant_ptr;
typedef I_var <DynamicServant> DynamicServant_var;
typedef I_out <DynamicServant> DynamicServant_out;

BRIDGE_BEGIN (DynamicServant, CORBA_NIRVANA_REPOSITORY_ID ("DynamicServant"))
void (*_delete) (Bridge <DynamicServant>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, DynamicServant> :
	public T
{
public:
	void _delete ();
};

class DynamicServant : public ClientInterface <DynamicServant>
{};

template <class T>
void Client <T, DynamicServant>::_delete ()
{
	Environment _env;
	Bridge <DynamicServant>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._delete) (&_b, &_env);
	_env.check ();
}

}
}

#endif
