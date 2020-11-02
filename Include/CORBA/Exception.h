// Nirvana project
// Object Request Broker
// Exception
#ifndef NIRVANA_ORB_EXCEPTION_H_
#define NIRVANA_ORB_EXCEPTION_H_

#include "I_var.h"
#include <new>

namespace CORBA {

class TypeCode;
typedef Nirvana::I_ptr <TypeCode> TypeCode_ptr;
typedef Nirvana::I_var <TypeCode> TypeCode_var;

class Exception
{
public:
	typedef int_fast16_t Code;
	typedef void Data;

	virtual ~Exception ()
	{}
	virtual void _raise () const = 0;
	virtual const Char* _name () const NIRVANA_NOEXCEPT = 0;
	virtual const Char* _rep_id () const NIRVANA_NOEXCEPT = 0;

	// Nirvana specific
	virtual Code __code () const NIRVANA_NOEXCEPT = 0;
	virtual TypeCode_ptr __type_code () const NIRVANA_NOEXCEPT = 0;

	const void* __data () const NIRVANA_NOEXCEPT
	{
		return this + 1;
	}

	void* __data () NIRVANA_NOEXCEPT
	{
		return this + 1;
	}

	enum : Code {
		EC_NO_EXCEPTION = -3,
		EC_USER_EXCEPTION = -2,
		EC_SYSTEM_EXCEPTION = -1
	};
};

namespace Nirvana {

struct ExceptionEntry
{
	const Char* rep_id;
	size_t size;
	void (*construct) (void*);
};

template <class E>
void construct (void* p)
{
	new (p) E ();
}

extern void set_exception (I_ptr <EnvironmentBridge> environment, Exception::Code code, const Char* rep_id, const void* param) NIRVANA_NOEXCEPT;
extern void set_exception (I_ptr <EnvironmentBridge> environment, const Exception& e) NIRVANA_NOEXCEPT;
extern void set_unknown_exception (I_ptr <EnvironmentBridge> environment) NIRVANA_NOEXCEPT;

}
} // namespace CORBA

#ifdef NIRVANA_C11
#define NIRVANA_DEFAULT_CONSTRUCTORS(T) T (const T&) = default; T (T&&) = default;
#else
#define NIRVANA_DEFAULT_CONSTRUCTORS(T)
#endif

#define DECLARE_EXCEPTION(E) public: E () {}\
NIRVANA_DEFAULT_CONSTRUCTORS (E)\
virtual void _raise () const { throw *this; }\
virtual const ::CORBA::Char* _rep_id () const NIRVANA_NOEXCEPT { return repository_id_; }\
static const ::CORBA::Char repository_id_ [];\
virtual const ::CORBA::Char* _name () const NIRVANA_NOEXCEPT { return __name (); }\
static constexpr const ::CORBA::Char* __name () NIRVANA_NOEXCEPT { return #E; }\
virtual ::CORBA::TypeCode_ptr __type_code () const NIRVANA_NOEXCEPT;\
static const E* _downcast (const ::CORBA::Exception* ep) NIRVANA_NOEXCEPT;\
static E* _downcast (::CORBA::Exception* ep) NIRVANA_NOEXCEPT { return const_cast <E*> (_downcast ((const ::CORBA::Exception*)ep)); }\
static const E* _narrow (const ::CORBA::Exception* ep) NIRVANA_NOEXCEPT { return _downcast (ep); }\
static E* _narrow (::CORBA::Exception* ep) NIRVANA_NOEXCEPT { return _downcast (ep); }

#endif
