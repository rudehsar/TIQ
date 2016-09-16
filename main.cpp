#include "stdafx.h"

unordered_map<PExtMain, bool> g_extMains;

int _tmain(int argc, _TCHAR* argv[])
{
    for (auto p : g_extMains)
        if (p.second)
            (*p.first)();

    return 0;
}
