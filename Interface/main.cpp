#include "Test_I1.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>

using namespace std;
using namespace CORBA;
using namespace Test;

// Dynamic implementation
class I1_dynamic :
	public PortableServer::Nirvana::Servant <I1_dynamic, Test::I1>
{
public:
	I1_dynamic (Long addendum) :
		m_addendum (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + m_addendum;
	}

private:
	Long m_addendum;
};

void test (I1_ptr p)
{
	try {
		assert (!is_nil (p));
		Object_ptr object = p;
		AbstractBase_ptr ab = object;
		Object_ptr o1 = ab->_to_object ();
		I1_ptr p1 = I1::_narrow (object);
		release (p1);
		Boolean b = p->_non_existent ();
		cout << "non_existent: " << (b != 0) << endl;
		if (p->_is_a ("IDL:omg.org/CORBA/Object:1.0"))
			cout << "Is CORBA.Object";
		else
			cout << "Isn't CORBA.Object";
		cout << endl;
		Long res = p->op1 (1);
		cout << "p->op1(0) = " << res << endl;
	} catch (const Exception& e) {
		cout << "Exception: " << e.code () << endl;
	}
	release (p);
}

void test_dynamic ()
{
	I1_dynamic* servant = new I1_dynamic (2);
	test (servant->_this ());
}

int main (int argc, char* argv [])
{
	test_dynamic ();
//	test_static ();
//	test_virtual ();
//	test_portable ();

	return 0;
}
