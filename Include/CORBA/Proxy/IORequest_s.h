// This file was generated from "IORequest.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_IOREQUEST_S_H_
#define IDL_IOREQUEST_S_H_
#include "IORequest.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, IORequest>
{
public:
	static const typename Bridge <IORequest>::EPV epv_;

protected:
	static Interface* __get_marshaler (Bridge <IORequest>* _b, Interface* _env)
	{
		try {
			return TypeItf <Marshal>::ret (S::_implementation (_b).marshaler ());
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return TypeItf <Marshal>::ret ();
	}

	static void _set_exception (Bridge <IORequest>* _b, Type <Any>::ABI_out e, Interface* _env)
	{
		try {
			S::_implementation (_b).set_exception (Type <Any>::inout (e));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _success (Bridge <IORequest>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).success ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

};

template <class S>
const Bridge <IORequest>::EPV Skeleton <S, IORequest>::epv_ = {
	{ // header
		Bridge <IORequest>::repository_id_,
		S::template __duplicate <IORequest>,
		S::template __release <IORequest>
	},
	{ // EPV
		S::__get_marshaler,
		S::_set_exception,
		S::_success
	}
};

template <class S>
class Servant <S, IORequest> : public ImplementationPseudo <S, IORequest>
{};

template <>
class ServantPOA <IORequest> : public ImplementationPseudoPOA <IORequest>
{
public:
	virtual TypeItf <Marshal>::Var marshaler () = 0;
	virtual void set_exception (Type <Any>::Var& e) = 0;
	virtual void success () = 0;
};

template <class S>
class ServantStatic <S, IORequest> : public ImplementationPseudoStatic <S, IORequest>
{};

}
}
namespace POA_CORBA {
namespace Nirvana {

typedef ::CORBA::Nirvana::ServantPOA <CORBA::Nirvana::IORequest> IORequest;
template <class T> using IORequest_tie = ::CORBA::Nirvana::ServantTied <T, CORBA::Nirvana::IORequest>;

}
}

#endif
