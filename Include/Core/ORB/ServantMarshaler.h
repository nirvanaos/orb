#ifndef NIRVANA_ORB_CORE_SERVANTMARSHALER_H_
#define NIRVANA_ORB_CORE_SERVANTMARSHALER_H_

#include <CORBA/Proxy/Marshal_s.h>
#include <CORBA/Proxy/Unmarshal_s.h>
#include <Core/LifeCycle.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

class ServantMarshaler :
	public ServantTraits <ServantMarshaler>,
	public ::Nirvana::Core::LifeCycle <ServantMarshaler>,
	public InterfaceImplBase <ServantMarshaler, Marshal>,
	public InterfaceImplBase <ServantMarshaler, Unmarshal>
{
public:
	ServantMarshaler (::Nirvana::Memory_ptr heap) :
		memory_ (heap)
	{}

	Marshal_ptr marshaler ()
	{
		return &static_cast <Marshal&> (static_cast <Bridge <Marshal>&> (*this));
	}

	Unmarshal_ptr unmarshaler ()
	{
		return &static_cast <Unmarshal&> (static_cast <Bridge <Unmarshal>&> (*this));
	}

	static Unmarshal_ptr unmarshaler (Marshal_ptr marshaler)
	{
		return Unmarshal::_check (static_cast <Bridge <Unmarshal>*> (static_cast <ServantMarshaler*> (&marshaler)));
	}

	using InterfaceImplBase <ServantMarshaler, Marshal>::_context;
	using InterfaceImplBase <ServantMarshaler, Unmarshal>::_context;
	MarshalContext context ();

	::Nirvana::UIntPtr marshal_memory (::Nirvana::ConstPointer p, ::Nirvana::Size& size, ::Nirvana::Size release_size);
	::Nirvana::UIntPtr get_buffer (::Nirvana::Size& size, ::Nirvana::Pointer& buf_ptr);
	::Nirvana::UIntPtr marshal_object (Object_ptr);
	::Nirvana::UIntPtr marshal_type_code (TypeCode_ptr);

	void adopt_memory (::Nirvana::ConstPointer p, ::Nirvana::Size size);
	NIRVANA_NODISCARD Interface* unmarshal_interface (::Nirvana::ConstPointer marshal_data, const String& iid);
	TypeCode_var unmarshal_type_code (::Nirvana::ConstPointer marshal_data);

private:
	::Nirvana::Memory_var memory_;
};

}
}
}

#endif
