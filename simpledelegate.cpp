#include "simpledelegate.h"

void DelegateInvoker::Invoke(DelegateArguments * args) {
	//args->GetDelegateInvoker()->Invoke(args);
	//delete args->GetDelegateInvoker();
	//delete args;
}

void * DelegateInvoker::StaticInvoke(DelegateInvoker* invoker, DelegateArguments* args) {
	invoker->Invoke(args);
	return nullptr;
}

void * DelegateArguments::StaticInvoke(DelegateArguments* args) {
	DelegateInvoker* invoker = args->GetDelegateInvoker();
	invoker->Invoke(args);
	return nullptr;
}
