#include "stdafx.h"

#define EXTMAIN(ns) namespace ns { extern void main(); }

EXTMAIN(lcd1)
EXTMAIN(oth1)
EXTMAIN(src1)
EXTMAIN(src2)
EXTMAIN(src3)
EXTMAIN(src4)
EXTMAIN(src5)

typedef void(*PExtMain)();
unordered_map<PExtMain, bool> g_extMains = {
    { lcd1::main, 1 },
    { oth1::main, 0 },
    { src1::main, 0 },
    { src2::main, 0 },
    { src3::main, 0 },
    { src4::main, 0 },
    { src5::main, 0 },
};

int _tmain(int argc, _TCHAR* argv[])
{
    for (auto p : g_extMains)
        if (p.second)
            (*p.first)();

    return 0;
}
