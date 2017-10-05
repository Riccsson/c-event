#ifndef CSHARPEVENT_H
#define CSHARPEVENT_H

// Reference: http://www.vojkovic.net/post/2008/04/29/C-like-events-in-C2b2b.aspx

// Std includes.
#include <vector>
#include <stddef.h>
#include <stdexcept>

struct Void
{
};

class BaseDelegate
{
protected:
	void* _void_ownwerMember;
	unsigned long long _void_method;
	//--------------------------------------------
	BaseDelegate(void* _class, unsigned long long _method)
		: _void_ownwerMember(_class)
		, _void_method(_method)
	{
	}
public: // TODO: Make this as friend instead.
		//--------------------------------------------
	bool _equals(const BaseDelegate& other)
	{
		// Return TRUE if instance pointer and method pointer is the same.
		return (_void_ownwerMember == other._void_ownwerMember && _void_method == other._void_method);
	}
};

#pragma region Event.

//---------------------------------------------------------------------------------------
template<typename ReturnType, typename DelegateType>
class event
{
	std::vector< DelegateType > _listOfDelegates;

public:

	//--------------------------------------------
	// Desctructor.
	~event()
	{
		//for (size_t i = 0; i < _listOfDelegates.size(); i++)
		//{
		//	delete _listOfDelegates[i];
		//}
	}

	//--------------------------------------------
	bool operator!=(void* nullPtr)
	{
		return (nullPtr == nullptr && !_listOfDelegates.empty());
	}

	//--------------------------------------------
	bool operator==(void* nullPtr)
	{
		return (nullPtr == nullptr && _listOfDelegates.empty());
	}

	//--------------------------------------------
	event& operator+=(DelegateType t)
	{
		_listOfDelegates.push_back(t);
		return *this;
	}

	//--------------------------------------------
	event& operator-=(DelegateType t)
	{
		for (int i = static_cast<int>(_listOfDelegates.size() - 1); i > -1; --i)
		{
			DelegateType& deleg(_listOfDelegates[i]);

			// If delegate already exist. Remove it.
			if (deleg._equals(t))
			{
				// Remove delegate from list.
				_listOfDelegates.erase(_listOfDelegates.begin() + i);

				// Jump out, if there is several delegates with the same value, we need
				// to use -= operator again.
				break;
			}
		}

		return *this;
	}

	//--------------------------------------------
	std::vector<BaseDelegate>& GetInvocationList()
	{
		return _listOfDelegates;
	}

	//--------------------------------------------
	ReturnType Invoke()
	{
		size_t size(_listOfDelegates.size());

		if (size == 0)
		{
			// Can not invoke an empty (Nulled) C# event.
			throw std::runtime_error("Object reference not set to an instance of an object.");
		}

		if (size > 1)
		{
			for (size_t i = 0; i < size - 1; i++)
			{
				_listOfDelegates[i].Invoke();
			}
		}
		return _listOfDelegates[size - 1].Invoke();
	}
	//--------------------------------------------
	template<typename Arg1> ReturnType Invoke(Arg1 arg1)
	{
		size_t size(_listOfDelegates.size());

		if (size == 0)
		{
			// Can not invoke an empty (Nulled) C# event.
			throw std::runtime_error("Object reference not set to an instance of an object.");
		}

		if (size > 1)
		{
			for (size_t i = 0; i < size - 1; i++)
			{
				_listOfDelegates[i].Invoke(arg1);
			}
		}
		return _listOfDelegates[size - 1].Invoke(arg1);
	}
	//--------------------------------------------
	template<typename Arg1, typename Arg2> ReturnType Invoke(Arg1 arg1, Arg2 arg2)
	{
		size_t size(_listOfDelegates.size());

		if (size == 0)
		{
			// Can not invoke an empty (Nulled) C# event.
			throw std::runtime_error("Object reference not set to an instance of an object.");
		}

		if (size > 1)
		{
			for (size_t i = 0; i < size - 1; i++)
			{
				_listOfDelegates[i].Invoke(arg1, arg2);
			}
		}
		return _listOfDelegates[size - 1].Invoke(arg1, arg2);
	}
	//--------------------------------------------
	template<typename Arg1, typename Arg2, typename Arg3> ReturnType Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3)
	{
		size_t size(_listOfDelegates.size());

		if (size == 0)
		{
			// Can not invoke an empty (Nulled) C# event.
			throw std::runtime_error("Object reference not set to an instance of an object.");
		}

		if (size > 1)
		{
			for (size_t i = 0; i < size - 1; i++)
			{
				_listOfDelegates[i].Invoke(arg1, arg2, arg3);
			}
		}
		return _listOfDelegates[size - 1].Invoke(arg1, arg2, arg3);
	}

	//--------------------------------------------
	ReturnType operator() ()
	{
		return Invoke();
	}
	//--------------------------------------------
	template<typename Arg1> ReturnType operator() (Arg1 arg1)
	{
		return Invoke(arg1);
	}
	//--------------------------------------------
	template<typename Arg1, typename Arg2> ReturnType operator() (Arg1 arg1, Arg2 arg2)
	{
		return Invoke(arg1, arg2);
	}
	//--------------------------------------------
	template<typename Arg1, typename Arg2, typename Arg3> ReturnType operator() (Arg1 arg1, Arg2 arg2, Arg3 arg3)
	{
		return Invoke(arg1, arg2, arg3);
	}
};

#pragma endregion


#pragma region InnerHolders

		template<typename ToReturn> class Holder0
		{
		public:
			virtual ToReturn Invoke() = 0;
			virtual void _delete() = 0;
			virtual Holder0<ToReturn>* _clone() = 0;
		};
		template<typename ToReturn, typename Arg1> class Holder1
		{
		public:
			virtual ToReturn Invoke(Arg1 arg1) = 0;
			virtual void _delete() = 0;
			virtual Holder1<ToReturn, Arg1>* _clone() = 0;
		};
		template<typename ToReturn, typename Arg1, typename Arg2> class Holder2
		{
		public:
			virtual ToReturn Invoke(Arg1 arg1, Arg2 arg2) = 0;
			virtual void _delete() = 0;
			virtual Holder2<ToReturn, Arg1, Arg2>* _clone() = 0;
		};

		template<typename ToReturn, typename Class> class InnerHolder0 : public Holder0<ToReturn>
		{
			typedef ToReturn(Class::*FuncTypeClass)();
			typedef ToReturn(*FuncType)();
		public:
			Class* _ownerMember;
			FuncTypeClass _methodClass;
			FuncType _method;

			InnerHolder0(Class* ownerMember, FuncTypeClass method)
				: _ownerMember(ownerMember)
				, _methodClass(method)
				, _method(nullptr)
			{
			}
			InnerHolder0(unsigned long long method)
				: _ownerMember(nullptr)
				, _methodClass(nullptr)
				, _method(reinterpret_cast<FuncType&>(method))
			{
			}
			virtual ToReturn Invoke()
			{
				if (_ownerMember == nullptr)
					return _method();
				else
					return (_ownerMember->*_methodClass)();
			}
			virtual void _delete()
			{
				delete _ownerMember;
				_ownerMember = nullptr;
			}
			virtual Holder0<ToReturn>* _clone()
			{
				InnerHolder0<ToReturn, Class>* newMe(nullptr);
				if (_ownerMember == nullptr)
					newMe = new InnerHolder0<ToReturn, Class>((unsigned long long)_method);
				else
					newMe = new InnerHolder0<ToReturn, Class>(_ownerMember, _methodClass);
				newMe->_ownerMember = _ownerMember;
				newMe->_methodClass = _methodClass;
				newMe->_method = _method;
				return newMe;
			}
		};
		template<typename ToReturn, typename Class, typename Arg1> class InnerHolder1 : public Holder1<ToReturn, Arg1>
		{
			typedef ToReturn(Class::*FuncTypeClass)(Arg1);
			typedef ToReturn(*FuncType)(Arg1);
		public:
			Class* _ownerMember;
			FuncTypeClass _methodClass;
			FuncType _method;

			InnerHolder1(Class* ownerMember, FuncTypeClass method)
				: _ownerMember(ownerMember)
				, _methodClass(method)
				, _method(nullptr)
			{
			}
			InnerHolder1(unsigned long long method)
				: _ownerMember(nullptr)
				, _methodClass(nullptr)
				, _method(reinterpret_cast<FuncType&>(method))
			{
			}
			virtual ToReturn Invoke(Arg1 arg1)
			{
				if (_ownerMember == nullptr)
					return _method(arg1);
				else
					return (_ownerMember->*_methodClass)(arg1);
			}
			virtual void _delete()
			{
				delete _ownerMember;
				_ownerMember = nullptr;
			}
			virtual Holder1<ToReturn, Arg1>* _clone()
			{
				InnerHolder1<ToReturn, Class, Arg1>* newMe(nullptr);
				if (_ownerMember == nullptr)
					newMe = new InnerHolder1<ToReturn, Class, Arg1>((unsigned long long)_method);
				else
					newMe = new InnerHolder1<ToReturn, Class, Arg1>(_ownerMember, _methodClass);
				newMe->_ownerMember = _ownerMember;
				newMe->_methodClass = _methodClass;
				newMe->_method = _method;
				return newMe;
			}
		};
		template<typename ToReturn, typename Class, typename Arg1, typename Arg2> class InnerHolder2 : public Holder2<ToReturn, Arg1, Arg2>
		{
			typedef ToReturn(Class::*FuncTypeClass)(Arg1, Arg2);
			typedef ToReturn(*FuncType)(Arg1, Arg2);
		public:
			Class* _ownerMember;
			FuncTypeClass _methodClass;
			FuncType _method;

			InnerHolder2(Class* ownerMember, FuncTypeClass method)
				: _ownerMember(ownerMember)
				, _methodClass(method)
				, _method(nullptr)
			{
			}
			InnerHolder2(unsigned long long method)
				: _ownerMember(nullptr)
				, _methodClass(nullptr)
				, _method(reinterpret_cast<FuncType&>(method))
			{
			}
			virtual ToReturn Invoke(Arg1 arg1, Arg2 arg2)
			{
				if (_ownerMember == nullptr)
					return _method(arg1, arg2);
				else
					return (_ownerMember->*_methodClass)(arg1, arg2);
			}
			virtual void _delete()
			{
				delete _ownerMember;
				_ownerMember = nullptr;
			}
			virtual Holder2<ToReturn, Arg1, Arg2>* _clone()
			{
				InnerHolder2<ToReturn, Class, Arg1, Arg2>* newMe(nullptr);
				if (_ownerMember == nullptr)
					newMe = new InnerHolder2<ToReturn, Class, Arg1, Arg2>((unsigned long long)_method);
				else
					newMe = new InnerHolder2<ToReturn, Class, Arg1, Arg2>(_ownerMember, _methodClass);
				newMe->_ownerMember = _ownerMember;
				newMe->_methodClass = _methodClass;
				newMe->_method = _method;
				return newMe;
			}
		};

#pragma endregion

#pragma region Create delegates

// DELEGATES.
//===========================================================================================================================================================
#define CREATE_DELEGATE0(ToReturn, DelegateName)																											\
class DelegateName : public BaseDelegate																													\
{																																							\
	typedef ToReturn ( *FuncType )();																														\
	Holder0<ToReturn>* _holder;																																\
public:																																						\
	template<typename Class> DelegateName(void* OwnerClass, ToReturn (Class::*Method)()) : BaseDelegate(OwnerClass, (unsigned long long)&Method)					\
	{																																						\
		_holder = new InnerHolder0<ToReturn, Class>((Class*)OwnerClass, Method);																			\
	}																																						\
	DelegateName(FuncType Method) : BaseDelegate(nullptr, (unsigned long long)Method)																				\
	{																																						\
		_holder = new InnerHolder0<ToReturn, Void>((unsigned long long)Method);																					\
	}																																						\
	DelegateName(const DelegateName& other) : BaseDelegate(other._void_ownwerMember, other._void_method)													\
	{																																						\
		_holder = other._holder->_clone();																													\
	}																																						\
	virtual ToReturn Invoke()																																\
	{																																						\
		return _holder->Invoke();																															\
	}																																						\
	ToReturn operator()()																																	\
	{																																						\
		return _holder->Invoke();																															\
	}																																						\
	DelegateName operator= (const DelegateName& method)																										\
	{																																						\
		if(_holder != nullptr)																																\
		{																																					\
			delete _holder;																																	\
			_holder = nullptr;																																\
		}																																					\
		_holder = method._holder->_clone();																													\
		this->_void_ownwerMember = method._void_ownwerMember;																								\
		this->_void_method = method._void_method;																											\
	}																																						\
	~DelegateName()																																			\
	{																																						\
		delete _holder;																																		\
		_holder = nullptr;																																	\
	}																																						\
};																																							

//===========================================================================================================================================================
#define CREATE_DELEGATE1(ToReturn, DelegateName, Arg1)																										\
class DelegateName : public BaseDelegate																													\
{																																							\
	typedef ToReturn ( *FuncType )( Arg1 );																													\
	Holder1<ToReturn, Arg1>* _holder;																														\
public:																																						\
	template<typename Class> DelegateName(void* OwnerClass, ToReturn (Class::*Method)(Arg1)) : BaseDelegate(OwnerClass, (unsigned long long)&Method)					\
	{																																						\
		_holder = new InnerHolder1<ToReturn, Class, Arg1>((Class*)OwnerClass, Method);																		\
	}																																						\
	DelegateName(FuncType Method) : BaseDelegate(nullptr, (unsigned long long)Method)																				\
	{																																						\
		_holder = new InnerHolder1<ToReturn, Void, Arg1>((unsigned long long)Method);																				\
	}																																						\
	DelegateName(const DelegateName& other) : BaseDelegate(other._void_ownwerMember, other._void_method)													\
	{																																						\
		_holder = other._holder->_clone();																													\
	}																																						\
	virtual ToReturn Invoke(Arg1 arg1)																														\
	{																																						\
		return _holder->Invoke(arg1);																														\
	}																																						\
	ToReturn operator()(Arg1 arg1)																															\
	{																																						\
		return _holder->Invoke(arg1);																														\
	}																																						\
	DelegateName operator= (const DelegateName& method)																										\
	{																																						\
		if(_holder != nullptr)																																\
		{																																					\
			delete _holder;																																	\
			_holder = nullptr;																																\
		}																																					\
		_holder = method._holder->_clone();																													\
		this->_void_ownwerMember = method._void_ownwerMember;																								\
		this->_void_method = method._void_method;																											\
		return *this;																											\
	}																																						\
	~DelegateName()																																			\
	{																																						\
		delete _holder;																																		\
		_holder = nullptr;																																	\
	}																																						\
};																																							
//===========================================================================================================================================================
#define CREATE_DELEGATE2(ToReturn, DelegateName, Arg1, Arg2)																								\
class DelegateName : public BaseDelegate																													\
{																																							\
	typedef ToReturn ( *FuncType )(Arg1, Arg2);																												\
	Holder2<ToReturn, Arg1, Arg2>* _holder;																													\
public:																																						\
	template<typename Class> DelegateName(void* OwnerClass, ToReturn (Class::*Method)(Arg1, Arg2)) : BaseDelegate(OwnerClass, (unsigned long long)&Method)					\
	{																																						\
		_holder = new InnerHolder2<ToReturn, Class, Arg1, Arg2>((Class*)OwnerClass, Method);																\
	}																																						\
	DelegateName(FuncType Method) : BaseDelegate(nullptr, (unsigned long long)Method)																				\
	{																																						\
		_holder = new InnerHolder2<ToReturn, Void, Arg1, Arg2>((unsigned long long)Method);																		\
	}																																						\
	DelegateName(const DelegateName& other) : BaseDelegate(other._void_ownwerMember, other._void_method)													\
	{																																						\
		_holder = other._holder->_clone();																													\
	}																																						\
	virtual ToReturn Invoke(Arg1 arg1, Arg2 arg2)																											\
	{																																						\
		return _holder->Invoke(arg1, arg2);																													\
	}																																						\
	ToReturn operator()(Arg1 arg1, Arg2 arg2)																												\
	{																																						\
		return _holder->Invoke(arg1, arg2);																													\
	}																																						\
	DelegateName operator= (const DelegateName& method)																										\
	{																																						\
		if(_holder != nullptr)																																\
		{																																					\
			delete _holder;																																	\
			_holder = nullptr;																																\
		}																																					\
		_holder = method._holder->_clone();																													\
		this->_void_ownwerMember = method._void_ownwerMember;																								\
		this->_void_method = method._void_method;																											\
	}																																						\
	~DelegateName()																																			\
	{																																						\
		delete _holder;																																		\
		_holder = nullptr;																																	\
	}																																						\
};																																							


#pragma endregion

#endif