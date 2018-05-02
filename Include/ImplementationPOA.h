// POA Nirvana interface implementation

#ifndef NIRVANA_ORB_IMPLEMENTATIONPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPOA_H_

#include "Implementation.h"

namespace PortableServer {
namespace Nirvana {

using namespace ::CORBA::Nirvana;

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

	template <class I>
	static Bridge <I>& _narrow (Bridge <I>& base)
	{
		return base;
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

template <class Primary, class ... Base> // Base includes only directly derived interfaces
class ImplementationPOA :
	public virtual ServantPOA <AbstractBase>,
	public virtual ServantPOA <Base> ...,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	typedef Primary _PrimaryInterface;

	static const Char* _primary_interface ()
	{
		return Bridge <Primary>::_primary_interface ();
	}

	virtual ::CORBA::Nirvana::Bridge <::CORBA::Nirvana::Interface>* _find_interface (const Char* id)
	{
		return InterfaceImpl <ServantPOA <Primary>, Primary>::_find_interface (*this, id);
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		return Bridge <Primary>::___is_a (type_id);
	}

	T_ptr <Primary> _this ()
	{
		return InterfaceImpl <ServantPOA <Primary>, Primary>::_this ();
	}
};

}
}

#endif
