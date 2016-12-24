#ifndef INCLUDE_GUARD_winapi_1364467341
#define INCLUDE_GUARD_winapi_1364467341

#include <stdint.h>
#include <assert.h>

#include <string>

#include <windows.h>

namespace winapi {
	namespace detail {
		//this is needed because DeleteFile is a macro which will be undefd to prevent collisions
		inline BOOL Call_DeleteFile(
		  _In_  LPCTSTR lpFileName)
		{
			return DeleteFile(lpFileName);
		}
	}
}

#undef min
#undef max
#undef DeleteFile

namespace winapi
{
	bool CHECK_RESULT(bool b, const char* function);
	HANDLE CHECK_RESULT_NULL(HANDLE h, const char* function);
	HANDLE CHECK_RESULT_IHV(HANDLE h, const char* function);
	
	std::string GetLastErrorStr();

	inline bool DeleteFile( _In_  LPCTSTR lpFileName)
	{
		return CHECK_RESULT(detail::Call_DeleteFile(lpFileName) != 0, "DeleteFile");
	}

//#define LOG_FILE_OPEN_CLOSE

	class File
	{
#ifdef LOG_FILE_OPEN_CLOSE
		std::string fname;
#endif
	public:
		File()
		: h(INVALID_HANDLE_VALUE)
		{}
		
		~File()
		{
			closeHandle();
		}
		
		void moveFrom(File& y)
		{
			closeHandle();
			h = y.h;
			y.h = INVALID_HANDLE_VALUE;
#ifdef LOG_FILE_OPEN_CLOSE
			fname = y.fname;
			y.fname.clear();
#endif
		}
		
		bool createFile(
			_In_      LPCTSTR lpFileName,
			_In_      DWORD dwDesiredAccess,
			_In_      DWORD dwShareMode,
			_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			_In_      DWORD dwCreationDisposition,
			_In_      DWORD dwFlagsAndAttributes,
			_In_opt_  HANDLE hTemplateFile)
		{
			assert(!valid());
			closeHandle();
			h = CHECK_RESULT_IHV(CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile), "CreateFile");
#ifdef LOG_FILE_OPEN_CLOSE
			fname = lpFileName;
			printf("CreateFile: %s %x\n", fname.c_str(), h);
#endif
			return h != INVALID_HANDLE_VALUE;
		}
		
		bool closeHandle()
		{
			bool b(true);
			if ( valid() )
			{
				b = CHECK_RESULT(CloseHandle(h) != 0, "CloseHandle");
#ifdef LOG_FILE_OPEN_CLOSE
			printf("CloseHandle: %s %d\n", fname.c_str(), b);
#endif
				h = INVALID_HANDLE_VALUE;
			}
			return b;
		}
		
		bool setFilePointerEx(int64_t liDistanceToMove, int64_t* lpNewFilePointer, DWORD dwMoveMethod) const
		{
			assert(valid());
			LARGE_INTEGER li, nli;
			li.QuadPart = liDistanceToMove;
			bool b = CHECK_RESULT(SetFilePointerEx(h, li, lpNewFilePointer ? &nli : NULL, dwMoveMethod) != 0, "SetFilePointerEx");
			if ( lpNewFilePointer )
				*lpNewFilePointer = nli.QuadPart;
			return b;
		}
		
		bool setEndOfFile() const
		{
			assert(valid());
			return CHECK_RESULT(SetEndOfFile(h) != 0, "SetEndOfFile");
		}
		
		HANDLE handle() const
		{
			assert(valid());
			return h;
		}
		
		bool valid() const
		{
			return h != INVALID_HANDLE_VALUE;
		}

		bool readFile(
		  _Out_        LPVOID lpBuffer,
		  _In_         DWORD nNumberOfBytesToRead,
		  _Out_opt_    LPDWORD lpNumberOfBytesRead,
		  _Inout_opt_  LPOVERLAPPED lpOverlapped
		) const
		{
			assert(valid());
			return CHECK_RESULT(::ReadFile(h, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped) != 0, "ReadFile");
		}

		bool writeFile(
		  _In_         LPCVOID lpBuffer,
		  _In_         DWORD nNumberOfBytesToWrite,
		  _Out_opt_    LPDWORD lpNumberOfBytesWritten,
		  _Inout_opt_  LPOVERLAPPED lpOverlapped
		) const
		{
			assert(valid());
			return CHECK_RESULT(::WriteFile(h, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped) != 0, "WriteFile");
		}

		bool flushFileBuffers() const
		{
			return CHECK_RESULT(FlushFileBuffers(h) != 0, "FlushFileBuffers");
		}
	private:
		HANDLE h;
		
		File(const File&);
		void operator=(const File&);
	};

	class FileMapping
	{
	public:
		FileMapping()
			: h(NULL)
		{}
		~FileMapping()
		{
			closeHandle();
		}
		void moveFrom(FileMapping& y)
		{
			closeHandle();
			h = y.h;
			y.h = NULL;
		}
		bool closeHandle()
		{
			bool b(true);
			if ( valid() )
			{
				b = CHECK_RESULT(CloseHandle(h) != 0, "CloseHandle");
				h = NULL;
			}
			return b;
		}
		bool valid() const
		{
			return h != NULL;
		}
		bool createFileMapping(
			winapi::File& file,
			_In_opt_  LPSECURITY_ATTRIBUTES lpAttributes,
			_In_      DWORD flProtect,
			_In_      DWORD dwMaximumSizeHigh,
			_In_      DWORD dwMaximumSizeLow,
			_In_opt_  LPCTSTR lpName)
		{
			assert(!valid());
			closeHandle();
			h = CHECK_RESULT_NULL(::CreateFileMapping(file.handle(), lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName), "CreateFileMapping");
			return valid();
		}
		HANDLE handle() const
		{
			assert(valid());
			return h;
		}
	private:
		HANDLE h;

		FileMapping(const FileMapping&);
		void operator=(const FileMapping&);
	};

	class ViewOfFile
	{
	public:
		ViewOfFile()
			: p(NULL)
		{}
		~ViewOfFile()
		{
			unmapViewOfFile();
		}
		void moveFrom(ViewOfFile& y)
		{
			unmapViewOfFile();
			p = y.p;
			y.p = NULL;
		}
		bool mapViewOfFile(
			winapi::FileMapping& fileMapping,
			_In_  DWORD dwDesiredAccess,
			uint64_t fileOffset,
			_In_  SIZE_T dwNumberOfBytesToMap) 
		{
			assert(!valid());
			unmapViewOfFile();
			p = CHECK_RESULT_NULL(MapViewOfFile(fileMapping.handle(), dwDesiredAccess,
				DWORD(fileOffset >> 32), DWORD(fileOffset & 0xffffffff), dwNumberOfBytesToMap), "MapViewOfFile");
			return valid();
		}
		bool unmapViewOfFile()
		{
			bool b(true);
			if ( valid() )
			{
				b = CHECK_RESULT(UnmapViewOfFile(p) != 0, "UnmapViewOfFile");
				p = NULL;
			}
			return b;
		}
		bool valid() const
		{
			return p != NULL;
		}
		void* address() const
		{
			assert(valid());
			return p;
		}
		bool flushViewOfFile(
			_In_  LPCVOID lpBaseAddress,
			_In_  SIZE_T dwNumberOfBytesToFlush) const
		{
			return CHECK_RESULT(FlushViewOfFile(lpBaseAddress, dwNumberOfBytesToFlush) != 0, "FlushViewOfFile");
		}
	private:
		void* p;
	};
}





#endif

