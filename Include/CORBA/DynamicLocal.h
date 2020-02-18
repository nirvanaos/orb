#ifndef NIRVANA_ORB_DYNAMICLOCAL_H_
#define NIRVANA_ORB_DYNAMICLOCAL_H_

#include "DynamicImpl.h"
#include "Servant_var.h"

namespace CORBA {
namespace Nirvana {

/// Locally implemented dynamic pseudo object
template <class T>
class DynamicLocal :
	public DynamicImpl <T>
{
public:
	typedef T* _ptr_type;
	typedef PortableServer::Servant_var <T> _var_type;
	typedef PortableServer::Servant_out <T> _out_type;
	typedef PortableServer::Servant_var <T>& _inout_type;

	static T* _duplicate (T* obj)
	{
		if (obj)
			obj->_add_ref ();
		return obj;
	}

	static T* _nil ()
	{
		return 0;
	}
};

}

template <class T>
void release (Nirvana::DynamicLocal <T>* ptr)
{
	if (ptr)
		ptr->_remove_ref ();
}

}

#endif
