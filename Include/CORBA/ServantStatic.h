#ifndef NIRVANA_ORB_SERVANTSTATIC_H_
#define NIRVANA_ORB_SERVANTSTATIC_H_

#include "ServantImpl.h"
#include "T_ptr.h"

namespace CORBA {
namespace Nirvana {

template <class S, class I> class ServantStatic;

//! Static servant traits
template <class S>
class ServantTraitsStatic :
	public ServantTraits <S>
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		return *(S*)nullptr;
	}
};

template <class S>
class ServantTraitsStaticEx :
	public ServantTraits <S>
{
public:
	template <class I>
	static S _implementation (Bridge <I>* bridge)
	{
		return S ();
	}
};

template <class S, class I>
class InterfaceStaticBase :
	public Skeleton <S, I>
{
public:
	operator Bridge <I>& () const
	{
		return *_bridge ();
	}

	static Bridge <I>* _bridge ()
	{
		return reinterpret_cast <Bridge <I>*> (&bridge_);
	}

	static const typename Bridge <I>::EPV* bridge_;
};

#define STATIC_BRIDGE(S, I) reinterpret_cast <::CORBA::Nirvana::Bridge <I>*> (&::CORBA::Nirvana::InterfaceStaticBase < S, I>::bridge_)

template <class S, class I>
const typename Bridge <I>::EPV* InterfaceStaticBase <S, I>::bridge_ = &InterfaceStaticBase <S, I>::epv_;

template <class S, class I>
class InterfaceStatic :
	public InterfaceStaticBase <S, I>
{};

//! \class ImplementationPseudo
//!
//! \brief Static implementation of a pseudo interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationStaticPseudo :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic,
	public InterfaceStaticBase <S, Bases> ...,
	public InterfaceStaticBase <S, Primary>
{
public:
	constexpr static Primary* _get_ptr ()
	{
		return static_cast <Primary*> (InterfaceStaticBase <S, Primary>::_bridge ());
	}
};

}
}

#endif

