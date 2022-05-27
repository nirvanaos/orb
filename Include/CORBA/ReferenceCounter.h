// This file was generated from "ReferenceCounter.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_REFERENCECOUNTER_H_
#define IDL_REFERENCECOUNTER_H_

#ifndef IDL_DECLARED_CORBA_Internal_ReferenceCounter
#define IDL_DECLARED_CORBA_Internal_ReferenceCounter

namespace CORBA {
namespace Internal {

class ReferenceCounter;

template <>
struct Type <ReferenceCounter> : TypeItf <ReferenceCounter>
{};

}
}

#endif

namespace CORBA {
namespace Internal {

template <>
struct Definitions <ReferenceCounter>
{
};

NIRVANA_BRIDGE_BEGIN (ReferenceCounter, "IDL:CORBA/Internal/ReferenceCounter:1.0")
void (*add_ref) (Bridge <ReferenceCounter>*, Interface*);
void (*remove_ref) (Bridge <ReferenceCounter>*, Interface*);
Type <ULong>::ABI_ret (*refcount_value) (Bridge <ReferenceCounter>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ReferenceCounter> :
	public T,
	public Definitions <ReferenceCounter>
{
public:
	void add_ref ();
	void remove_ref ();
	Type <ULong>::VRet refcount_value ();
};

template <class T>
void Client <T, ReferenceCounter>::add_ref ()
{
	Environment _env;
	Bridge < ReferenceCounter>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.add_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
void Client <T, ReferenceCounter>::remove_ref ()
{
	Environment _env;
	Bridge < ReferenceCounter>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.remove_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
Type <ULong>::VRet Client <T, ReferenceCounter>::refcount_value ()
{
	Environment _env;
	Bridge < ReferenceCounter>& _b (T::_get_bridge (_env));
	Type <ULong>::C_ret _ret = (_b._epv ().epv.refcount_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

class ReferenceCounter : public ClientInterface <ReferenceCounter>
{
public:
};

}
}

#endif
