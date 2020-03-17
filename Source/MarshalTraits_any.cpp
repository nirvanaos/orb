#include <CORBA/Proxy/MarshalTraits_any.h>

namespace CORBA {
namespace Nirvana {

void MarshalTraits <Any>::marshal_in (const Any& src, PlatformMarshal_ptr marshaler, ABI& dst)
{
	TypeCode_ptr tc = src.type ();
	if (!tc)
		dst.reset ();
	else {
		::Nirvana::ConstPointer psrc;
		::Nirvana::Pointer pdst;
		if (src.is_large ()) {
			ULong size = tc->_size ();
			::Nirvana::UIntPtr p = marshaler->get_buffer (size, pdst);
			dst.large_pointer ((::Nirvana::Pointer)p, size);
			pdst = dst.large_pointer ();
			psrc = src.large_pointer ();
		} else {
			pdst = dst.small_pointer ();
			psrc = src.small_pointer ();
		}
		tc->_marshal_in (psrc, marshaler, pdst);
		dst.type ((::Nirvana::Pointer)marshaler->marshal_type_code (tc));
	}
}

void MarshalTraits <Any>::marshal_out (Any& src, PlatformMarshal_ptr marshaler, ABI& dst)
{
	TypeCode_ptr tc = src.type ();
	if (!tc)
		dst.reset ();
	else {
		::Nirvana::Pointer psrc;
		::Nirvana::Pointer pdst;
		if (src.is_large ()) {
			psrc = src.large_pointer ();
			if (PlatformMarshalContext::SHARED_MEMORY == marshaler->context ()) {
				dst = src;
				src.reset ();
				pdst = dst.large_pointer ();
			} else {
				ULong size = tc->_size ();
				::Nirvana::UIntPtr p = marshaler->get_buffer (size, pdst);
				dst.large_pointer ((::Nirvana::Pointer)p, size);
				pdst = dst.large_pointer ();
			}
		} else {
			pdst = dst.small_pointer ();
			psrc = src.small_pointer ();
		}
		tc->_marshal_out (psrc, marshaler, pdst);
		dst.type ((::Nirvana::Pointer)marshaler->marshal_type_code (tc));
		release (tc);
		src.reset ();
	}
}

void MarshalTraits <Any>::unmarshal (ABI& src, PlatformUnmarshal_ptr unmarshaler, Any& dst)
{
	TypeCode* ptc = src.type ();
	if (!ptc)
		dst.reset ();
	else {
		TypeCode_var tc = unmarshaler->unmarshal_type_code (ptc);
		::Nirvana::Pointer psrc;
		::Nirvana::Pointer pdst;
		if (src.is_large ()) {
			psrc = src.large_pointer ();
			size_t size = src.large_size ();
			if (size)
				unmarshaler->adopt_memory (pdst = psrc, size);
			else
				pdst = ::Nirvana::g_memory->allocate (0, size, 0);
			try {
				tc->_unmarshal (psrc, unmarshaler, pdst);
			} catch (...) {
				::Nirvana::g_memory->release (pdst, size);
				throw;
			}
			dst.large_pointer (pdst, size);
		} else {
			pdst = dst.small_pointer ();
			psrc = src.small_pointer ();
			tc->_unmarshal (psrc, unmarshaler, pdst);
		}
		dst.type (tc._retn ());
	}
}

}
}
