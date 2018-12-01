#ifndef GUIWORKER_H
#define GUIWORKER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "workerthread.h"
#include "simpledelegate.h"
#include <string>

template<class TClass, typename Ret, typename... Params>
int InvokeInWorkerThread(workerthread_context_t * wt_ctx, TClass* object, Ret (TClass::*func)(Params...), Params... params) {
	DelegateArguments* deleg = CreateInvokableDelegate(object, func, std::forward<Params>(params)...);
	int res = workerthread_enqueue(wt_ctx, (workerthread_function_t)&DelegateArguments::StaticInvoke, (void *)deleg, (void *)nullptr);
	if (res != 0) {
		delete deleg;
	}
	return res;
}

class GUIWorkerForm {
public:
	GUIWorkerForm(HWND hwnd);
	~GUIWorkerForm();

	int ExecuteTaskA();
	static void * ExecuteTaskAWt(GUIWorkerForm * me, void * args);
	int ExecuteTaskB();
	//static void * ExecuteTaskB(GUIWorkerForm * me, void * args);
	void * ExecuteTaskBWt(unsigned long int delay);

private:
	HWND dialog_hwnd;
	workerthread_context_t * workerthread_ctx;
};

INT_PTR CALLBACK GUIWorkerFormProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // GUIWORKER_H
