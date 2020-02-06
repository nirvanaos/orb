// Nirvana project
// Object Request Broker
// Exception
#ifndef NIRVANA_ORB_EXCEPTION_H_
#define NIRVANA_ORB_EXCEPTION_H_

#include "T_ptr.h"

#define DECLARE_EXCEPTION(e) \
virtual void raise () const;\
virtual const char* _rep_id () const;\
static const char repository_id_ [];\
virtual const char* _name () const;\
static constexpr const char* __name () { return #e; }\
virtual TypeCode_ptr __type_code () const;\
virtual Exception* __clone () const;\
static const e* _downcast (const Exception* ep);\
static e* _downcast (Exception* ep) { return const_cast <e*> (_downcast ((const Exception*)ep)); }\
static const e* _narrow (const Exception* ep) { return _downcast (ep); }\
static e* _narrow (Exception* ep) { return _downcast (ep); }\
static Exception* _create (const void* data);

#define DEFINE_EXCEPTION(e, rep_id) \
void e::raise () const { throw *this; } \
const char e::repository_id_ [] = rep_id; \
const char* e::_rep_id () const { return repository_id_; } \
const char* e::_name () const { return __name (); } \
Exception* e::__clone () const { return new e (*this); } \
Exception* e::_create (const void* data) { return new e ((Data*)data); } // TODO : Remove

#define EX_TABLE_ENTRY(e) { e::repository_id_, e::_create }

namespace CORBA {

class TypeCode;
typedef Nirvana::T_ptr <TypeCode> TypeCode_ptr;

class Exception
{
public:
	typedef void Data;

	virtual ~Exception ()
	{}
	virtual void raise () const = 0;
	virtual const char* _name () const = 0;
	virtual const char* _rep_id () const = 0;

	// Nirvana specific
	virtual Long __code () const = 0;
	virtual TypeCode_ptr __type_code () const = 0;
	virtual Exception* __clone () const = 0;	// TODO: Remove!

	const void* __data () const
	{
		return this + 1;
	}

	enum {
		EC_NO_EXCEPTION = -2,
		EC_USER_EXCEPTION = -1
	};

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

typedef ::CORBA::Exception* (*ExceptionCreateProc) (const void* data);

struct ExceptionEntry
{
	const char* rep_id;
	ExceptionCreateProc create;
};

}
} // namespace CORBA

#endif
