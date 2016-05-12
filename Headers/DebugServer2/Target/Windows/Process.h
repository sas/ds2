//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_Target_Windows_Process_h
#define __DebugServer2_Target_Windows_Process_h

#include "DebugServer2/Host/ProcessSpawner.h"
#include "DebugServer2/Target/ProcessBase.h"
#include "DebugServer2/Target/Thread.h"
#include "DebugServer2/Utils/Log.h"

namespace ds2 {
namespace Target {
namespace Windows {

class Process : public ds2::Target::ProcessBase {
protected:
  HANDLE _handle;

public:
  struct PendingEvent {
  private:
    bool _valid;
    ThreadId _tid;

    PendingEvent() : _valid(false), _tid(0) {}

  public:
    inline void set(Thread *thread) {
      DS2ASSERT(!_valid);
      _valid = true;
      _tid = thread->tid();
      thread->suspend();
    }

    inline void reset() {
      DS2ASSERT(_valid);
      _valid = false;
      _tid = 0;
    }
  } _pendingEvent;

  PendingEvent &pendingEvent() { return _pendingEvent; }

protected:
  Process();

public:
  ~Process() override;

public:
  inline HANDLE handle() const { return _handle; }

protected:
  ErrorCode initialize(ProcessId pid, uint32_t flags) override;

public:
  ErrorCode detach() override;
  ErrorCode interrupt() override;
  ErrorCode terminate() override;
  bool isAlive() const override;

public:
  ErrorCode readString(Address const &address, std::string &str, size_t length,
                       size_t *nread = nullptr) override;
  ErrorCode readMemory(Address const &address, void *data, size_t length,
                       size_t *nread = nullptr) override;
  ErrorCode writeMemory(Address const &address, void const *data, size_t length,
                        size_t *nwritten = nullptr) override;

public:
  ErrorCode getMemoryRegionInfo(Address const &address,
                                MemoryRegionInfo &info) override {
    return kErrorUnsupported;
  }

public:
  ErrorCode updateInfo() override;

public:
  ErrorCode allocateMemory(size_t size, uint32_t protection,
                           uint64_t *address) override;
  ErrorCode deallocateMemory(uint64_t address, size_t size) override;

public:
  ErrorCode wait() override;

public:
  static Target::Process *Create(Host::ProcessSpawner &spawner);
  static Target::Process *Attach(ProcessId pid);

public:
  ErrorCode enumerateSharedLibraries(
      std::function<void(SharedLibraryInfo const &)> const &cb);
};
}
}
}

#endif // !__DebugServer2_Target_Windows_Process_h
