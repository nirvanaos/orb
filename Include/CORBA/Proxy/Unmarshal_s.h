#ifndef NIRVANA_ORB_UNMARSHAL_S_H_
#define NIRVANA_ORB_UNMARSHAL_S_H_

#include "Unmarshal.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, Unmarshal>
{
public:
	static const typename Bridge <Unmarshal>::EPV epv_;

protected:
	static ABI_enum _context (Bridge <Unmarshal>* _b, EnvironmentBridge* _env)
	{
		try {
			return Type <MarshalContext>::ret (S::_implementation (_b).context ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _adopt_memory (Bridge <Unmarshal>* _b, ::Nirvana::ConstPointer p, 
		::Nirvana::Size size, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).adopt_memory (p, size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static Interface* _unmarshal_interface (Bridge <Unmarshal>* _b, ::Nirvana::ConstPointer data,
		Type <String>::ABI_in iid, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).unmarshal_interface (data, Type <String>::in (iid));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static Interface* _unmarshal_type_code (Bridge <Unmarshal>* _b, ::Nirvana::ConstPointer data,
		EnvironmentBridge* _env)
	{
		try {
			return TypeI <TypeCode>::ret (S::_implementation (_b).unmarshal_type_code (data));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}
};

template <class S>
const Bridge <Unmarshal>::EPV Skeleton <S, Unmarshal>::epv_ = {
	{ // header
		Bridge <Unmarshal>::repository_id_,
		S::template __duplicate <Unmarshal>,
		S::template __release <Unmarshal>
	},
	{ // epv
		S::_context,
		S::_adopt_memory,
		S::_unmarshal_interface,
		S::_unmarshal_type_code
	}
};

}
}

#endif
