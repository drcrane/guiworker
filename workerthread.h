#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void * (*workerthread_function_t)(void *, void *);

typedef struct workerthread_context {
	HANDLE thread;
	HANDLE event;
	volatile int keep_going;
	volatile void * argument;
	volatile void * self;
	volatile void * (*current_function)(void * self, void * arg);
} workerthread_context_t;

workerthread_context_t * workerthread_create();
int workerthread_enqueue(workerthread_context_t * ctx, void * (*current_function)(void *, void *), void * self, void * argument);
int workerthread_shutdown(workerthread_context_t * ctx);

#ifdef __cplusplus
};
#endif

#endif // WORKERTHREAD_H
