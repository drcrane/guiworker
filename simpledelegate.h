#ifndef SIMPLEDELEGATE_H
#define SIMPLEDELEGATE_H

class DelegateInvoker;

class DelegateArguments {
public:
	DelegateArguments(DelegateInvoker* delegate_invoker) {
		this->delegate_invoker = delegate_invoker;
	}

	DelegateInvoker* GetDelegateInvoker() {
		return delegate_invoker;
	}
private:
	DelegateInvoker* delegate_invoker;
};

template<typename Param1>
class DelegateArguments1 : public DelegateArguments {
public:
	DelegateArguments1(DelegateInvoker* invoker, Param1 param1)
		: DelegateArguments(invoker)
	{
		m_param1 = param1;
	}

	Param1 GetParam1() {
		return m_param1;
	}
private:
	Param1 m_param1;
};

template<typename... Params>
class DelegateArgumentsN : public DelegateArguments {
public:
	DelegateArgumentsN(DelegateInvoker* invoker, Params... params) {

	}
};

class DelegateInvoker {
public:
	virtual void Invoke(DelegateArguments* args);
	static void * StaticInvoke(DelegateInvoker* invoker, DelegateArguments* args);
};

template<typename TClass, typename Ret, typename Param1>
class Delegate : public DelegateInvoker {
	typedef TClass* ObjectPtr;
	typedef Ret (TClass::*MemberFunc)(Param1);
	typedef Ret (TClass::*ConstMemberFunc)(Param1) const;

public:
	Delegate(ObjectPtr object, MemberFunc func) {
		Bind(object, func);
	}

	void Bind(ObjectPtr object, MemberFunc func) {
		this->m_object = object;
		this->m_func = func;
	}

	void Bind(ObjectPtr object, ConstMemberFunc func) {
		this->m_object = object;
		this->m_func = reinterpret_cast<MemberFunc>(func);
	}

	virtual Ret operator()(Param1 param1) {
		return (*m_object.*m_func)(param1);
	}

	virtual void Invoke(DelegateArguments* args) {
		DelegateArguments1<Param1>* args_1 = (DelegateArguments1<Param1>*)args;
		Param1 param1;
		param1 = args_1->GetParam1();
		(*m_object.*m_func)(param1);
	}

private:
	ObjectPtr m_object;
	MemberFunc m_func;
};

/*
template<typename return_type, typename... params>
class Delegate {
	typedef return_type(*Type)(void* callee, params...);
public:
	Delegate(void* callee, Type function)
		: fpCallee(callee)
		, fpCallbackFunction(function) {
	}

	template <class T, return_type(T::*TMethod)(params...)>
	static Delegate from_function(T* callee) {
		Delegate d(callee, &methodCaller<T, TMethod>);
		return d;
	}

	return_type operator()(params... xs) const {
		return (*fpCallbackFunction)(fpCallee, xs...);
	}

private:
	void* fpCallee;
	Type fpCallbackFunction;

	template <class T, return_type(T::*TMethod)(params...)>
	static return_type methodCaller(void* callee, params... xs) {
		T* p = static_cast<T*>(callee);
		return (p->*TMethod)(xs...);
	}
};
*/

#endif // SIMPLEDELEGATE_H
