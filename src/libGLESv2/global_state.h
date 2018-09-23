//
// Copyright(c) 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// global_state.h : Defines functions for querying the thread-local GL and EGL state.

#ifndef LIBGLESV2_GLOBALSTATE_H_
#define LIBGLESV2_GLOBALSTATE_H_

#include "common/tls.h"
#include "libANGLE/Thread.h"
#include "libANGLE/features.h"

#include <mutex>

namespace egl
{
class Debug;
class Thread;

extern TLSIndex threadTLS;
Thread *GetCurrentThreadInit();
Debug *GetDebug();

ANGLE_INLINE Thread *GetCurrentThread()
{
    Thread *current = nullptr;
    if (threadTLS != TLS_INVALID_INDEX)
        current = static_cast<Thread *>(TlsGetValue(threadTLS)); // super hack for perf testing. I didn't spent the time to make GetTLSValue inlineable.
        //current = static_cast<Thread *>(GetTLSValue(threadTLS));

    return (current ? current : GetCurrentThreadInit());
}

}  // namespace egl

namespace gl
{
class Context;

Context *GetGlobalContext();

ANGLE_INLINE Context *GetValidGlobalContext()
{
    egl::Thread *thread = egl::GetCurrentThread();
    return thread->getValidContext();
}

}  // namespace gl

#if ANGLE_FORCE_THREAD_SAFETY == ANGLE_ENABLED
namespace angle
{
std::mutex &GetGlobalMutex();
}  // namespace angle

#define ANGLE_SCOPED_GLOBAL_LOCK() \
    std::lock_guard<std::mutex> globalMutexLock(angle::GetGlobalMutex())
#else
#define ANGLE_SCOPED_GLOBAL_LOCK()
#endif

#endif  // LIBGLESV2_GLOBALSTATE_H_
