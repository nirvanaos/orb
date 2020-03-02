#ifndef NIRVANA_ORB_SERVANTBASELINK_H_
#define NIRVANA_ORB_SERVANTBASELINK_H_

#include "ServantBase_s.h"

namespace CORBA {
namespace Nirvana {

//! \brief Implements delegate to the core ServantBase implementation.
class ServantBaseLink :
	public Bridge <PortableServer::ServantBase>
{
public:
	operator Bridge <Object>& ()
	{
		return static_cast <Bridge <Object>&> (*_get_proxy (Object::interface_id_));
	}

	// ServantBase operations

	PortableServer::POA_var _default_POA () const
	{
		return servant_base_->_default_POA ();
	}

	InterfaceDef_var _get_interface () const
	{
		return servant_base_->_get_interface ();
	}

	Boolean _is_a (const String& type_id) const
	{
		return servant_base_->_is_a (type_id);
	}

	Boolean _non_existent () const
	{
		return false;
	}

	// Our extensions
	Boolean _is_active () const
	{
		return !servant_base_->_non_existent ();
	}

	PortableServer::Servant __core_servant () const
	{
		return servant_base_;
	}

protected:
	ServantBaseLink (const Bridge <PortableServer::ServantBase>::EPV& epv) :
		Bridge <PortableServer::ServantBase> (epv)
	{}

	ServantBaseLink (const ServantBaseLink&) = delete;
	ServantBaseLink& operator = (const ServantBaseLink&)
	{
		return *this; // Do nothing
	}

	void _construct ();

	Interface* _get_proxy (String_in iid = 0);

protected:
	PortableServer::ServantBase_var servant_base_;
};

}
}

#endif
