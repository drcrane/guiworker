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
	
	static void * StaticInvoke(DelegateArguments* args);
private:
	DelegateInvoker* delegate_invoker;
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

template<typename TClass, typename Ret>
class Delegate0 : public DelegateInvoker {
	typedef TClass* ObjectPtr;
	typedef Ret(TClass::*MemberFunc)();
	typedef Ret(TClass::*ConstMemberFunc)() const;

public:
	Delegate0(ObjectPtr object, MemberFunc func) {
		Bind(object, func);
	}

	Delegate0(ObjectPtr object, ConstMemberFunc func) {
		Bind(object, reinterpret_cast<MemberFunc>(func));
	}

	void Bind(ObjectPtr object, MemberFunc func) {
		this->m_object = object;
		this->m_func = func;
	}

	virtual void Invoke(DelegateArguments* args_ptr) {
		DelegateArguments* args = (DelegateArguments*)args_ptr;
		(*m_object.*m_func)();
		delete args;
		delete this;
	}
private:
	ObjectPtr m_object;
	MemberFunc m_func;
};

template<typename Param1>
class DelegateArguments1 : public DelegateArguments {
public:
	DelegateArguments1(DelegateInvoker* invoker, Param1 param1)
		: DelegateArguments(invoker) {
		m_param1 = param1;
	}

	Param1 GetParam1() {
		return m_param1;
	}
private:
	Param1 m_param1;
};

template<typename TClass, typename Ret, typename Param1>
class Delegate1 : public DelegateInvoker {
	typedef TClass* ObjectPtr;
	typedef Ret(TClass::*MemberFunc)(Param1);
	typedef Ret(TClass::*ConstMemberFunc)(Param1) const;

public:
	Delegate1(ObjectPtr object, MemberFunc func) {
		Bind(object, func);
	}

	void Bind(ObjectPtr object, MemberFunc func) {
		this->m_object = object;
		this->m_func = func;
	}

	virtual Ret operator()(Param1 param1) {
		return (*m_object.*m_func)(param1);
	}

	virtual void Invoke(DelegateArguments* args_ptr) {
		DelegateArguments1<Param1>* args = (DelegateArguments1<Param1>*)args_ptr;
		Param1 param1;
		param1 = args->GetParam1();
		(*m_object.*m_func)(param1);
		delete args;
		delete this;
	}
private:
	ObjectPtr m_object;
	MemberFunc m_func;
};

template<typename Param1, typename Param2>
class DelegateArguments2 : public DelegateArguments {
public:
	DelegateArguments2(DelegateInvoker* invoker, Param1 param1, Param2 param2)
		: DelegateArguments(invoker) {
		m_param1 = param1;
		m_param2 = param2;
	}

	Param1 GetParam1() {
		return m_param1;
	}
	Param2 GetParam2() {
		return m_param2;
	}
private:
	Param1 m_param1;
	Param2 m_param2;
};

template<typename TClass, typename Ret, typename Param1, typename Param2>
class Delegate2 : public DelegateInvoker {
	typedef TClass* ObjectPtr;
	typedef Ret(TClass::*MemberFunc)(Param1, Param2);
	typedef Ret(TClass::*ConstMemberFunc)(Param1, Param2) const;

public:
	Delegate2(ObjectPtr object, MemberFunc func) {
		Bind(object, func);
	}

	void Bind(ObjectPtr object, MemberFunc func) {
		this->m_object = object;
		this->m_func = func;
	}

	virtual Ret operator()(Param1 param1, Param2 param2) {
		return (*m_object.*m_func)(param1, param2);
	}

	virtual void Invoke(DelegateArguments* args_ptr) {
		DelegateArguments2<Param1, Param2>* args = (DelegateArguments2<Param1, Param2>*)args_ptr;
		Param1 param1;
		Param2 param2;
		param1 = args->GetParam1();
		param2 = args->GetParam2();
		(*m_object.*m_func)(param1, param2);
		delete args;
		delete this;
	}
private:
	ObjectPtr m_object;
	MemberFunc m_func;
};

template<class TClass, typename Ret>
DelegateArguments* CreateInvokableDelegate(TClass* object, Ret(TClass::*func)()) {
	Delegate0<TClass, Ret>* deleg = new Delegate0<TClass, Ret>(object, func);
	DelegateArguments* deleg_arg = new DelegateArguments(deleg);
	return deleg_arg;
}

template<class TClass, typename Ret, typename Param1>
DelegateArguments1<Param1>* CreateInvokableDelegate(TClass* object, Ret (TClass::*func)(Param1), Param1 param1) {
	Delegate1<TClass, Ret, Param1>* deleg = new Delegate1<TClass, Ret, Param1>(object, func);
	DelegateArguments1<Param1>* deleg_arg = new DelegateArguments1<Param1>(deleg, param1);
	return deleg_arg;
}

template<class TClass, typename Ret, typename Param1, typename Param2>
DelegateArguments2<Param1, Param2>* CreateInvokableDelegate(TClass* object, Ret (TClass::*func)(Param1), Param1 param1, Param2 param2) {
	Delegate2<TClass, Ret, Param1, Param2>* deleg = new Delegate2<TClass, Ret, Param1, Param2>(object, func);
	DelegateArguments2<Param1, Param2>* deleg_arg = new DelegateArguments2<Param1, Param2>(deleg, param1, param2);
	return deleg_arg;
}

/*
template<class TClass, typename Ret, typename... Params>
DelegateArguments* CreateInvokableDelegate(TClass* object, Ret (TClass::*func)(Params...), Params... params) {
	return CreateInvokableDelegate(object, func, std::forward<Params>(params)...);
}
*/

// This is great but will not work for async messaging :-(
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
