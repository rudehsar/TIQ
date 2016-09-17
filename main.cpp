#include "stdafx.h"

vector<PRunnable>& getRunnable()
{
    static vector<PRunnable> s_runnables;
    return s_runnables;
}

int _tmain(int argc, _TCHAR* argv[])
{
    for (auto& p : getRunnable())
        (*p)();

    return 0;
}
