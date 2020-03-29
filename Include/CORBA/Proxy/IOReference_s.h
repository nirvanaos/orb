#ifndef NIRVANA_ORB_IOREFERENCE_S_H_
#define NIRVANA_ORB_IOREFERENCE_S_H_

#include "IOReference.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, IOReference>
{
public:
	static const typename Bridge <IOReference>::EPV epv_;

protected:
	static Interface* _create_marshaler (Bridge <IOReference>* _b, EnvironmentBridge* _env)
	{
		try {
			return TypeI <Marshal>::ret (S::_implementation (_b).create_marshaler ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static Interface* _call (Bridge <IOReference>* _b, Type <OperationIndex>::ABI_in op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, Interface** marshaler,
		::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size, EnvironmentBridge* _env)
	{
		try {
			return TypeI <Unmarshal>::ret (S::_implementation (_b).call (Type <OperationIndex>::in (op),
				in_params, in_params_size, TypeI <Marshal>::inout, out_params, out_params_size));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}
};

}
}

#endif
