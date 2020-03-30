#ifndef NIRVANA_ORB_MARSHAL_S_H_
#define NIRVANA_ORB_MARSHAL_S_H_

#include "Marshal.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, Marshal>
{
public:
	static const typename Bridge <Marshal>::EPV epv_;

protected:
	static ABI_enum _context (Bridge <Marshal>* _b, EnvironmentBridge* _env)
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

	static ::Nirvana::UIntPtr _marshal_memory (Bridge <Marshal>* _b, ::Nirvana::ConstPointer p,
		::Nirvana::Size* size, ::Nirvana::Size release_size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).marshal_memory (p, Type <::Nirvana::Size>::inout (size), release_size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::UIntPtr _get_buffer (Bridge <Marshal>* _b, ::Nirvana::Size* size,
		::Nirvana::Pointer* buf_ptr, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).get_buffer (Type <::Nirvana::Size>::inout (size),
				Type <::Nirvana::Pointer>::out (buf_ptr));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::UIntPtr _marshal_object (Bridge <Marshal>* _b, Interface* obj, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).marshal_object (TypeI <Object>::in (obj));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::UIntPtr _marshal_type_code (Bridge <Marshal>* _b, Interface* tc, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).marshal_type_code (TypeI <TypeCode>::in (tc));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge <Marshal>::EPV Skeleton <S, Marshal>::epv_ = {
	{ // header
		Bridge <Marshal>::repository_id_,
		S::template __duplicate <Marshal>,
		S::template __release <Marshal>
	},
	{ // epv
		S::_context,
		S::_marshal_memory,
		S::_get_buffer,
		S::_marshal_object,
		S::_marshal_type_code
	}
};

}
}

#endif
