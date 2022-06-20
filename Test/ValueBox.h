// Test value box implementation
#ifndef NIRVANA_ORB_TEST_VALUEBOX_H_
#define NIRVANA_ORB_TEST_VALUEBOX_H_

#include <CORBA/CORBA.h>

class StringVal;
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_StringVal;

namespace CORBA {
namespace Internal {

template <> const Char RepIdOf <StringVal>::id [] = "IDL:StringVal:1.0";

}
}

class StringVal :
	public CORBA::Internal::ValueBox <StringVal, CORBA::Internal::String>
{
#ifdef LEGACY_CORBA_CPP
public:
#else
private:
	template <class T1, class ... Args>
	friend CORBA::servant_reference <T1> CORBA::make_reference (Args ... args);
#endif

	StringVal ()
	{}

	StringVal (const BoxedType& v) :
		value_ (v)
	{}

	StringVal (BoxedType&& v) :
		value_ (std::move (v))
	{}

private:
	friend class CORBA::Internal::ValueBox <StringVal, CORBA::Internal::String>;

	BoxedType value_;
};

#endif
