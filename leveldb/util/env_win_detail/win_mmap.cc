#include "win_mmap.h"
#include "win_misc.h"

MMap::
	MMap()
{}

MMap::
	MMap(winapi::File& f) //moves 'f' out
{
	f_.moveFrom(f);
}

MMap::
	~MMap()
{
	close();
}

void MMap::
	init(winapi::File& f)
{
	assert(!validFile());
	assert(f.valid());
	close();
	f_.moveFrom(f);
}

bool MMap::
	close()
{
	bool b = v_.unmapViewOfFile();
	b = fm_.closeHandle() && b;
	b = f_.closeHandle() && b;
	return b;
}

bool MMap::
	mmap(size_t length, bool bAllowWrite, int64_t offset)
{
	v_.unmapViewOfFile();

	if ( !fm_.valid() )
		fm_.createFileMapping(f_, NULL, bAllowWrite ? PAGE_READWRITE : PAGE_READONLY, 0, 0, NULL);

	if ( fm_.valid() )
		v_.mapViewOfFile(fm_, bAllowWrite ? FILE_MAP_WRITE : FILE_MAP_READ, offset, length);

	return validView();
}

bool MMap::
	msync(void *addr, size_t length)
{
	return v_.flushViewOfFile(addr, length);
}

bool MMap::
	munmap()
{
	bool b1 = v_.unmapViewOfFile();
	bool b2 = fm_.closeHandle();

	return b1 && b2;
}

void MMap::
	moveFrom(MMap& y)
{
	close();
	v_.moveFrom(y.v_);
	fm_.moveFrom(y.fm_);
	f_.moveFrom(y.f_);
}


void MMap::
	releaseFileHandle(winapi::File& y)
{
	v_.unmapViewOfFile();
	fm_.closeHandle();
	y.moveFrom(f_);
}

bool MMap::
	ftruncate(int64_t size) const
{
	return ::ftruncate(f_, size);
}

bool MMap::
	flushFileBuffers() const
{
	return f_.flushFileBuffers();
}

