// Test value box implementation
#ifndef NIRVANA_ORB_TEST_VALUEBOX_H_
#define NIRVANA_ORB_TEST_VALUEBOX_H_

#include <CORBA/CORBA.h>

class StringVal;
extern const ::Nirvana::ImportInterfaceT < ::CORBA::TypeCode> _tc_StringVal;

namespace CORBA {
namespace Internal {

template <> const Char RepIdOf < ::StringVal>::id [] = "IDL:StringVal:1.0";

template <>
struct Type < ::StringVal> : TypeValueBox < ::StringVal>
{

	static I_ptr <TypeCode> type_code ()
	{
		return ::_tc_StringVal;
	}

};

}
}

class StringVal :
	public CORBA::Internal::ValueBox <StringVal, CORBA::Internal::String>
{
public:
	~StringVal ()
	{
		_value ().~BoxedType ();
	}

#ifdef LEGACY_CORBA_CPP
public:
#else
private:
	template <class T1, class ... Args>
	friend CORBA::servant_reference <T1> CORBA::make_reference (Args ... args);
#endif

	StringVal ()
	{
		new (&value_) BoxedType ();
	}

	StringVal (const BoxedType& v)
	{
		new (&value_) BoxedType (v);
	}

	StringVal (BoxedType&& v)
	{
		new (&value_) BoxedType (std::move (v));
	}
};

#endif
