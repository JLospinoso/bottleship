#include "win_thread.h"
#include "win_misc.h"

struct CallerThreadProcParams
{
	void *(*start_routine) (void *);
	void *arg;
};

DWORD WINAPI WinThreadProc(LPVOID p)
{
	CallerThreadProcParams* tp = (CallerThreadProcParams*)p;
	CallerThreadProcParams lp(*tp);
	delete tp;
	tp = 0;
	void* result = lp.start_routine(lp.arg);
	//result is lost (can't return pointer in DWORD)
	return 0;
}

bool winthread_create(winthread_t *thread, void *(*start_routine) (void *), void *arg)
{
	CallerThreadProcParams* p = new CallerThreadProcParams();
	p->start_routine = start_routine;
	p->arg = arg;

	static_assert(sizeof(*thread) == sizeof(DWORD), "Set winthread_t to DWORD");

	HANDLE handle = winapi::CHECK_RESULT_NULL(CreateThread(
		NULL,
		0,
		WinThreadProc,
		p,
		0,
		(LPDWORD)(thread)
	), "CreateThread");

	return handle != NULL;
}

winthread_t winthread_self()
{
	return GetCurrentThreadId();
}