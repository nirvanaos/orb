#include <CORBA/RepositoryId.h>
#include "CORBA_EnvironmentServant.h"
#include <string.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge < ::CORBA::Environment>::interface_id_ [] = CORBA_REPOSITORY_ID (Environment);

void BridgeMarshal < ::CORBA::Environment>::set_exception (Long code, const char* rep_id, const void* param)
{
	// Client can pass NULL environment in special cases, so we check this for NULL
	if (this && RepositoryId::compatible (_epv ().interface_id, Bridge < ::CORBA::Environment>::interface_id_)) {
		Bridge < ::CORBA::Environment>* b = &static_cast <Bridge < ::CORBA::Environment>&> (*this);
		(b->_epv ().epv.exception_set) (b, code, rep_id, param);
	}
}

void BridgeMarshal < ::CORBA::Environment>::set_exception (const Exception& e)
{
	set_exception (e.__code (), e._rep_id (), e.__data ());
}

void BridgeMarshal < ::CORBA::Environment>::set_unknown_exception ()
{
	set_exception (0, CORBA_REPOSITORY_ID (UNKNOWN), nullptr);
}

void EnvironmentBase::exception_set (Long code, const char* rep_id, const void* param, 
																		 const Nirvana::ExceptionEntry* const* user_exceptions)
{
	exception_free ();
	if (rep_id) {
		Exception* e = nullptr;
		try {
			if (code >= 0)
				e = SystemException::_create (rep_id, param, code);
			else if (user_exceptions) {
				for (const ExceptionEntry* const* p = user_exceptions; *p; ++p)
					if (RepositoryId::compatible ((*p)->rep_id, rep_id)) {
						e = ((*p)->create) (param);
						break;
					}
				if (!e)
					e = new UNKNOWN ();
			}
		} catch (...) {
		}
		exception_ = e;
	}
}

class CORBA_EnvironmentProxy :
	public CORBA_EnvironmentImpl <CORBA_EnvironmentProxy>
{
public:
	CORBA_EnvironmentProxy (Bridge < ::CORBA::Environment>* bridge) :
		bridge_ (*bridge)
	{}

	~CORBA_EnvironmentProxy ()
	{
		Interface::_release (&bridge_);
	}

	virtual operator Bridge < ::CORBA::Environment>& ()
	{
		return bridge_;
	}

	virtual void exception (Exception* ex)
	{
		(bridge_._epv ().epv.exception_set) (&bridge_, ex->__code (), ex->_rep_id (), ex->__data ());
		if (!the_same_binary ()) {
			delete exception_;
			exception_ = ex;
		}
	}

	virtual const Exception* exception () const
	{
		if (the_same_binary ()) {
			const void* p = (bridge_._epv ().epv.exception_value) (&bridge_);
			if (p)
				return (const Exception*)p - 1;
			else
				return nullptr;
		} else {
			const Char* ex_id = (bridge_._epv ().epv.exception_id) (&bridge_);
			if (ex_id) {
				if (!strcmp (ex_id, exception_->_rep_id ()))
					return exception_;
				delete exception_;
				const_cast <CORBA_EnvironmentProxy&> (*this).exception_ = nullptr;
				const_cast <CORBA_EnvironmentProxy&> (*this).exception_ =
					SystemException::_create (ex_id, (bridge_._epv ().epv.exception_value) (&bridge_));
			} else {
				delete exception_;
				const_cast <CORBA_EnvironmentProxy&> (*this).exception_ = nullptr;
			}
			return exception_;
		}
	}

	virtual void clear ()
	{
		exception_free ();
		(bridge_._epv ().epv.exception_free) (&bridge_);
	}

private:
	bool the_same_binary () const
	{
		return Nirvana::CORBA_EnvironmentServant::is_my_bridge (bridge_);
	}

private:
	Bridge < ::CORBA::Environment>& bridge_;
};

}

Environment_ptr Environment::unmarshal (Nirvana::BridgeMarshal <Environment>* bridge)
{
	if (bridge) {
		Nirvana::CORBA_EnvironmentServant* p = Nirvana::CORBA_EnvironmentServant::_narrow (bridge);
		if (p)
			return &static_cast <Environment&> (*p);

		if (Nirvana::RepositoryId::compatible (bridge->_epv ().interface_id, Nirvana::Bridge <Environment>::interface_id_))
			return new Nirvana::CORBA_EnvironmentProxy (&static_cast <Nirvana::Bridge <Environment>&> (*bridge));
	}
	return Environment_ptr::nil ();
}

}
