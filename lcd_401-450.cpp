#include "stdafx.h"

namespace lcd8
{
    /* Frog Jump */
    // LeetCode#403
    // A frog is crossing a river. The river is divided into x units and at each unit there may or 
    // may not exist a stone. The frog can jump on a stone, but it must not jump into the water.
    // Given a list of stones' positions (in units) in sorted ascending order, determine if the frog 
    // is able to cross the river by landing on the last stone. Initially, the frog is on the first 
    // stone and assume the first jump must be 1 unit. If the frog's last jump was k units, then its 
    // next jump must be either k - 1, k, or k + 1 units. Note that the frog can only jump in the 
    // forward direction.
    struct p403
    {
        bool canCrossHelper(const vector<int>& stones, int i, int j, int k)
        {
            if (abs(stones[j] - stones[i] - k) > 1)
                return false;

            if (j == stones.size() - 1)
                return true;

            k = stones[j] - stones[i];
            return canCrossHelper(stones, j, j + 1, k) || canCrossHelper(stones, j, j + 2, k);
        }

        bool canCross_0(const vector<int>& stones)
        {
            if ((stones[1] - stones[0]) > 1)
                return false;

            return canCrossHelper(stones, 0, 1, 1);
        }

        bool canCross(const vector<int>& stones)
        {
            if ((stones[1] - stones[0]) > 1)
                return false;

            // available jump options from 2 stones back
            pair<int, int> p0 = { 0, 0 };
            // available jump options from 1 stones back
            pair<int, int> p1 = { 1, 1 };

            for (int i = 2; i < stones.size(); ++i)
            {
                pair<int, int> p3 = { INT_MAX, INT_MAX };

                int j1 = stones[i] - stones[i - 1];
                if (min(abs(j1 - p1.first), abs(j1 - p1.second)) < 2)
                    p3.first = j1;

                int j0 = stones[i] - stones[i - 2];
                if (min(abs(j0 - p0.first), abs(j0 - p0.second)) < 2)
                    p3.second = j0;

                p0 = p1;
                p1 = p3;
            }

            return min(p1.first, p1.second) < INT_MAX;
        }

        void test()
        {
            VERIFY(canCross({ 0, 1, 3, 4, 5, 7, 9, 10, 12 }));
            VERIFY(canCross({ 0, 1, 3, 5, 6, 8, 12, 17 }));
            VERIFY(!canCross({ 0, 2 }));
            VERIFY(!canCross({ 0, 1, 2, 3, 4, 8, 9, 11 }));
            VERIFY(!canCross({ 0, 1, 3, 6, 7 }));
            VERIFY(canCross({ 0, 1, 3, 6, 10, 15, 16, 21 }));
        }
    };

    static void run()
    {
        //p403().test();
    }

    //REGISTER_RUNNABLE(lcd8)
}