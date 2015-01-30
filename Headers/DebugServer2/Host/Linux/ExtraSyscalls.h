//
// Copyright (c) 2014, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_Host_Linux_ExtraSyscalls_h
#define __DebugServer2_Host_Linux_ExtraSyscalls_h

#include <fcntl.h>
// Older android native sysroots don't have sys/personality.h.
#if defined(HAVE_SYS_PERSONALITY_H)
#include <sys/personality.h>
#else
#include <linux/personality.h>
#undef personality
#endif
#include <sys/syscall.h>
#include <unistd.h>

#if !defined(HAVE_GETTID)
static inline pid_t gettid() { return ::syscall(__NR_gettid); }
#endif

#if !defined(HAVE_PERSONALITY)
static inline int personality(unsigned long persona) {
  return ::syscall(__NR_personality, persona);
}
#endif

#if !defined(HAVE_POSIX_OPENPT)
static inline int posix_openpt(int flags) {
  return ::open("/dev/ptmx", flags);
}
#endif

// No glibc wrapper for tgkill
static inline int tgkill(pid_t pid, pid_t tid, int signo) {
  return ::syscall(__NR_tgkill, pid, tid, signo);
}

// No glibc wrapper for tkill
static inline int tkill(pid_t tid, int signo) {
  return ::syscall(__NR_tkill, tid, signo);
}

#if !defined(HAVE_WAIT4)
static inline pid_t wait4(pid_t pid, int *stat_loc, int options,
                          struct rusage *rusage) {
  return ::syscall(__NR_wait4, pid, stat_loc, options, rusage);
}
#endif

#if defined(__i386__) && !defined(__ANDROID__)
#include <sys/user.h>
typedef struct user_fxsr_struct user_fpxregs_struct;
#endif

#endif // !__DebugServer2_Host_Linux_ExtraSyscalls_h
