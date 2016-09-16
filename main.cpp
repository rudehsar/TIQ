#include "stdafx.h"

extern vector<PExtMain> g_runnables;

int _tmain(int argc, _TCHAR* argv[])
{
    //LOG("&g_runnables " << &g_runnables);
    //LOG(g_runnables.size());

    for (auto& p : g_runnables)
    {
        //LOG("Running" << p);
        (*p)();
    }

    return 0;
}
