#ifndef NIRVANA_TESTORB_OBJECTIMPL_H_
#define NIRVANA_TESTORB_OBJECTIMPL_H_

#include "Server.h"

namespace CORBA {
namespace Nirvana {

class ObjectBase
{
public:
	ObjectBase (AbstractBase_ptr servant) :
		servant_ (servant)
	{}

	operator Bridge <AbstractBase>& () const
	{
		return *servant_;
	}

	AbstractBase_ptr abstract_base () const
	{
		return servant_;
	}

	Interface_ptr primary_interface () const
	{
		return servant_->_query_interface (0);
	}

	const Char* primary_interface_id () const
	{
		return primary_interface ()->_epv ().interface_id;
	}

	Interface_ptr _query_interface (String_in id);

	// Object operations

	static Interface* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return 0; // We dont implement it
	}

	InterfaceDef_ptr _get_interface () const
	{
		return InterfaceDef::_nil ();	// TODO: Implement
	}

	Boolean _is_a (const String& type_id) const
	{
		Interface* itf = servant_->_query_interface (type_id);
		if (itf)
			return true;
		else
			return false;
	}

	static ABI_ret <Boolean> __non_existent (Bridge <Object>*, EnvironmentBridge*)
	{
		return false;
	}

	Boolean _is_equivalent (Object_ptr other)
	{
		return _query_interface (0) == AbstractBase_ptr (other)->_query_interface (0);
	}

	ULong _hash (ULong maximum) const
	{
		return (ULong)(uintptr_t)this % maximum;
	}
	// TODO: More Object operations shall be here...
	
private:
	AbstractBase_ptr servant_;
};

template <class S>
class ObjectImpl :
	public ObjectBase,
	public InterfaceImplBase <S, AbstractBase>,
	public InterfaceImplBase <S, Object>
{
public:
	ObjectImpl (AbstractBase_ptr servant) :
		ObjectBase (servant)
	{}

	static Interface* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return ObjectBase::__get_implementation (obj, env);
	}

	static ABI_ret <Boolean> __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return ObjectBase::__non_existent (obj, env);
	}
};

}
}

#endif
