#pragma once
#include<Unknwnbase.h>
#include<xutility>
#undef INTERFACE
template<typename ...Ts>
struct DoQueryInterfaceHelper;
template<typename COM_TYPE, typename INTERFACE, typename ...Ts>
struct DoQueryInterfaceHelper<COM_TYPE, INTERFACE, Ts...> : private DoQueryInterfaceHelper<COM_TYPE, Ts...>
{
	static HRESULT DoQueryInterface(COM_TYPE* obj, REFIID riid, void** ppvObject)
	{
		if (ppvObject == nullptr)
		{
			return E_POINTER;
		}
		if (__uuidof(INTERFACE) == riid)
		{
			*ppvObject = reinterpret_cast<void*>(static_cast<INTERFACE*>(obj));
			obj->AddRef();
			return S_OK;
		}
		return DoQueryInterfaceHelper<COM_TYPE, Ts...>::DoQueryInterface(obj, riid, ppvObject);
	}
};
template<typename COM_TYPE, typename INTERFACE>
struct DoQueryInterfaceHelper<COM_TYPE, INTERFACE>
{
	static HRESULT DoQueryInterface(COM_TYPE* obj, REFIID riid, void** ppvObject)
	{
		if (ppvObject == nullptr)
		{
			return E_POINTER;
		}
		if (__uuidof(INTERFACE) == riid)
		{
			*ppvObject = reinterpret_cast<void*>(static_cast<INTERFACE*>(obj));
			return S_OK;
		}
		return E_NOINTERFACE;
	}
};

template<typename T, typename ...BaseType>
class RefCountImpl: public T
{
public:
	template<typename ...Arg>
	RefCountImpl(Arg&& ...args):
		T{std::forward<Arg>(args)...}
	{
		refCount = 1;
	}
	STDMETHOD(QueryInterface(REFIID riid,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject))
	{
		return DoQueryInterfaceHelper<T, BaseType...>::DoQueryInterface(this, riid, ppvObject);
	}
	STDMETHOD_(ULONG, AddRef())
	{
		return InterlockedIncrement(&refCount);
		
	}
	STDMETHOD_(ULONG, Release())
	{
		ULONG r = InterlockedDecrement(&refCount);
		if (r == 0)
		{
			delete this;
		}
		return r;
	}
	ULONG refCount;
};