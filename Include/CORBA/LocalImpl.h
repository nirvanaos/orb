/// Locally implemented pseudo objects (Environment etc)
#ifndef NIRVANA_ORB_LOCALIMPL_H_
#define NIRVANA_ORB_LOCALIMPL_H_

#include "BasicTypes.h"
#include "Servant_var.h"

namespace CORBA {
namespace Nirvana {

/// Locally implemented dynamic pseudo object
template <class T>
class LocalImpl
{
public:
	typedef T* _ptr_type;
	typedef PortableServer::Servant_var <T> _var_type;
	typedef PortableServer::Servant_out <T> _out_type;
	typedef PortableServer::Servant_var <T>& _inout_type;

	LocalImpl () :
		ref_cnt_ (1)
	{}

	LocalImpl (const LocalImpl&) :
		ref_cnt_ (1)
	{}

	LocalImpl& operator = (const LocalImpl&)
	{
		return *this; // Do nothing
	}

	void _add_ref ()
	{
		++ref_cnt_;
	}

	void _remove_ref ()
	{
		assert (ref_cnt_);
		if (!--ref_cnt_)
			delete& static_cast <T&> (*this);
	}

	ULong _refcount_value () const
	{
		return ref_cnt_;
	}

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

private:
	ULong ref_cnt_;
};

}

/// CORBA::release
template <class T> inline
void release (Nirvana::LocalImpl <T>* ptr)
{
	if (ptr)
		ptr->_remove_ref ();
}

}

#endif
