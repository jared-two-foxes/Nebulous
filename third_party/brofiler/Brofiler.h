// Stub Brofiler.h — brofiler profiler is not available in this build.
// The original engine included this for profiling, but no .cpp files
// actually use any Brofiler macros. This stub is provided to satisfy
// the #include directive in Dependencies.h.
#ifndef __BROFILER_STUB_H__
#define __BROFILER_STUB_H__

// Stub macros for Brofiler profiler API
#define BROFILER_FRAME(threadName)
#define BROFILER_CATEGORY(name, color)
#define BROFILER_EVENT(label, category)
#define BROFILER_EVENT_DURATION(label, category, duration)

// Generic profiling macro used in DebugUtil.cpp
#define PROFILE

#endif // __BROFILER_STUB_H__
