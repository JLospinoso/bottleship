#ifndef INCLUDE_GUARD_mmap_1364388458
#define INCLUDE_GUARD_mmap_1364388458

#include <stdint.h>

#include "winapi.h"

class MMap
{
public:
	MMap();
	MMap(winapi::File& f); //moves 'f' out
	~MMap();

	void init(winapi::File& f); //moves 'f' out
	bool close();

	bool mmap(size_t length, bool bAllowWrite, int64_t offset);
	bool munmap();
	bool msync(void *addr, size_t length);

	void moveFrom(MMap& y);

	void* address() const { return v_.address(); }
	bool validFile() const { return f_.valid(); }
	bool validFileMapping() const { return fm_.valid(); }
	bool validView() const { return v_.valid(); }

	void releaseFileHandle(winapi::File& f); //closes file mapping and view objects
	bool ftruncate(int64_t size) const;
	bool flushFileBuffers() const;
private:
	bool closeView();

	MMap(const MMap&);
	void operator=(const MMap&);

	winapi::File f_;
	winapi::FileMapping fm_;
	winapi::ViewOfFile v_;
};

#endif
