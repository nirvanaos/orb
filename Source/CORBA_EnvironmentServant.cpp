#include "CORBA_EnvironmentServant.h"

namespace CORBA {
namespace Nirvana {

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

	virtual Exception* exception () const
	{
		if (the_same_binary ()) {
			const void* p = (bridge_._epv ().epv.exception_value) (&bridge_);
			if (p)
				return const_cast <Exception*> ((const Exception*)p - 1);
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
