// Nirvana project
// Object Request Broker
// Exception
#ifndef NIRVANA_ORB_EXCEPTION_H_
#define NIRVANA_ORB_EXCEPTION_H_

#include "I_ptr.h"

namespace CORBA {

class TypeCode;
typedef Nirvana::I_ptr <TypeCode> TypeCode_ptr;

class Exception
{
public:
	typedef void Data;

	virtual ~Exception ()
	{}
	virtual void _raise () const = 0;
	virtual const char* _name () const = 0;
	virtual const char* _rep_id () const = 0;

	// Nirvana specific
	virtual Long __code () const = 0;
	virtual TypeCode_ptr __type_code () const = 0;

	const void* __data () const
	{
		return this + 1;
	}

	enum {
		EC_NO_EXCEPTION = -3,
		EC_USER_EXCEPTION = -2,
		EC_SYSTEM_EXCEPTION = -1
	};

	/// Create dynamically allocated copy of the exception object.
	/// Then it must be deleted by operator `delete`.
	Exception* __clone () const;

protected:
	Exception ()
	{}

	Exception (const Exception&)
	{}

	Exception& operator = (const Exception&)
	{
		return *this;
	}
};

namespace Nirvana {

typedef const StaticInterface <TypeCode> ExceptionEntry;

extern void set_exception (EnvironmentBridge* environment, Long code, const char* rep_id, const void* param);
extern void set_exception (EnvironmentBridge* environment, const Exception& e);
extern void set_unknown_exception (EnvironmentBridge* environment);

}
} // namespace CORBA

#ifdef NIRVANA_C11
#define NIRVANA_DEFAULT_CONSTRUCTORS(t) t (const t&) = default; t (t&&) = default;
#else
#define NIRVANA_DEFAULT_CONSTRUCTORS(t)
#endif

#define DECLARE_EXCEPTION(e) \
NIRVANA_DEFAULT_CONSTRUCTORS(e)\
virtual void _raise () const;\
virtual const char* _rep_id () const;\
static const char repository_id_ [];\
virtual const char* _name () const;\
static constexpr const char* __name () { return #e; }\
virtual ::CORBA::TypeCode_ptr __type_code () const;\
static const e* _downcast (const ::CORBA::Exception* ep);\
static e* _downcast (::CORBA::Exception* ep) { return const_cast <e*> (_downcast ((const ::CORBA::Exception*)ep)); }\
static const e* _narrow (const ::CORBA::Exception* ep) { return _downcast (ep); }\
static e* _narrow (::CORBA::Exception* ep) { return _downcast (ep); }

#define DEFINE_EXCEPTION(e, rep_id) \
void e::_raise () const { throw *this; } \
const char e::repository_id_ [] = rep_id; \
const char* e::_rep_id () const { return repository_id_; } \
const char* e::_name () const { return __name (); }

#endif
