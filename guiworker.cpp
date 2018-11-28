#include "guiworker.h"
#include "resource.h"
#include "workerthread.h"

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
			break;
		default:
			break;
		}
		break;
	}
	return NULL;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_GUIWORKERFORM), NULL, &GUIWorkerFormProc);
}
