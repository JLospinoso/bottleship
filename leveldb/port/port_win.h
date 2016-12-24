// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// This file contains the specification, but not the implementations,
// of the types/operations/etc. that should be defined by a platform
// specific port_<platform>.h file.  Use this file as a reference for
// how to port this package to a new platform.

#ifndef STORAGE_LEVELDB_PORT_PORT_EXAMPLE_H_
#define STORAGE_LEVELDB_PORT_PORT_EXAMPLE_H_

#include <stdio.h>
#include <string>
#include <stdint.h>

#ifdef SNAPPY
#include <snappy.h>
#endif

#define snprintf _snprintf

typedef ptrdiff_t ssize_t;

namespace leveldb {
namespace port {

// TODO(jorlow): Many of these belong more in the environment class rather than
//               here. We should try moving them and see if it affects perf.

// The following boolean constant must be true on a little-endian machine
// and false otherwise.
static const bool kLittleEndian = true;

// ------------------ Threading -------------------

// A Mutex represents an exclusive lock.
class Mutex {
 public:
  Mutex();
  ~Mutex();

  // Lock the mutex.  Waits until other lockers have exited.
  // Will deadlock if the mutex is already locked by this thread.
  void Lock();

  // Unlock the mutex.
  // REQUIRES: This mutex was locked by this thread.
  void Unlock();

  // Optionally crash if this thread does not hold this mutex.
  // The implementation must be fast, especially if NDEBUG is
  // defined.  The implementation is allowed to skip all checks.
  void AssertHeld()
#ifdef NDEBUG
  {}
#else
  ;
#endif

 private:

  friend class CondVar;

  static const int SIZEOF_CRITICAL_SECTION =
#ifndef _WIN64
	  24;
#else
	  40;
#endif

  char cs_[SIZEOF_CRITICAL_SECTION];

#ifndef NDEBUG
  uint32_t holding_thread_id_; 
#endif

  // No copying
  Mutex(const Mutex&);
  void operator=(const Mutex&);
};

class CondVar {
 public:
  explicit CondVar(Mutex* mu);
  ~CondVar();

  // Atomically release *mu and block on this condition variable until
  // either a call to SignalAll(), or a call to Signal() that picks
  // this thread to wakeup.
  // REQUIRES: this thread holds *mu
  void Wait();

  // If there are some threads waiting, wake up at least one of them.
  void Signal();

  // Wake up all waiting threads.
  void SignalAll();
private:
  static const int SIZEOF_CONDITION_VARIABLE = 4;
  char cv_[SIZEOF_CONDITION_VARIABLE];
  Mutex* mu_;
};

// Thread-safe initialization.
// Used as follows:
//      static port::OnceType init_control = LEVELDB_ONCE_INIT;
//      static void Initializer() { ... do something ...; }
//      ...
//      port::InitOnce(&init_control, &Initializer);

union OnceType { //this must be the same as INIT_ONCE
    void* Ptr;                      
};

#define LEVELDB_ONCE_INIT {0} //this must be the same as INIT_ONCE_STATIC_INIT

extern void InitOnce(port::OnceType*, void (*initializer)());

// A type that holds a pointer that can be read or written atomically
// (i.e., without word-tearing.)
class AtomicPointer {
 private:
  typedef void* pvoid_t;
  volatile pvoid_t rep_;
 public:
  // Initialize to arbitrary value
  AtomicPointer() {}

  // Initialize to hold v
  explicit AtomicPointer(void* v) : rep_(v) { }

  // Read and return the stored pointer with the guarantee that no
  // later memory access (read or write) by this thread can be
  // reordered ahead of this read.
  void* Acquire_Load() const;

  // Set v as the stored pointer with the guarantee that no earlier
  // memory access (read or write) by this thread can be reordered
  // after this store.
  void Release_Store(void* v);

  // Read the stored pointer with no ordering guarantees.
  void* NoBarrier_Load() const
  {
	  static_assert(sizeof(rep_) <= 8, "For 128-bit pointers please update AtomicPointer::NoBarrier_Load / NoBarrier_Store");
	  return rep_; //32/64 bit reads guaranteed to be atomic on windows
  }

  // Set va as the stored pointer with no ordering guarantees.
  void NoBarrier_Store(void* v)
  {
	  rep_ = v; //32/64 bit reads guaranteed to be atomic on windows
  }
};

// ------------------ Compression -------------------

// Store the snappy compression of "input[0,input_length-1]" in *output.
// Returns false if snappy is not supported by this port.
inline bool Snappy_Compress(const char* input, size_t length,
                            std::string* output)
{
#ifdef SNAPPY
  output->resize(snappy::MaxCompressedLength(length));
  size_t outlen;
  snappy::RawCompress(input, length, &(*output)[0], &outlen);
  output->resize(outlen);
  return true;
#else
  return false;
#endif
}

// If input[0,input_length-1] looks like a valid snappy compressed
// buffer, store the size of the uncompressed data in *result and
// return true.  Else return false.
inline bool Snappy_GetUncompressedLength(const char* input, size_t length,
                                         size_t* result)
{
#ifdef SNAPPY
  return snappy::GetUncompressedLength(input, length, result);
#else
  return false;
#endif
}

// Attempt to snappy uncompress input[0,input_length-1] into *output.
// Returns true if successful, false if the input is invalid lightweight
// compressed data.
//
// REQUIRES: at least the first "n" bytes of output[] must be writable
// where "n" is the result of a successful call to
// Snappy_GetUncompressedLength.
inline bool Snappy_Uncompress(const char* input, size_t length,
                              char* output)
{
#ifdef SNAPPY
  return snappy::RawUncompress(input, length, output);
#else
  return false;
#endif
}

#if 0

// ------------------ Miscellaneous -------------------

// If heap profiling is not supported, returns false.
// Else repeatedly calls (*func)(arg, data, n) and then returns true.
// The concatenation of all "data[0,n-1]" fragments is the heap profile.
extern bool GetHeapProfile(void (*func)(void*, const char*, int), void* arg);

#endif

}  // namespace port
}  // namespace leveldb


#endif  // STORAGE_LEVELDB_PORT_PORT_EXAMPLE_H_
