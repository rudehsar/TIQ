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
        using interval = pair<int, int>;

        void insertInterval(vector<interval>& v, const interval& i)
        {
            if (v.empty())
            {
                v.push_back(i);
                return;
            }

            // c => inserted new interval
            auto c = v.end();
            // find the 1st interval that's strictly greater than new interval i.e. the new interval
            // should be inserted there.
            auto n = upper_bound(v.begin(), v.end(), i, [](auto& x, auto& y) { return x < y; } );

            if ((n != v.end()) && (i.second + 1 >= n->first))
            {
                // merge forward
                n->first = min(n->first, i.first);
                n->second = max(n->second, i.second);

                c = n;
            }
            else
                c = v.insert(n, i);

            auto p = c == v.begin() ? v.end() : c - 1;
            if ((p != v.end()) && (p->second + 1 >= c->first))
            {
                // merge backward
                p->first = min(p->first, c->first);
                p->second = max(p->second, c->second);

                v.erase(c);
            }
        }

        vector<vector<pair<int, int>>> getSummarizedIntervals(vector<int> sn)
        {
            vector<vector<pair<int, int>>> r;

            vector<pair<int, int>> v;
            for (int i = 0; i < sn.size(); ++i)
            {
                insertInterval(v, { sn[i], sn[i] });
                r.push_back(v);
            }

            return r;
        }

        void test()
        {
            vector<int> v = { 3, 7, 1, 2, 6 };
            auto r = getSummarizedIntervals(v);
            VERIFY(r.size() == v.size());

            vector<vector<pair<int, int>>> p{
                { { 3, 3 } },
                { { 3, 3 },{ 7, 7 } },
                { { 1, 1 },{ 3, 3 },{ 7, 7 } },
                { { 1, 3 },{ 7, 7 } },
                { { 1, 3 },{ 6, 7 } },
            };
            VERIFY(p == r);
        }
    };

    /* Russian Doll Envelopes */
    // LeetCode#354
    // You have a number of envelopes with widths and heights given as a pair of integers (w, h). One 
    // envelope can fit into another if and only if both the width and height of one envelope is greater 
    // than the width and height of the other envelope.
    // What is the maximum number of envelopes can you Russian doll? (put one inside other)
    // Example:
    //  Given envelopes = [[5,4],[6,4],[6,7],[2,3]], the maximum number of envelopes you can Russian 
    // doll is 3 ([2,3] => [5,4] => [6,7]).
    struct p354
    {
        int maxEnvelopes(vector<pair<int, int>> envelopes)
        {
            // sort by ascending width, descending height
            sort(envelopes.begin(), envelopes.end(), [](auto& x, auto& y)
            {
                return (x.first == y.first)
                    ? y.second < x.second
                    : x.first < y.first;
            });

            // find max increasing subsequence based on height
            vector<int> r;
            for (auto& i : envelopes)
            {
                auto j = lower_bound(r.begin(), r.end(), i.second);
                if (j == r.end())
                    r.push_back(i.second);
                else
                    *j = i.second;
            }

            return r.size();
        }

        void test()
        {
            VERIFY(0 == maxEnvelopes({
            }));

            VERIFY(4 == maxEnvelopes({
                { 5, 4 },
                { 6, 4 },
                { 6, 7 },
                { 2, 3 },
                { 8, 8 },
                { 4, 2 },
            }));

            VERIFY(3 == maxEnvelopes({
                { 30, 50 },
                { 12, 2 },
                { 3, 4 },
                { 12, 15 },
            }));

            VERIFY(7 == maxEnvelopes({
                { 1, 2 },
                { 2, 3 },
                { 3, 4 },
                { 3, 5 },
                { 4, 5 },
                { 5, 5 },
                { 5, 6 },
                { 6, 7 },
                { 7, 8 },
            }));

            VERIFY(5 == maxEnvelopes({
                { 15,  8 },
                {  2, 20 },
                {  2, 14 },
                {  4, 17 },
                {  8, 19 },
                {  8,  9 },
                {  5,  7 },
                { 11, 19 },
                {  8, 11 },
                { 13, 11 },
                {  2, 13 },
                { 11, 19 },
                {  8, 11 },
                { 13, 11 },
                {  2, 13 },
                { 11, 19 },
                { 16,  1 },
                { 18, 13 },
                { 14, 17 },
                { 18, 19 },
            }));
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

    /* Insert Delete GetRandom O(1) */
    // LeetCode#380
    // Design a data structure that supports all following operations in average O(1) time.
    // insert(val): Inserts an item val to the set if not already present.
    // remove(val): Removes an item val from the set if present.
    // getRandom: Returns a random element from current set of elements. Each element must have the 
    // same probability of being returned.
    struct p380
    {
        class RandomSet
        {
        public:
            bool insert(const int x)
            {
                if (m_values.find(x) != m_values.end())
                    return false;

                m_slots.push_back(x);
                m_values[x] = m_slots.size() - 1;

                return true;
            }

            bool remove(const int x)
            {
                if (m_values.find(x) == m_values.end())
                    return false;

                m_values[m_slots.back()] = m_values[x];
                swap(m_slots[m_values[x]], m_slots.back());

                m_values.erase(x);
                m_slots.pop_back();

                return true;
            }

            const int get()
            {
                return m_slots[uniform_int_distribution<>(0, m_slots.size() - 1)(m_eng)];
            }

        private:
            unordered_map<int, int> m_values;
            vector<int> m_slots;
            default_random_engine m_eng = default_random_engine(random_device()());
        };

        void test()
        {
            RandomSet rs;

            VERIFY(rs.insert(0));
            VERIFY(rs.insert(1));
            VERIFY(rs.remove(0));
            VERIFY(rs.insert(2));
            VERIFY(rs.remove(1));
            VERIFY(2 == rs.get());
            VERIFY(rs.remove(2));

            VERIFY(rs.insert(0));
            VERIFY(rs.insert(1));

            vector<int> f(2);
            for (int i = 0; i < 10000; ++i)
                ++f[rs.get()];

            printVector(f);
            VERIFY((int)round(f[0] / 1000.0) == 5);
        }
    };

    static void run()
    {
        p380().test();
    }

    //REGISTER_RUNNABLE(lcd8)
}
