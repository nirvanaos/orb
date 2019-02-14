#ifndef NIRVANA_TESTORB_TEST_H_
#define NIRVANA_TESTORB_TEST_H_

namespace TestORB {

class Instance
{
public:
	static int count ()
	{
		return count_;
	}

protected:
	Instance ()
	{
		++count_;
	}

	~Instance ()
	{
		--count_;
	}

private:
	static int count_;
};

const long MAGIC_CONST = 1963;

}

#endif


