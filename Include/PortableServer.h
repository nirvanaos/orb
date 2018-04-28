#ifndef NIRVANA_ORB_PORTABLESERVER_H_
#define NIRVANA_ORB_PORTABLESERVER_H_

#include "Object.h"

namespace PortableServer {

using namespace CORBA;
using namespace CORBA::Nirvana;

class POA;
//typedef CORBA::Nirvana::T_ptr <POA> POA_ptr;
typedef CORBA::Nirvana::Bridge<POA>* POA_ptr; // Not defined yet

namespace Nirvana {

// POA implementation

template <class I> class ServantPOA;

template <>
class ServantPOA <AbstractBase> :
	public InterfaceImpl <ServantPOA <AbstractBase>, AbstractBase>,
	public RefCountBase
{
public:
	template <class I>
	static ServantPOA <I>& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <ServantPOA <I>, I>::sm_epv.interface);
		return static_cast <ServantPOA <I>&> (*bridge);
	}

	template <class I>
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf, Environment* env)
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
	static void _release (Bridge <Interface>* itf)
	{
		try {
			_implementation (static_cast <Bridge <I>*> (itf))._remove_ref ();
		} catch (...) {
		}
	}

	template <class I, class Primary>
	static Bridge <I>& _narrow (ServantPOA <Primary>& base)
	{
		return static_cast <Bridge <I>&> (base);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, Environment* env)
	{
		try {
			return static_cast <Bridge <Base>*> (&_implementation (derived));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, Environment* env)
	{
		try {
			_check_pointer (base, sm_epv.interface);
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
	virtual Bridge <::CORBA::Nirvana::Interface>* _find_interface (const Char* id) = 0;
};

// Base interfaces
template <class ... I> // Only directly derived interface must be listed
class BaseImplPOA :
	public virtual ServantPOA <AbstractBase>,
	public virtual ServantPOA <I> ...
{};

template <>
class ServantPOA <::CORBA::Object> :
	public Implementation <ServantPOA <::CORBA::Object>, ::CORBA::Object, BaseImplPOA <>>
{
public:
	virtual POA_ptr _default_POA ()
	{
		return 0;
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return ObjectBase::_get_interface (_primary_interface ());
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		return ___is_a (type_id);
	}

	virtual Boolean _non_existent ()
	{
		return ObjectBase::_non_existent ();
	}

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, Environment* env)
	{
		try {
			_check_pointer (type_id);
			return _implementation (obj)._is_a (type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

protected:
	virtual Bridge <::CORBA::Nirvana::Interface>* _find_interface (const Char* id)
	{
		return Skeleton <ServantPOA <::CORBA::Object>, ::CORBA::Object>::_find_interface (*this, id);
	}
};

}

typedef Nirvana::ServantPOA <::CORBA::Object> ServantBase;

}

#endif
