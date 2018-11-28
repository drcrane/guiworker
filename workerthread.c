#include "workerthread.h"
#include <string.h>
#include <malloc.h>

static void * workexecutor(void * argument) {
	workerthread_context_t * ctx;
	HRESULT hres;
	ctx = (workerthread_context_t *)argument;
	while (ctx->keep_going) {
		if (ctx->current_function != NULL) {
			ctx->current_function((void *)ctx->self, (void *)ctx->argument);
			ctx->current_function = NULL;
		}
		hres = WaitForSingleObject(ctx->event, 5000);
		if (hres == WAIT_TIMEOUT) {
			//
		} else
		if (hres == WAIT_OBJECT_0) {
			//
		}
	}
	return NULL;
}

workerthread_context_t * workerthread_create() {
	workerthread_context_t * ctx;
	ctx = (workerthread_context_t *)malloc(sizeof(workerthread_context_t));
	if (ctx == NULL) {
		return NULL;
	}
	ctx->keep_going = 1;
	ctx->current_function = NULL;
	ctx->event = CreateEvent(NULL, FALSE, FALSE, NULL);
	ctx->thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)workexecutor, (LPVOID)ctx, 0, NULL);
	return ctx;
}

int workerthread_enqueue(workerthread_context_t * ctx, void * (*to_be_queued)(void *, void *), void * self, void * argument) {
	if (ctx->current_function == NULL) {
		ctx->self = (volatile void *)self;
		ctx->argument = (volatile void *)argument;
		ctx->current_function = (volatile void * (*)(void *, void *))to_be_queued;
		SetEvent(ctx->event);
		return 0;
	}
	return -1;
}

int workerthread_shutdown(workerthread_context_t * ctx) {
	ctx->keep_going = 0;
	SetEvent(ctx->event);
	WaitForSingleObject(ctx->thread, INFINITE);
	CloseHandle(ctx->event);
	CloseHandle(ctx->thread);
	return 0;
}
