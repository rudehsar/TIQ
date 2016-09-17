#include "stdafx.h"

namespace lcd8
{
    /* Data Stream as Disjoint Intervals */
    // LeetCode#352
    //Given a data stream input of non - negative integers a1, a2, ..., an, ..., summarize the numbers
    //seen so far as a list of disjoint intervals. For example, suppose the integers from the data
    //stream are 1, 3, 7, 2, 6, ..., then the summary will be :
    //[1, 1]
    //[1, 1], [3, 3]
    //[1, 1], [3, 3], [7, 7]
    //[1, 3], [7, 7]
    //[1, 3], [6, 7]
    struct p352
    {
        bool treeIsValid(vector<pair<int, int>>& bt, int n)
        {
            return (n < bt.size()) && (bt[n].first > INT_MIN);
        }

        bool treeInsert(vector<pair<int, int>>& bt, int value, int& n)
        {
            n = 0;
            if (bt.empty())
            {
                bt.emplace_back(value, value);
                return true;
            }

            while (treeIsValid(bt, n))
            {
                if ((bt[n].first <= value) && (bt[n].second >= value))
                    return false;

                if (bt[n].first == value + 1)
                {
                    bt[n].first = value;
                    return false;
                }

                if (bt[n].second == value - 1)
                {
                    bt[n].second = value;
                    return false;
                }

                if (bt[n].first > value)
                    n = 2 * n + 1;
                else
                    n = 2 * n + 2;
            }

            if (bt.size() < (n + 1))
                bt.resize(n + 1, { INT_MIN, INT_MIN });

            bt[n].swap(make_pair(value, value));
            return true;
        }

        bool treeFind(vector<pair<int, int>>& bt, int value, int& n)
        {
            while (treeIsValid(bt, n))
            {
                if ((bt[n].first >= value) && (bt[n].second >= value))
                    return true;

                n = bt[n].first > value ? 2 * n + 1 : 2 * n + 2;
            }

            return false;
        }

        void treeDelete(vector<pair<int, int>>& bt, int n)
        {
            if (n == bt.size())
                bt.pop_back();
            else
                bt[n].first = bt[n].second = INT_MIN;
        }

        void treeInOrderTraverse(vector<pair<int, int>>& bt, vector<pair<int, int>> &p, int n = 0)
        {
            if ((n < bt.size()) && (bt[n].first >= 0))
            {
                treeInOrderTraverse(bt, p, 2 * n + 1);
                p.push_back(bt[n]);
                treeInOrderTraverse(bt, p, 2 * n + 2);
            }
        }

        vector<vector<pair<int, int>>> getSummarizedIntervals(vector<int> sn)
        {
            vector<vector<pair<int, int>>> r;

            vector<pair<int, int>> bt;
            for (int i = 0; i < sn.size(); ++i)
            {
                int n;
                if (!treeInsert(bt, sn[i], n))
                {
                    int m = 2 * n + 1;
                    if ((bt[n].first == sn[i]) && treeFind(bt, bt[n].first, m) ||
                        (bt[n].second == sn[i]) && treeFind(bt, bt[n].second, ++m))
                    {
                        bt[n].first = min(bt[n].first, bt[m].first);
                        bt[n].second = max(bt[n].second, bt[m].second);
                        treeDelete(bt, m);
                    }
                }

                vector<pair<int, int>> p;
                treeInOrderTraverse(bt, p);
                r.push_back(p);
            }

            return r;
        }

        void test()
        {
            vector<int> v = { 1, 3, 7, 2, 6 };
            auto r = getSummarizedIntervals(v);
            VERIFY(r.size() == v.size());

            vector<vector<pair<int, int>>> p{
                { { 1, 1 } },
                { { 1, 1 },{ 3, 3 } },
                { { 1, 1 },{ 3, 3 },{ 7, 7 } },
                { { 1, 3 },{ 7, 7 } },
                { { 1, 3 },{ 6, 7 } },
            };
            VERIFY(p == r);
        }
    };

    /* Count Numbers with Unique Digits */
    // LeetCode#357
    // Given a non-negative integer n, count all numbers with unique digits, x, where 0 = x < 10n.
    // Example:
    // Given n = 2, return 91. (The answer should be the total numbers in the range of 0 = x < 100,
    // excluding[11, 22, 33, 44, 55, 66, 77, 88, 99])
    struct p357
    {
        int countNumbersWithUniqueDigits(int n)
        {
            if (n == 0)
                return 1;

            if (n == 1)
                return 10;

            int s = 10; // sum
            int a = 9; // accumulator
            int f = 9; // factor

            while (--n > 0)
            {
                a *= f;
                s += a;
                --f;
            }

            return s;
        }

        /*
        int countNumbersWithUniqueDigits(int n)
        {
            int s = 1; // sum
            int a = 1; // accumulator
            int f = 9; // factor

            bool skipOnce = true;
            while (n-- > 0)
            {
                a *= f;
                s += a;

                if (skipOnce)
                    skipOnce = false;
                else
                    --f;
            }

            return s;
        }
        */

        /*
        int countNumbersWithUniqueDigits(int n)
        {
            if (n == 0)
                return 1;

            if (n == 1)
                return 10;

            int r = 10;
            int c = 9;
            for (int i = 2; i <= n; ++i)
            {
                c = c * (10 - i + 1);
                r += c;
            }

            return r;
        }
        */

        void test()
        {
            VERIFY(1 == countNumbersWithUniqueDigits(0));
            VERIFY(10 == countNumbersWithUniqueDigits(1));
            VERIFY(91 == countNumbersWithUniqueDigits(2));
            VERIFY(739 == countNumbersWithUniqueDigits(3));
            VERIFY(32491 == countNumbersWithUniqueDigits(5));
        }
    };

    /* Find K Pairs with Smallest Sums  */
    // LeetCode#373
    // You are given two integer arrays nums1 and nums2 sorted in ascending order and an integer k.
    // Define a pair (u,v) which consists of one element from the first array and one element from 
    // the second array. Find the k pairs (u1,v1),(u2,v2) ...(uk,vk) with the smallest sums.
    struct p373
    {
        bool validate(const vector<int>& v1, const vector<int>& v2, int k, const vector<pair<int, int>> mp)
        {
            auto cmp = [&v1, &v2](pair<int, int> p1, pair<int, int> p2)
            {
                return v1[p1.first] + v2[p1.second] > v1[p2.first] + v2[p2.second];
            };

            priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pq(cmp);
            for (int i = 0; i < v1.size(); ++i)
                for (int j = 0; j < v2.size(); ++j)
                    pq.emplace(i, j);

            vector<pair<int, int>> ep;
            while ((k-- > 0) && !pq.empty())
            {
                ep.push_back(pq.top());
                pq.pop();
            }

            return ep == mp;
        }

        vector<pair<int, int>> findSmallestKPairs_0(const vector<int>& v1, const vector<int>& v2, int k)
        {
            vector<pair<int, int>> kp;
            vector<int> sj(v1.size());

            k = min(k, (int)(v1.size() * v2.size()));
            while (k-- > 0)
            {
                int m = INT_MAX;
                pair<int, int> mp(-1, -1);
                for (int i = 0; i < v1.size(); ++i)
                {
                    for (int j = sj[i]; j < v2.size(); ++j)
                    {
                        if (v1[i] + v2[j] < m)
                        {
                            mp.first = i;
                            mp.second = j;
                            m = v1[i] + v2[j];
                        }
                    }
                }

                kp.push_back(mp);
                sj[mp.first] = mp.second + 1;
            }

            return kp;
        }

        vector<pair<int, int>> findSmallestKPairs(const vector<int>& v1, const vector<int>& v2, int k)
        {
            vector<pair<int, int>> kp;
            if (v1.empty() || v2.empty() || (k == 0))
                return kp;

            using pqitem = pair<int, pair<int, int>>;
            priority_queue<pqitem, vector<pqitem>, greater<pqitem>> pq;

            k = min(k, (int)(v1.size() * v2.size()));
            for (int i = 0; i < v1.size() && (i < k); ++i)
                pq.push(make_pair(v1[i] + v2[0], make_pair(i, 0)));

            while (k-- > 0)
            {
                auto p = pq.top().second;
                kp.push_back(p);

                int i = p.first;
                int j = p.second;

                pq.pop();

                if ((j + 1) < v2.size())
                    pq.push(make_pair(v1[i] + v2[j + 1], make_pair(i, j + 1)));
            }
            
            return kp;
        }

        void test()
        {
            auto v1 = getRandom(0, 100, 10);
            auto v2 = getRandom(0, 100, 5);
            sort(v1.begin(), v1.end());
            sort(v2.begin(), v2.end());
            VERIFY(validate(v1, v2, 5, findSmallestKPairs(v1, v2, 5)));

            v1 = getRandom(0, 100, 3);
            v2 = getRandom(0, 100, 2);
            sort(v1.begin(), v1.end());
            sort(v2.begin(), v2.end());

            VERIFY(validate(v1, v2, 6, findSmallestKPairs(v1, v2, 10)));
        }
    };

    /* Guess Number Higher or Lower II */
    // LeetCode#375
    // We are playing the Guess Game. The game is as follows: I pick a number from 1 to n. You have 
    // to guess which number I picked. Every time you guess wrong, I'll tell you whether the number I 
    // picked is higher or lower. However, when you guess a particular number x, and you guess wrong, 
    // you pay $x. You win the game when you guess the number I picked.
    struct p375
    {
        int getMoneyAmount(int n)
        {
            vector<vector<int>> dp(n + 1, vector<int>(n + 1));

            for (int j = 2; j < dp[0].size(); ++j)
                for (int i = j - 1; i > 0; --i)
                {
                    if ((j - i) < 2)
                    {
                        dp[i][j] = i;
                        continue;
                    }

                    int m = INT_MAX;
                    for (int k = i + 1; k < j; ++k)
                        m = min(m, k + max(dp[i][k - 1], dp[k + 1][j]));

                    dp[i][j] = m;
                }

            //printVector(dp);
            return dp[1][n];
        }

        void test()
        {
            VERIFY(0 == getMoneyAmount(1));
            VERIFY(2 == getMoneyAmount(3));
            VERIFY(16 == getMoneyAmount(10));
        }
    };

    static void run()
    {
        p375().test();
    }

    //REGISTER_RUNNABLE(lcd8)
}
