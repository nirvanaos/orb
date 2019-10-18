#ifndef NIRVANA_ORB_SEQUENCEABI_H_
#define NIRVANA_ORB_SEQUENCEABI_H_

#include <Nirvana/NirvanaBase.h>

namespace CORBA {
namespace Nirvana {

template <class T>
class SequenceABI
{
protected:
	T* begin_;
	size_t size_;
	size_t allocated_;
};

}
}

#endif
