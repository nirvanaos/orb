#ifndef NIRVANA_TESTORB_OBJECTIMPL_H_
#define NIRVANA_TESTORB_OBJECTIMPL_H_

#include <CORBA/Object_s.h>
#include <CORBA/Implementation.h>

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
		return Interface::_check (servant_->_query_interface (nullptr));
	}

	const Char* primary_interface_id () const
	{
		return primary_interface ()->_epv ().interface_id;
	}

	Interface_ptr _query_interface (const Char* id);

	// Object operations

	static Interface* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return nullptr; // We dont implement it
	}

	InterfaceDef_ptr _get_interface () const
	{
		return InterfaceDef::_nil ();	// TODO: Implement
	}

	Boolean _is_a (const Char* type_id) const
	{
		Interface* itf = servant_->_query_interface (type_id);
		if (itf)
			return TRUE;
		else
			return FALSE;
	}

	static Boolean __non_existent (Bridge <Object>*, EnvironmentBridge*)
	{
		return FALSE;
	}

	Boolean _is_equivalent (Object_ptr other)
	{
		return _query_interface (nullptr) == AbstractBase_ptr (other)->_query_interface (nullptr);
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

	static Boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return ObjectBase::__non_existent (obj, env);
	}
};

}
}

#endif
