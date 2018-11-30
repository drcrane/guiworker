#ifndef GUIWORKER_H
#define GUIWORKER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "workerthread.h"
#include <string>

template<typename T, typename Q, typename R, typename... Parameters>
class InnerDelegate : Delegate {
private:
	T object;
	R(Q::*method)(Parameters...);

public:
	InnerDelegate(T object, R(Q::*method)(Parameters...)) : object(object), method(method) {

	}

	R Call(Parameters... parameters) {
		return ((*object).*method)(params...);
	}
};

/*
template <typename T, typename Q, typename R, typename... Parameters>
InnerDelegate * CreateDelegate(T object, R(Q::*method)(Parameters...)) {
	return new InnerDelegate<T, Q, R, Parameters...>(object, method);
	//return id;
}
*/

class DelegateER
{
private:
	//T object;
	//R(Q::*method)(Parameters... params);

public:
	//template <typename T, typename Q, typename R, typename... Parameters>
	//Delegate(T object, R(Q::*method)(Parameters...))
		//: object(object), method(method)
	//{
	//	InnerDelegate<T, Q, R, Parameters...> id(object, method);
	//}

	/*
	R Call(Parameters... params) {
		size_t paramsSz = sizeof...(Parameters);
		OutputDebugStringA(std::string(std::to_string(paramsSz) + "\r\n").c_str());

		return ((*object).*method)(params...);
		// return (object->*method)(params...);
	}
	*/
};

class GUIWorkerForm {
public:
	GUIWorkerForm(HWND hwnd);
	~GUIWorkerForm();

	int ExecuteTaskA();
	static void * ExecuteTaskAWt(GUIWorkerForm * me, void * args);
	int ExecuteTaskB();
	//static void * ExecuteTaskB(GUIWorkerForm * me, void * args);
	void * ExecuteTaskBWt(std::string* args);

private:
	HWND dialog_hwnd;
	workerthread_context_t * workerthread_ctx;
};

INT_PTR CALLBACK GUIWorkerFormProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // GUIWORKER_H
