#include "simpledelegate.h"

void DelegateInvoker::Invoke(DelegateArguments * args) {
	// Default Implementation for Virtual Function
	// Actually Delegate0::Invoke should be called...
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
