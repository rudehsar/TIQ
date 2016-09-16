// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Warning	C4800 'int': forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable:4800)

// warning C4018: '<': signed/unsigned mismatch
#pragma warning(disable:4018)

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include <algorithm>
#include <condition_variable>
#include <iterator>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <numeric>
#include <queue>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <assert.h>

#include "Utility.h"
#include "Common.h"

using namespace std;
using namespace utl;
using namespace cmn;

typedef void(*PExtMain)();
__declspec(selectany) vector<PExtMain> g_runnables;

#define REGISTER_RUNNABLE(x) \
    static void run(); \
    void x ## () { run(); } \
    class __register_ ## x \
    { \
    public: \
        __register_ ## x() \
        { \
            g_runnables.push_back(x); \
        } \
    }; \
    static __register_ ## x __register_ ## x ## _; \
            //LOG(#x); \
            //LOG("&g_runnables " << &g_runnables); \
            //LOG("added " << x); \
            //LOG(g_runnables.size()); \

#define LOG(x) std::cout << x << std::endl

#define VERIFY(x) do { \
        if (x) \
            LOG("verification passed (" ## #x ## ")"); \
        else \
            LOG("verification FAILED (" ## #x ## ")"); \
    } while (0)
