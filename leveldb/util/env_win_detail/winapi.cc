#include "winapi.h"

namespace winapi {

const bool bLogErrors = false;

bool CHECK_RESULT(bool b, const char* function)
{
	if ( !b && bLogErrors)
		fprintf(stderr, "Function %s returned winapi error: %s", function, GetLastErrorStr().c_str());

	return b;
}

HANDLE CHECK_RESULT_NULL(HANDLE h, const char* function)
{
	if ( h == NULL && bLogErrors)
		fprintf(stderr, "Function %s returned winapi error: %s", function, GetLastErrorStr().c_str());

	return h;
}

HANDLE CHECK_RESULT_IHV(HANDLE h, const char* function)
{
	if ( h == INVALID_HANDLE_VALUE && bLogErrors)
		fprintf(stderr, "Function %s returned winapi error: %s", function, GetLastErrorStr().c_str());

	return h;
}

std::string GetLastErrorStr()
{
	DWORD le = ::GetLastError();
	LPVOID lpMsgBuf;
	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		le,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	std::string errorMsg((char*)lpMsgBuf);

	::LocalFree(lpMsgBuf);

	return errorMsg;
}

}
