// This file was generated from "IORequest.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_IOREQUEST_H_
#define IDL_IOREQUEST_H_

namespace CORBA {
namespace Nirvana {

class Marshal;
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_Marshal;
typedef ::CORBA::Nirvana::TypeItf <Marshal>::C_ptr Marshal_ptr;
typedef ::CORBA::Nirvana::TypeItf <Marshal>::C_var Marshal_var;

class IORequest;
typedef ::CORBA::Nirvana::TypeItf <IORequest>::C_ptr IORequest_ptr;
typedef ::CORBA::Nirvana::TypeItf <IORequest>::C_var IORequest_var;

template <>
struct Type <IORequest> : TypeItf < IORequest>
{};

template <>
struct Definitions < IORequest>
{
};

NIRVANA_BRIDGE_BEGIN (IORequest, "IDL:CORBA/Nirvana/IORequest:1.0")
Interface* (*_get_marshaler) (Bridge < IORequest>*, Interface*);
void (*set_exception) (Bridge < IORequest>* _b, Type <Any>::ABI_out, Interface* _env);
void (*success) (Bridge < IORequest>* _b, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, IORequest> :
	public T,
	public Definitions <IORequest>
{
public:
	TypeItf <Marshal>::Var marshaler ();
	void set_exception (Type <Any>::C_inout e);
	void success ();
};

template <class T>
TypeItf <Marshal>::Var Client <T, IORequest>::marshaler ()
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	TypeItf <Marshal>::C_ret _ret = (_b._epv ().epv._get_marshaler) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::set_exception (Type <Any>::C_inout e)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.set_exception) (&_b, &e, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::success ()
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.success) (&_b, &_env);
	_env.check ();
}

class IORequest : public CORBA::Nirvana::ClientInterface <IORequest>
{
public:
};

}
}

#endif
