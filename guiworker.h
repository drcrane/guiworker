#ifndef GUIWORKER_H
#define GUIWORKER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "workerthread.h"

class GUIWorkerForm {
public:
	GUIWorkerForm(HWND hwnd);
	~GUIWorkerForm();

	int ExecuteTaskA();
	static void * ExecuteTaskAWt(GUIWorkerForm * me, void * args);
	int ExecuteTaskB();
	static void * ExecuteTaskB(GUIWorkerForm * me, void * args);

private:
	HWND dialog_hwnd;
	workerthread_context_t * workerthread_ctx;
};

INT_PTR CALLBACK GUIWorkerFormProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // GUIWORKER_H
