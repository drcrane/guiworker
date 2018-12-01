#include "guiworker.h"
#include "resource.h"
#include "workerthread.h"
#include "simpledelegate.h"
#include <thread>
#include <functional>

#include <malloc.h>

GUIWorkerForm::GUIWorkerForm(HWND hwnd) {
	this->dialog_hwnd = hwnd;
	this->workerthread_ctx = workerthread_create();
}

int GUIWorkerForm::ExecuteTaskA() {
	int res;
	if (IsDlgButtonChecked(this->dialog_hwnd, IDC_LONGTASKARADIO) == BST_CHECKED) {
		unsigned long int * args = (unsigned long int *)malloc(sizeof(unsigned long int));
		if (args == nullptr) {
			return -1;
		}
		args[0] = 1000;
		res = workerthread_enqueue(this->workerthread_ctx, (workerthread_function_t)&GUIWorkerForm::ExecuteTaskAWt, this, (void *)args);
		if (res != 0) {
			SendDlgItemMessageW(this->dialog_hwnd, IDC_TASKASTATUSSTATIC, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Error Queueing");
			free(args);
			return -1;
		}
		SendDlgItemMessageW(this->dialog_hwnd, IDC_TASKASTATUSSTATIC, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Running Long Task...");
		EnableWindow(GetDlgItem(this->dialog_hwnd, IDC_EXECUTETASKABUTTON), FALSE);
	} else
	if (IsDlgButtonChecked(this->dialog_hwnd, IDC_LONGERTASKARADIO) == BST_CHECKED) {
		unsigned long int * args = (unsigned long int *)malloc(sizeof(unsigned long int));
		if (args == nullptr) {
			return -1;
		}
		args[0] = 5000;
		res = workerthread_enqueue(this->workerthread_ctx, (workerthread_function_t)&GUIWorkerForm::ExecuteTaskAWt, this, (void *)args);
		if (res != 0) {
			SendDlgItemMessageW(this->dialog_hwnd, IDC_TASKASTATUSSTATIC, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Error Queueing");
			free(args);
			return -1;
		}
		SendDlgItemMessageW(this->dialog_hwnd, IDC_TASKASTATUSSTATIC, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Running Longer Task...");
		EnableWindow(GetDlgItem(this->dialog_hwnd, IDC_EXECUTETASKABUTTON), FALSE);
	}
	return 0;
}

void * GUIWorkerForm::ExecuteTaskAWt(GUIWorkerForm * me, void * args) {
	unsigned long int * arg = (unsigned long int *)args;
	Sleep(*arg);
	SendDlgItemMessageW(me->dialog_hwnd, IDC_TASKASTATUSSTATIC, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Finished");
	EnableWindow(GetDlgItem(me->dialog_hwnd, IDC_EXECUTETASKABUTTON), TRUE);
	free(args);
	return nullptr;
}

#if 1
int GUIWorkerForm::ExecuteTaskB() {
	SendDlgItemMessageW(this->dialog_hwnd, IDC_TASKBSTATUSSTATIC, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Running Longer Task...");
	EnableWindow(GetDlgItem(this->dialog_hwnd, IDC_EXECUTETASKBBUTTON), FALSE);
	
	std::string* param = new std::string("Welcome");
	//DelegateArguments* deleg = CreateInvokableDelegate(this, &GUIWorkerForm::ExecuteTaskBWt, param);
	//workerthread_enqueue(this->workerthread_ctx, (workerthread_function_t)&DelegateArguments::StaticInvoke, (void *)deleg, (void *)nullptr);
	
	InvokeInWorkerThread(this->workerthread_ctx, this, &GUIWorkerForm::ExecuteTaskBWt, param);
	return 0;
}
#endif

#if 0
int GUIWorkerForm::ExecuteTaskB() {
	SendDlgItemMessageW(this->dialog_hwnd, IDC_TASKBSTATUSSTATIC, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Running Longer Task...");
	EnableWindow(GetDlgItem(this->dialog_hwnd, IDC_EXECUTETASKBBUTTON), FALSE);

	Delegate1<GUIWorkerForm, void *, std::string *>* deleg = new Delegate1<GUIWorkerForm, void *, std::string *>(this, &GUIWorkerForm::ExecuteTaskBWt);
	std::string * string = new std::string("Welcomeee");
	DelegateArguments1<std::string *>* deleg_arg = new DelegateArguments1<std::string *>(deleg, string);
	
	// Invoke from this thread... somewhat defeats the object!
	//deleg_arg->GetDelegateInvoker()->Invoke(reinterpret_cast<DelegateArguments*>(deleg_arg));
	//delete string;
	
	DelegateInvoker* deleg_inv = deleg_arg->GetDelegateInvoker();
	DelegateArguments* deleg_arg2 = deleg_arg;

	int res;
	res = workerthread_enqueue(this->workerthread_ctx, this, (workerthread_function_t)&DelegateInvoker::StaticInvoke, (void *)deleg_inv, (void *)deleg_arg2);

	//std::thread my_thread = std::thread(&DelegateInvoker::Invoke, deleg_inv, deleg_arg);
	//my_thread.detach();

	return 0;
}
#endif

void * GUIWorkerForm::ExecuteTaskBWt(std::string* args) {
	Sleep(1000);
	SendDlgItemMessageA(this->dialog_hwnd, IDC_TASKBSTATUSSTATIC, WM_SETTEXT, (WPARAM)0, (LPARAM)(*args).c_str());
	EnableWindow(GetDlgItem(this->dialog_hwnd, IDC_EXECUTETASKBBUTTON), TRUE);
	delete args;
	return NULL;
}

GUIWorkerForm::~GUIWorkerForm() {
	workerthread_shutdown(this->workerthread_ctx);
}

GUIWorkerForm * guiworkerform;

INT_PTR CALLBACK GUIWorkerFormProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		guiworkerform = new GUIWorkerForm(hwndDlg);
		break;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_EXECUTETASKABUTTON:
			guiworkerform->ExecuteTaskA();
			break;
		case IDC_EXECUTETASKBBUTTON:
			guiworkerform->ExecuteTaskB();
			break;
		default:
			break;
		}
		break;
	}
	return (INT_PTR)NULL;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_GUIWORKERFORM), NULL, &GUIWorkerFormProc);
	return 0;
}
