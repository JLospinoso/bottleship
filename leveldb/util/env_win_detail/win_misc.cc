#include "win_misc.h"

void usleep(int usec)
{
	Sleep((usec + 999)/1000);
}

int getpagesize(void)
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}

bool ftruncate(const winapi::File& fd, int64_t length)
{
	bool b = fd.setFilePointerEx(length, NULL, FILE_BEGIN);
	if ( !b ) return false;
	return fd.setEndOfFile();
}




