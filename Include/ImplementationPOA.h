// Nirvana project
// Object Request Broker
// POA Nirvana interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPOA_H_

#include "Implementation.h"

namespace CORBA {
namespace Nirvana {

template <class I> class ServantPOA;

// Virtual implementation of AbstractBase

template <>
class ServantPOA <AbstractBase> :
	public InterfaceImpl <ServantPOA <AbstractBase>, AbstractBase>,
	public RefCountBase
{
public:
	template <class I>
	static ServantPOA <I>& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <ServantPOA <I>, I>::epv_.interface);
		return static_cast <ServantPOA <I>&> (*bridge);
	}

	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		try {
			_implementation (static_cast <Bridge <I>*> (itf))._add_ref ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return itf;
	}

	template <class I>
	static void __release (Bridge <Interface>* itf)
	{
		try {
			_implementation (static_cast <Bridge <I>*> (itf))._remove_ref ();
		} catch (...) {
		}
	}

	template <class I, class S>
	static Bridge <I>& _narrow (S& servant)
	{
		servant._add_ref ();
		return static_cast <Bridge <I>&> (servant);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, EnvironmentBridge* env)
	{
		try {
			return &static_cast <Bridge <Base>&> (_implementation (derived));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, EnvironmentBridge* env)
	{
		try {
			_check_pointer (base, epv_.interface);
			_check_pointer (id);
			return static_cast <ServantPOA <AbstractBase>&> (*base)._find_interface (id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	virtual void _add_ref ()
	{
		RefCountBase::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		RefCountBase::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return RefCountBase::_refcount_value ();
	}

protected:
	virtual Bridge <Interface>* _find_interface (const Char* id) = 0;
};

typedef ServantPOA <AbstractBase> AbstractBasePOA;

// Virtual implementation of ServantBase

template <>
class ServantPOA <ServantBase> :
	public virtual ServantPOA <AbstractBase>,
	public InterfaceImpl <ServantPOA <ServantBase>, ServantBase>,
	public ServantBaseImpl
{
public:
	// ServantBase operations

	virtual POA_ptr _default_POA ()
	{
		return ServantBaseImpl::_default_POA ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return ServantBaseImpl::_get_interface ();
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		return ServantBaseImpl::_is_a (type_id);
	}

	virtual Boolean _non_existent ()
	{
		return ServantBaseImpl::_non_existent ();
	}

protected:
	ServantPOA () :
		ServantBaseImpl (this, _primary_interface ())
	{}
protected:
	virtual const Char* _primary_interface () = 0;
};

template <>
class ServantPOA <Object> :
	public ServantPOA <ServantBase>
{
public:
	// For _narrow() and _wide() operations
	operator Bridge <Object>& ()
	{
		_activate ();
		return *servant_links_->object;
	}

protected:
	virtual const Char* _primary_interface ()
	{
		return Bridge <Object>::interface_id_;
	}

	virtual Bridge <Interface>* _find_interface (const Char* id)
	{
		return Skeleton <ServantPOA <Object>, Object>::_find_interface (*this, id);
	}
};

typedef ServantPOA <Object> ObjectPOA;

}
}

#endif
