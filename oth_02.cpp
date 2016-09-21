#include "stdafx.h"

namespace oth2
{
    /* Draw Circle */
    struct p01
    {
        template<typename T>
        vector<pair<T, T>> drawCircle(const pair<T, T>& c, T r, T resolution = (T)1)
        {
            vector<pair<T, T>> v;
            for (T x = 0; x <= r; x += resolution)
            {
                T y = static_cast<T>(sqrt(r * r - x * x));

                v.emplace_back(c.first + x, c.second + y);
                v.emplace_back(c.first + x, c.second - y);
                v.emplace_back(c.first - x, c.second + y);
                v.emplace_back(c.first - x, c.second - y);
            }

            sort(v.begin(), v.end());
            return v;
        }

        void test()
        {
            // center
            pair<int, int> c = { 5, 5 };
            // radius
            int r = 10;
            // resolution
            int res = 1;

            auto v = drawCircle<double>(c, r, res);

            bool verified = true;
            for (auto& p : v)
            {
                double rp = sqrt(pow((p.first - c.first), 2) + pow((p.second - c.second), 2));
                //LOG(rp);

                if (abs(r - rp) > res)
                    verified = false;
            }

            VERIFY(verified);
        }
    };

    /* Sort Resulting Polynomial Array */
    struct p02
    {
        vector<int> getSortedPolynomial(const vector<int>& v, int a, int b, int c)
        {
            int pivot = -b / (2 * a);

            int j = -1;
            vector<int> r(v);
            for (int i = 0; i < r.size(); ++i)
            {
                r[i] = (a * r[i] * r[i]) + (b * r[i]) + c;

                // set the pivot position, if found
                if ((j < 0) && (i > 0) && (v[i - 1] < pivot) && (v[i] >= pivot))
                    j = i;
            }

            // no pivot, nothing more to do
            if (j < 0)
                return r;

            int i = j - 1;
            vector<int> r2(r.size());
            if (a < 0)
            {
                // "peak" case
                int k = r.size() - 1;
                while ((i >= 0) && (j < r.size()))
                    r2[k--] = (r[i] > r[j]) ? r[i--] : r[j++];

                while (i >= 0)
                    r2[k--] = r[i--];

                while (j < r.size())
                    r2[k--] = r[j++];
            }
            else
            {
                // "trough" case
                int k = 0;
                while ((i >= 0) && (j < r.size()))
                    r2[k++] = (r[i] < r[j]) ? r[i--] : r[j++];

                while (i >= 0)
                    r2[k++] = r[i--];

                while (j < r.size())
                    r2[k++] = r[j++];
            }

            swap(r, r2);
            return r;
        }

        void test()
        {
            auto isSorted = [](const vector<int>& v)
            {
                return is_sorted(v.begin(), v.end());
            };

            VERIFY(isSorted(getSortedPolynomial({ 0, 1, 2, 3, 4, 5 }, 1, 2, 3)));
            VERIFY(isSorted(getSortedPolynomial({ 0, 1, 2, 3, 4, 5 }, 1, -2, 3)));
            VERIFY(isSorted(getSortedPolynomial({ 3, -2, -1, 0, 1, 2 }, 1, 2, 3)));
            VERIFY(isSorted(getSortedPolynomial({ -2, -1, 0, 1, 2, 3 }, 1, -2, 3)));
        }
    };

    /* Compress String */
    // Write a function to shrink a given string. For example: Input String is "aaabbbaaccc" and
    // then the output string should be "a3b3a2c3". You are passed a character array. Make changes
    // in that string (do not create a new string) and return it.
    struct p03
    {
        string compressString(string str)
        {
            if (str.size() < 2)
                return str;

            string s(str);

            char p = s[0];
            int c = 1;
            int i = 1;
            for (int j = 1; j < s.size(); ++j)
            {
                if (s[j] != p)
                {
                    if (c > 1)
                        s[i++] = to_string(c)[0];

                    s[i++] = s[j];
                    p = s[j];
                    c = 1;
                }
                else
                {
                    ++c;
                }
            }

            if (c > 1)
                s[i++] = to_string(c)[0];

            return s.substr(0, i);
        }

        void test()
        {
            VERIFY("a3b3a2c3" == compressString("aaabbbaaccc"));
            VERIFY("ab2c" == compressString("abbc"));
            VERIFY("a" == compressString("a"));
            VERIFY("ab2" == compressString("abb"));
        }
    };

    /* Find 2nd (K'th) Max */
    struct p04
    {
        int getSecondMax(vector<int>& v)
        {
            if (v.size() < 2)
                return -1;

            int m1 = max(v[0], v[1]);
            int m2 = min(v[0], v[1]);

            for (int i = 2; i < v.size(); ++i)
            {
                if (v[i] > m1)
                {
                    m2 = m1;
                    m1 = v[i];
                }
                else if (v[i] > m2)
                {
                    m2 = v[i];
                }
            }

            return m2;
        }

        int getKthMax(vector<int>& v, int k)
        {
            if (v.size() < k)
                return -1;

            priority_queue<int, vector<int>, greater<int>> pq;
            for (int i = 0; i < v.size(); ++i)
            {
                pq.push(v[i]);
                if (pq.size() > k)
                    pq.pop();
            }

            return pq.top();
        }

        int getKthMax2(vector<int>& v, int k)
        {
            if (v.size() < k)
                return -1;

            // target index
            int t = v.size() - k;
            int w = 0;

            int i = 0;
            int j = v.size() - 1;
            while (i <= j)
            {
                int mid = i + (j - i) / 2;
                swap(v[mid], v[j]);

                // partition
                w = i;
                for (int k = i; k < j; ++k)
                {
                    if (v[k] <= v[j])
                        swap(v[k], v[w++]);
                }

                // final swap
                swap(v[w], v[j]);

                // check if reached target index
                if (w == t)
                    break;
                else if (w < t)
                    i = w + 1;
                else
                    j = w - 1;
            }

            return v[w];
        }

        void test()
        {
            auto v = getRandom(0, 100, 20);
            vector<int> v2(v);
            sort(v2.begin(), v2.end());

            VERIFY(v2[v.size() - 2] == getSecondMax(v));
            VERIFY(v2[v.size() - 2] == getKthMax(v, 2));
            VERIFY(v2[v.size() - 5] == getKthMax2(v, 5));
        }
    };

    /* Max Edit Distance */
    struct p05
    {
        /* DP */
        int getEditDistance(const string& s1, const string& s2)
        {
            vector<vector<int>> dp(s1.size() + 1, vector<int>(s2.size() + 1));

            for (int i = 1; i < dp.size(); ++i)
                dp[i][0] = i;

            for (int j = 1; j < dp[0].size(); ++j)
                dp[0][j] = j;

            for (int i = 1; i < dp.size(); ++i)
                for (int j = 1; j < dp[0].size(); ++j)
                    dp[i][j] = s1[i - 1] == s2[j - 1]
                    ? dp[i - 1][j - 1]
                    : min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1;

            return dp.back().back();
        }

        /* Recursive */
        int getEditDistance(const string& s1, int i, const string& s2, int j)
        {
            if ((i == s1.size()) || (j == s2.size()))
                return max(s1.size() - i, s2.size() - j);

            if (s1[i] == s2[j])
                return getEditDistance(s1, i + 1, s2, j + 1);
            else
                return min(
                    min(
                        getEditDistance(s1, i, s2, j + 1),  // remove
                        getEditDistance(s1, i + 1, s2, j)), // insert
                    getEditDistance(s1, i + 1, s2, j + 1))  // replace
                + 1;
        }

        bool isMaxEditDistanceApart(const string& s1, const string& s2, int k)
        {
            //return getEditDistance(s1, 0, s2, 0) <= k;
            return getEditDistance(s1, s2) <= k;
        }

        void test()
        {
            VERIFY(isMaxEditDistanceApart("foo", "foo", 0));
            VERIFY(isMaxEditDistanceApart("foo", "foo", 1));
            VERIFY(isMaxEditDistanceApart("foo", "foo", 2));
            VERIFY(isMaxEditDistanceApart("foo", "foox", 1));
            VERIFY(isMaxEditDistanceApart("foox", "foo", 1));
            VERIFY(isMaxEditDistanceApart("foxo", "foo", 1));
            VERIFY(!isMaxEditDistanceApart("foo", "ofo", 1));
            VERIFY(isMaxEditDistanceApart("foo", "ofo", 2));
        }
    };

    /* Parity bit */
    // Problem 5.1: How would you go about computing the parity of a very large number
    // of 64 - bit nonnegative integers ?
    struct p06
    {
        //bool isEvenParity(unsigned long long n)
        //{
        //    int parity = 0;
        //    while (n)
        //    {
        //        n &= n - 1;
        //        parity ^= 1;
        //    }

        //    return parity != 1;
        //}

        //bool isEvenParity(unsigned long long n)
        //{
        //    static int blt[4] = {
        //        0, // 00
        //        1, // 01
        //        1, // 10
        //        2, // 11
        //    };

        //    static int mask = 0x3;
        //    static int shift = 2;

        //    int parity = 0;
        //    while (n)
        //    {
        //        parity += blt[n & mask];
        //        n >>= shift;
        //    }

        //    return (parity % 2) == 0;
        //}

        bool isEvenParity(unsigned long long n)
        {
            const int range = (int)pow(2, 16);
            const int mask = 0xffff;
            const int shift = 16;
            /* static */ unique_ptr<unsigned short[]> blt(new unsigned short[range]);
            /* static */ bool initialized = false;

            if (!initialized)
            {
                for (int i = 0; i < range; ++i)
                {
                    int m = i;
                    int k = 0;
                    while (m)
                    {
                        m &= m - 1;
                        ++k;
                    }

                    blt[i] = k;
                }

                initialized = true;
            }

            int parity = 0;
            while (n)
            {
                parity += blt[n & mask];
                n >>= shift;
            }

            return (parity % 2) == 0;
        }

        void test()
        {
            VERIFY(!isEvenParity(0xb));
            VERIFY(isEvenParity(0x9));
            VERIFY(!isEvenParity(1));
        }
    };

    /* Swap bits */
    // Problem 5.2: A 64 - bit integer can be viewed as an array of 64 bits, with the bit at
    // index 0 corresponding to the least significant bit, and the bit at index 63 corresponding
    // to the most significant bit. Implement code that takes as input a 64-bit integer x and
    // swaps the bits at indices i and j.
    struct p07
    {
        unsigned long long swapBits_0(unsigned long long n, int i, int j)
        {
            if (i < j)
                swap(i, j);

            if (((n >> i) & 0x1) ^ ((n >> j) & 0x1))
            {
                n ^= ((1ULL << i) & n) >> (i - j);
                n ^= ((1ULL << j) & n) << (i - j);
            }

            return n;
        }

        // 64-bit integer type? unsigned long long
        unsigned long long swapBits(unsigned long long x, int i, int j)
        {
            if (((x >> i) & 0x1) ^ ((x >> j) & 0x1))
            {
                // MUST cast to unsigned long long or else 0x1 is treated as int
                x ^= (unsigned long long)0x1 << i;
                x ^= (unsigned long long)0x1 << j;
            }

            return x;
        }

        void test()
        {
            VERIFY(1 == swapBits(8, 3, 0));
            VERIFY(5 == swapBits(5, 2, 0));
            VERIFY(9 == swapBits(9, 1, 2));
            VERIFY(4 == swapBits(4, 2, 2));

            VERIFY(0x0 == swapBits(0x0, 9, 13));
            VERIFY(0x1 == swapBits((unsigned long long)0x1 << 63, 0, 63));
            VERIFY(0xa == swapBits(0xa, 3, 1));
            VERIFY(0x4 == swapBits(0x8, 2, 3));
        }
    };

    /* Power set */
    // Problem 5.5: Implement a method that takes as input a set 5 of distinct elements),
    // and prints the power set of S. Print the subsets one per line, with elements separated
    // by commas.
    struct p08
    {
        //void getPowerSetHelper(vector<vector<char>>& r, int i, vector<char>& v)
        //{
        //    if (i >= v.size())
        //    {
        //        r.push_back({});
        //        return;
        //    }

        //    getPowerSetHelper(r, i + 1, v);
        //    int n = r.size();
        //    for (int j = 0; j < n; ++j)
        //    {
        //        vector<char> x(r[j]);
        //        x.push_back(v[i]);
        //        r.push_back(x);
        //    }
        //}

        // #1 arbitrary range
        //vector<vector<char>> getPowerSet(vector<char> v)
        //{
        //    vector<vector<char>> result;
        //    getPowerSetHelper(result, 0, v);

        //    return result;
        //}

        // #2 range < 32
        vector<vector<char>> getPowerSet(vector<char> v)
        {
            vector<vector<char>> result;
            for (int i = 0; i < pow(2, v.size()); ++i)
            {
                vector<char> c;
                int j = i;
                int k = 0;
                do
                {
                    if (j & 0x1)
                        c.push_back(v[k]);

                    j >>= 1;
                    ++k;
                } while (j);

                result.push_back(c);
            }

            return result;
        }

        void test()
        {
            vector<char> set = { 'A', 'B', 'C' };
            vector<vector<char>> pset = getPowerSet(set);

            for (auto x : pset)
            {
                cout << "{ ";

                bool first = true;
                for (auto y : x)
                {
                    if (!first)
                        cout << ", ";
                    else
                        first = false;

                    cout << y;
                }

                cout << " }" << endl;
            }
        }
    };

    /* atoi/itoa */
    // Problem 5.6: Implement string/integer inter-conversion functions. Use the following function
    // signatures:
    //  String intToString(int x)
    // and
    //  int stringTolnt(String s).
    struct p09
    {
        string intToString(int x)
        {
            string r;
            bool neg = x < 0;

            do
            {
                r.insert(r.begin(), abs(x % 10) + '0');
                x /= 10;
            } while (x);

            return neg ? "-" + r : r;
        }

        int stringToInt(string s)
        {
            int x = 0;
            bool neg = false;
            for (int i = 0; i < s.size(); ++i)
            {
                if (s[i] == '-')
                {
                    // assert(i == 0);
                    neg = true;
                    continue;
                }

                x *= 10;
                if (x < (x / 10))
                {
                    // overflow error;
                    return x;
                }

                x += (s[i] - '0');
            }

            return neg ? -x : x;
        }

        void test()
        {
            VERIFY("0" == intToString(0));
            VERIFY("-1" == intToString(-1));
            VERIFY("1" == intToString(1));
            VERIFY("123" == intToString(123));
            VERIFY(to_string(INT_MAX) == intToString(INT_MAX));
            VERIFY(to_string(INT_MIN) == intToString(INT_MIN));

            VERIFY(0 == stringToInt("0"));
            VERIFY(1 == stringToInt("1"));
            VERIFY(-1 == stringToInt("-1"));
            VERIFY(123 == stringToInt("123"));
            VERIFY(INT_MAX == stringToInt(to_string(INT_MAX)));
            VERIFY(INT_MIN == stringToInt(to_string(INT_MIN)));
        }
    };

    /* Convert base */
    // Problem 5.7: Write a function that performs base conversion. Specifically, the input
    // is an integer base b1, a string s, representing an integer x in base bll and another
    // integer base b2; the output is the string representing the integer x in base b2. Assume
    // 2 <= bl1 <= b2 <= 16. Use "A" to represent 10, "B" for 11, ... and "F" for 15.
    struct p10
    {
        string convertBase(string s, int b1, int b2)
        {
            static char digits[] = {
                '0', '1', '2', '3',
                '4', '5', '6', '7',
                '8', '9', 'a', 'b',
                'c', 'd', 'e', 'f'
            };

            auto getDigit = [](char c)
            {
                if ((c >= '0') && (c <= '9'))
                    return c - '0';
                else if ((c >= 'a') && (c <= 'f'))
                    return c - 'a' + 10;
                else
                    return -1;
            };

            int x = 0;
            bool neg = s[0] == '-';
            for (int i = 0; i < s.size(); ++i)
            {
                int d = getDigit(s[i]);
                if (d < 0)
                    continue;
                if (d && neg)
                    d *= -1;

                x = x * b1 + d;
            }

            string r;
            do
            {
                r.insert(r.begin(), digits[abs(x % b2)]);
                x /= b2;
            } while (x);

            if (neg)
                r.insert(r.begin(), '-');

            return r;
        }

        void test()
        {
            VERIFY("1001" == convertBase("9", 16, 2));
            VERIFY("-10011111" == convertBase("-9f", 16, 2));
            VERIFY("7b" == convertBase("123", 10, 16));
        }
    };

    /* Dutch Partition */
    // 6.1: Write a function that takes an array A and an index i into A, and rearranges the
    // elements such that all elements less than A[i] appear first, followed by elements equal to
    // A[i], followed by elements greater than A[i]. Your algorithm should have 0(1) space
    // complexity and O(nlogn) time complexity.
    struct p11
    {
        int dutchPartitionHelper(vector<int>& v, int i, int j, int p, bool compareStrictlyLess)
        {
            swap(v[p], v[j]);

            int w = i;
            for (int k = i; k < j; ++k)
            {
                if (compareStrictlyLess ? v[k] < v[j] : v[k] <= v[j])
                    swap(v[k], v[w++]);
            }

            swap(v[w], v[j]);

            return w;
        }

        void dutchPartition(vector<int>& v, int i)
        {
            int p = dutchPartitionHelper(v, 0, v.size() - 1, i, false);
            dutchPartitionHelper(v, 0, p, p, true);
        }

        void test()
        {
            vector<int> v1 = { 1, 1, 2, 2, 3, 3, 3, 4, 4 };

            vector<int> v2(v1);
            random_shuffle(v2.begin(), v2.end());

            int p = 0;
            int pivot = 3;
            while (v2[p] != pivot)
                ++p;

            dutchPartition(v2, p);

            bool seenPivot = false;
            bool verified = true;
            for (int i = 0; i < v2.size(); ++i)
            {
                if (!seenPivot)
                {
                    if (v2[i] == pivot)
                    {
                        seenPivot = true;
                        while (v2[i] == pivot)
                            ++i;

                        continue;
                    }

                    if (v2[i] >= pivot)
                        verified = false;
                }
                else
                {
                    if (v2[i] <= pivot)
                        verified = false;
                }
            }

            VERIFY(verified);
        }
    };

    /* Dutch Partition II */
    struct p12
    {
        int doPartition(vector<int>& v, int i, int j, bool isStrictlyLess)
        {
            int wp = i;
            while (i < j)
            {
                if ((isStrictlyLess && (v[i] < v[j])) ||
                    (!isStrictlyLess && (v[i] <= v[j])))
                {
                    swap(v[wp++], v[i]);
                }

                ++i;
            }

            swap(v[wp], v[j]);
            return wp;
        }

        vector<int> dutchPartition(const vector<int>& values, int target)
        {
            vector<int> v(values);

            if (v.size() < 2)
                return v;

            int p = find(v.begin(), v.end(), target) - v.begin();
            if (p >= v.size())
                return v;

            swap(v[p], v[v.size() - 1]);
            p = doPartition(v, 0, v.size() - 1, true);

            swap(v[p], v[v.size() - 1]);
            p = doPartition(v, p, v.size() - 1, false);

            return v;
        }

        void test()
        {
            VERIFY(dutchPartition({ 2, 3, 3, 1, 2, 3, 1, 1 }, 2) ==
                vector<int>({ 1, 1, 1, 2, 2, 3, 3, 3 }));
            VERIFY(dutchPartition({}, 0) == vector<int>({}));
            VERIFY(dutchPartition({}, 1) == vector<int>({}));
            VERIFY(dutchPartition({ 2 }, 1) == vector<int>({ 2 }));
            VERIFY(dutchPartition({ 2, 3 }, 1) == vector<int>({ 2, 3 }));
            VERIFY(dutchPartition({ 2, 3, 3, 1, 2, 3, 1, 1 }, 5) ==
                vector<int>({ 2, 3, 3, 1, 2, 3, 1, 1 }));

            VERIFY(dutchPartition({ 2, 1 }, 1) == vector<int>({ 1, 2 }));
            VERIFY(dutchPartition({ 2, 1 }, 2) == vector<int>({ 1, 2 }));
        }
    };

    /* Mod-N Sum Subset */
    // Problem 6.5: Design an efficient algorithm for the 0 mod n-sum subset problem.
    struct p13
    {
        struct vector_hasher
        {
            size_t operator()(const vector<int>& v) const
            {
                size_t h = hash<int>()(v[0]);
                for (int i = 1; i < v.size(); ++i)
                    h ^= hash<int>()(v[i]);

                return h;
            }
        };

        void subsetSumModNAllBt(unordered_set<vector<int>, vector_hasher>& r, int sum, vector<int>& c, int i, vector<int>& v)
        {
            if ((sum > 0) && ((sum % v.size()) == 0))
            {
                sort(c.begin(), c.end());
                if (r.find(c) == r.end())
                    r.emplace(c);
            }

            if (i >= v.size())
                return;

            for (int j = i; j < v.size(); ++j)
            {
                c.push_back(j);
                subsetSumModNAllBt(r, sum + v[j], c, j + 1, v);

                c.pop_back();
            }
        }

        vector<vector<int>> subsetSumModNAll(vector<int> v)
        {
            unordered_set<vector<int>, vector_hasher> r;
            vector<int> c;
            subsetSumModNAllBt(r, 0, c, 0, v);

            return{ r.begin(), r.end() };
        }

        bool subsetSumModNBt(int sum, vector<int>& c, int i, vector<int>& v)
        {
            if ((sum > 0) && ((sum % v.size()) == 0))
                return true;

            for (int j = i; j < v.size(); ++j)
            {
                c.push_back(j);
                if (subsetSumModNBt(sum + v[j], c, j + 1, v))
                    return true;

                c.pop_back();
            }

            return false;
        }

        vector<int> subsetSumModN(vector<int> v)
        {
            vector<int> r;
            subsetSumModNBt(0, r, 0, v);

            return r;
        }

        vector<int> subsetSumModNSolution(vector<int> v)
        {
            vector<int> csum(v);
            csum[0] %= csum.size();
            for (int i = 1; i < csum.size(); ++i)
            {
                csum[i] += csum[i - 1];
                csum[i] %= csum.size();
            }

            vector<int> lt(csum.size(), -1);
            for (int i = 0; i < csum.size(); ++i)
            {
                if (csum[i] == 0)
                {
                    vector<int> ans(i + 1);
                    iota(ans.begin(), ans.end(), 0);

                    return ans;
                }
                else if (lt[csum[i]] != -1)
                {
                    int j = lt[csum[i]];
                    vector<int> ans(i - j);
                    iota(ans.begin(), ans.end(), j + 1);

                    return ans;
                }

                lt[csum[i]] = i;
            }
        }

        void test()
        {
            vector<int> v = { 429, 334, 62, 711, 704, 763, 98, 733, 721, 995 };

            // { 3, 4, 9 }
            auto r = subsetSumModNSolution(v);

            int sum = 0;
            for (auto i : subsetSumModN(v))
                sum += v[i];

            VERIFY((sum % v.size()) == 0);

            auto ar = subsetSumModNAll(v);

            unordered_set<vector<int>, vector_hasher> m;
            m.insert({ 1, 2 });
            LOG((m.find({ 1, 2, 3 }) != m.end()));
            LOG((m.find({ 1, 2 }) != m.end()));
        }
    };

    /* Replace characters */
    // Problem 6.21, pg.60: Write a function which takes as input a string s, and removes each "b"
    // and replaces each "a" by "dd". Use 0(1) additional storage - assume s is stored in an array
    // that has enough space for the final result.
    struct p14
    {
        void replaceChars(string& s, unordered_map<char, string>& toReplace, unordered_set<char>& toDelete)
        {
            int i = 0;
            int j = 0;
            while (j < s.size())
            {
                if (toDelete.find(s[j]) == toDelete.end())
                    s[i++] = s[j];
                ++j;
            }

            --i;
            --j;
            while (i >= 0)
            {
                if (toReplace.find(s[i]) == toReplace.end())
                    s[j--] = s[i];
                else
                    for (int k = toReplace[s[i]].size() - 1; k >= 0; --k)
                        s[j--] = toReplace[s[i]][k];

                --i;
            }
        }

        void test()
        {
            string s = "abracadabraa";
            unordered_map<char, string> r = {
                { 'b', "xxyy" },
            };

            unordered_set<char> d = { 'a' };
            replaceChars(s, r, d);

            VERIFY("xxyyrcdxxyyr" == s);
        }
    };

    /* Normalized Path Names */
    // #8.8
    // A file or directory can be specified via a string called the path name. This string may
    // specify an absolute path, starting from the root, e.g., /usr/bin/gcc, or a path relative
    // to the current working directory, e.g., scripts/awkscripts.
    struct p15
    {
        string normalizePath(const string& s)
        {
            if (s.size() <= 1)
                return s;

            vector<string> q;
            int i = 0;
            int j = 0;
            while ((j = s.find('/', i)) != string::npos)
            {
                string ps = s.substr(i, j - i + 1);
                i = j + 1;

                if (ps == "./")
                    continue;

                if ((ps == "/") && !q.empty())
                    continue;

                if (ps == "../")
                    q.pop_back();
                else
                    q.push_back(ps);
            }

            if (i < s.size())
                q.push_back(s.substr(i));

            string r;
            while (!q.empty())
            {
                r += q.front();
                q.erase(q.begin());
            }

            return r;
        }

        void test()
        {
            VERIFY("/usr/bin/gcc" == normalizePath("/usr/lib/../bin/gcc"));
            VERIFY("scripts/awkscripts/" == normalizePath("scripts//./../scripts/awkscripts/././"));
        }
    };

    /* Binary search (Closest larger) */
    // Problem 11.2: Design an efficient algorithm that takes a sorted array A and a key
    // k, and finds the index of the first occurrence an element larger than k; return -1 if
    // every element is less than or equal to k. For example, when applied to the array in
    // Figure 11.1 your algorithm should return -1 if k = 500; if k = 101, your algorithm
    // should return 3.
    struct p16
    {
        int bsearch1stLarger(const vector<int>& v, int k)
        {
            int i = 0;
            int j = v.size() - 1;
            while (i <= j)
            {
                int m = i + (j - i) / 2;
                if ((v[m] >= k) && ((m == 0) || (v[m - 1] < k)))
                    return m;
                else
                    if (v[m] < k)
                        i = m + 1;
                    else
                        j = m - 1;
            }

            return -1;
        }

        void test()
        {
            vector<int> v = { -14, -10, 2, 108, 108, 243, 285, 285, 285, 401 };
            VERIFY(-1 == bsearch1stLarger(v, 500));
            VERIFY(3 == bsearch1stLarger(v, 101));
            VERIFY(0 == bsearch1stLarger(v, -15));
            VERIFY(9 == bsearch1stLarger(v, 300));
        }
    };

    /* Merge Intervals */
    // Problem 13.11: Design an algorithm that takes as input a set of intervals I, and outputs the
    // union of the intervals.
    namespace p17
    {
        struct endpoint
        {
            int value;
            bool closed;
        };

        bool operator<(const endpoint& i1, const endpoint& i2)
        {
            return i1.value == i2.value
                ? i1.closed && !i2.closed
                : i1.value < i2.value;
        }

        struct interval
        {
            endpoint start;
            endpoint end;

            bool overlapped(const interval& other) const
            {
                return (end.value > other.start.value) ||
                    ((end.value == other.start.value) && (end.closed || other.start.closed));
            }

            interval& merge(const interval& other)
            {
                // assert(overlapped(other));
                if (other.start < start)
                    start = other.start;

                if (end < other.end)
                    end = other.end;

                return *this;
            }
        };

        bool operator<(const interval& i1, const interval& i2)
        {
            return i1.start < i2.start;
        }

        struct p11
        {
            vector<interval> getIntervalUnion(vector<interval> v)
            {
                sort(v.begin(), v.end());

                interval i1 = v.front();
                v.erase(v.begin());

                vector<interval> r;
                while (!v.empty())
                {
                    interval i2 = v.front();
                    v.erase(v.begin());

                    if (i1.overlapped(i2))
                        i1.merge(i2);
                    else
                    {
                        r.push_back(i1);
                        i1 = i2;
                    }
                }

                r.push_back(i1);
                return r;
            }

            void test()
            {
                vector<interval> v = {
                    { { 0, false },{ 4, true } },
                    { { 5, true },{ 11, true } },
                    { { 12, true },{ 17, false } },
                    { { 2, true },{ 4, true } },
                    { { 8, true },{ 11, false } },
                    { { 13, false },{ 13, false } },
                    { { 16, false },{ 17, false } },
                    { { 1, true },{ 1, true } },
                    { { 3, true },{ 4, false } },
                    { { 7, true },{ 8, false } },
                    { { 12, false },{ 15, true } },
                    { { 0, false },{ 3, false } },
                    { { 5, true },{ 7, false } },
                    { { 9, false },{ 11, true } },
                    { { 12, true },{ 14, true } },
                    { { 3, true },{ 6, true } },
                    { { 9, true },{ 13, false } },
                    { { 15, true },{ 19, false } },
                };

                vector<interval> r = getIntervalUnion(v);
            }
        };
    }

    /* Picking up coins, maximum gain */
    // 15.18
    // In the pick-up-coins game, an even number of coins are placed in a line, as in Figure 4.6 
    // on Page 44. Two players, F and S, take turns at choosing one coin eachthey can only choose 
    // from the two coins at the ends of the line. Player F goes first. The game ends when all 
    // the coins have been picked up. The player whose coins have the higher total value wins. A 
    // player cannot pass his turn. Design an efficient algorithm for computing the maximum 
    // margin of victory for the starting player in the pick-up-coins game.
    struct p18
    {
        int getMaxGain_0(const vector<int>& v)
        {
            // dp[i][k] := optimal gain amount of array starting at i, of length k
            vector<vector<int>> dp(v.size(), vector<int>(v.size() + 1));

            for (int i = 0; i < dp.size(); ++i)
            {
                dp[i][1] = v[i];
                if (i < dp.size() - 1)
                    dp[i][2] = max(v[i], v[i + 1]);
            }

            for (int k = 3; k < dp[0].size(); ++k)
                for (int i = 0; i < dp.size() - k + 1; ++i)
                {
                    dp[i][k] = max(
                        v[i] + min(dp[i + 1][k - 2], dp[i + 2][k - 2]),
                        v[i + k - 1] + min(dp[i][k - 2], dp[i + 1][k - 2]));
                }

            printVector(dp);
            return dp[0].back();
        }

        int getMaxGain(const vector<int>& v)
        {
            // assert(!(v.size() % 2));

            // dp[i][j] := optimal gain amount of array starting at i, ending at j
            vector<vector<int>> dp(v.size(), vector<int>(v.size()));

            // init the pair-wise values
            for (int i = 0; i < dp.size() - 1; ++i)
                dp[i][i + 1] = max(v[i], v[i + 1]);

            for (int k = 4; k <= v.size(); k += 2)
                for (int i = 0; i < v.size() - k + 1; ++i)
                {
                    int j = i + k - 1;
                    dp[i][j] = max(
                        v[i] + min(dp[i + 1][j - 1], dp[i + 2][j]),
                        v[j] + min(dp[i][j - 2], dp[i + 1][j - 1]));
                }

            return dp[0].back();
        }

        void test()
        {
            vector<int> coins = { 25, 5, 10, 5, 10, 5, 10, 25, 1, 25, 1, 25, 1, 25, 5, 10 };
            //VERIFY(140 == getMaxGain_0(coins));
            VERIFY(140 == getMaxGain(coins));
        }
    };

    /* Subset sum */
    struct p19
    {
        bool isSubsetSum(vector<int> v, int sum)
        {
            int minsum = *min_element(v.begin(), v.end());
            int maxsum = sum;

            vector<int> s(maxsum - minsum + 1);
            iota(s.begin(), s.end(), minsum);

            vector<vector<bool>> dp(v.size() + 1, vector<bool>(s.size()));

            for (int i = 1; i < dp.size(); ++i)
                for (int j = 0; j < dp[0].size(); ++j)
                {
                    if (s[j] == v[i - 1])
                        dp[i][j] = true;
                    else if (((s[j] - v[i - 1]) >= minsum) && ((s[j] - v[i - 1]) <= maxsum))
                        dp[i][j] = dp[i - 1][j] || dp[i - 1][s[j] - v[i - 1] - minsum];
                    else
                        dp[i][j] = dp[i - 1][j];
                }

            printDPMatrix(dp, s, v, 3);
            return dp.back().back();
        }

        void test()
        {
            VERIFY(isSubsetSum({ 1, 2, 3, 4, 5 }, 7));
            VERIFY(isSubsetSum({ -7, -3, -2, 5, 8 }, 0));
        }
    };

    /* 0-1 Knapsack */
    // Problem 17.2, pg.139: Design an algorithm for the knapsack problem that selects a subset of
    // items that has maximum value and weighs at most w ounces. All items have integer weights and
    // values.
    struct p20
    {
        pair<int, vector<int>> getKnapsackValue(vector<int>& v, vector<int>& W, vector<int>& C, int w)
        {
            vector<vector<int>> dp(v.size() + 1, vector<int>(w + 1));

            // compute optimal cost using DP
            for (int i = 1; i < dp.size(); ++i)
                for (int j = 0; j < dp[0].size(); ++j)
                {
                    if (W[i - 1] > j)
                        dp[i][j] = dp[i - 1][j];
                    else
                        dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - W[i - 1]] + C[i - 1]);
                }

            // find the optimal solution set
            vector<int> k;
            int i = dp.size() - 1;
            int j = dp[0].size() - 1;
            while ((i > 0) && (j > 0))
            {
                if (dp[i][j] > dp[i - 1][j])
                {
                    k.insert(k.begin(), (i - 1));
                    j -= W[i - 1];
                }

                --i;
            }

            //printDpMatrix(dp, getSeqVector(0, w), v);
            return{ dp.back().back(), k };
        }

        void test()
        {
            vector<int> v = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N','O', 'P' };
            vector<int> W = { 20, 8, 60, 55, 40, 70, 85, 25, 30, 65, 75, 10, 95, 50, 40, 10 };
            vector<int> C = { 65, 35, 245, 195, 65, 150, 275, 155, 120, 320, 75, 40, 200, 100, 220, 99 };

            auto p = getKnapsackValue(v, W, C, 130);
            VERIFY(695 == p.first);
            VERIFY((vector<int>{ 7, 9, 14 } == p.second));
        }
    };

    /* Convert Excel IDs */
    // Problem 5.8: Write a function that converts Excel column ids to the corresponding
    // integer, with" A" corresponding to 1. The function signature is int
    // ssDecodeColID(string); you may ignore error conditions, such as col containing
    // characters outside of [A, Z). How would you test your code?
    struct p21
    {
        int ssDecodeColID(const string& s)
        {
            int n = 0;
            for (int i = 0; i < s.size(); ++i)
            {
                n *= 26;
                n += s[i] - 'A' + 1;
            }

            return n;
        }

        void test()
        {
            VERIFY(1 == ssDecodeColID("A"));
            VERIFY(28 == ssDecodeColID("AB"));
        }
    };

    /* Reverse Bits */
    // Problem 5.3: Write a function that takes a 64-bit integer x and returns a 64-bit integer
    // consisting of the bits of x in reverse order.
    struct p22
    {
        unsigned long long reverseBits(unsigned long long x)
        {
            unsigned long long n = 0x0;
            for (int i = 0; i < sizeof(x) * 8; ++i)
            {
                n <<= 1;
                n |= (x >> i) & 0x1;
            }

            return n;
        }

        void test()
        {
            VERIFY(0xb00070005000b000 == reverseBits(0x000d000a000e000d));
        }
    };

    /* Generate Primes */
    // Problem 5.11: Write a function that takes a single positive integer argument n (n >= 2)
    // and return all the primes between 1 and n.
    struct p23
    {
        vector<int> getPrimeNumbers(int n)
        {
            vector<int> v(n, 0);
            iota(v.begin(), v.end(), 1);

            for (int i = 1; i < n / 2; ++i)
            {
                if (v[i] > 0)
                {
                    int j = (i + 1) * 2 - 1;
                    while (j < n)
                    {
                        v[j] = 0;
                        j += i + 1;
                    }
                }
            }

            vector<int> r;
            for (int i = 0; i < n; ++i)
            {
                if (v[i] > 0)
                    r.push_back(v[i]);
            }

            return r;
        }

        void test()
        {
            VERIFY((vector<int>{ 1, 2, 3, 5, 7, 11, 13, 17, 19 }) == getPrimeNumbers(20));
        }
    };

    /* Product of number strings */
    // Problem 6.9: Write a function that takes two strings representing integers, and
    // returns an integer representing their product.
    struct p24
    {
        string getSum(const string& x, const string& y)
        {
            if (x.size() > y.size())
                return getSum(y, x);

            string r;

            int i = x.size() - 1;
            int j = y.size() - 1;
            int c = 0;
            while (i >= 0)
            {
                int a = x[i--] - '0';
                int b = y[j--] - '0';

                int s = a + b + c;
                r.push_back((s % 10) + '0');
                c = s / 10;
            }

            while (j >= 0)
            {
                int b = y[j--] - '0';
                int s = b + c;
                r.push_back((s % 10) + '0');
                c = s / 10;
            }

            if (c > 0)
                r.push_back(c + '0');

            reverse(r.begin(), r.end());
            return r;
        }

        string getProduct(const string& x, const string& y)
        {
            string prod;
            string factor;
            for (int i = x.size() - 1; i >= 0 && x[i] != '-'; --i)
            {
                int a = x[i] - '0';

                string cp;
                int c = 0;
                for (int j = y.size() - 1; j >= 0 && y[j] != '-'; --j)
                {
                    int b = y[j] - '0';
                    int p = a * b + c;
                    cp.push_back((p % 10) + '0');
                    c = p / 10;
                }

                if (c > 0)
                    cp.push_back(c + '0');

                reverse(cp.begin(), cp.end());
                cp += factor;

                prod = getSum(prod, cp);
                factor.push_back('0');
            }

            if ((x[0] == '-') ^ (y[0] == '-'))
                prod.insert(prod.begin(), '-');

            return prod;
        }

        void test()
        {
            VERIFY(to_string(361 * 92) == getProduct("361", "92"));
            VERIFY(to_string((unsigned long long)3302961 * 9394) == getProduct("3302961", "9394"));
            VERIFY(to_string(-64 * 81) == getProduct("-64", "81"));
            VERIFY(to_string(-53 * -70) == getProduct("-53", "-70"));
            VERIFY(to_string(39 * -44) == getProduct("39", "-44"));
        }
    };

    /* Rectangle Intersection */
    // Problem 5.12: Let R and S be xy-aligned rectangles in the Cartesian plane. Write a
    // function which tests if R and S have a nonempty intersection. If the intersection is
    // nonempty, return the rectangle formed by their intersection.
    struct p25
    {
        struct point
        {
            int x;
            int y;

            bool operator==(const point& other)
            {
                return (x == other.x) && (y == other.y);
            }
        };

        struct rectangle
        {
            point tl;
            point br;

            bool operator==(const rectangle& other)
            {
                return (tl == other.tl) && (br == other.br);
            }
        };

        bool tryGetIntersect(const rectangle& r1, const rectangle& r2, rectangle& i)
        {
            if ((r1.br.x < r2.tl.x) || // if any of right-edges comes before left-edge
                (r2.br.x < r1.tl.x) ||
                (r1.tl.y < r2.br.y) || // if any of top-edge comes before bottom-edge
                (r2.tl.y < r1.br.y))
                return false;

            // compute the intersection

            // left-edge is max of left-edges
            i.tl.x = max(r1.tl.x, r2.tl.x);
            // right-edge is min of right-edges
            i.br.x = min(r1.br.x, r2.br.x);

            // bottom-edge is max of bottom-edges
            i.br.y = max(r1.br.y, r2.br.y);
            // top-edge is max of top-edges
            i.tl.y = min(r1.tl.y, r2.tl.y);

            return true;
        }

        void test()
        {
            rectangle i;

            VERIFY(tryGetIntersect({ { 0, 3 },{ 5, 0 } }, { { 4, 4 },{ 7, 2 } }, i) &&
                (i == rectangle{ { 4, 3 },{ 5, 2 } }));

            VERIFY(!tryGetIntersect({ { 0, 3 },{ 5, 0 } }, { { 4, 4 },{ 7, 3 } }, i));
        }
    };

    /* Binary Search Left-most Target */
    // Programming Pearls
    // 4.7.2
    // Binary Search to find the 1st element that meets the criteria from an array of possible
    // duplicates.
    struct p26
    {
        int bsearch(vector<int>& v, int value)
        {
            int i = 0;
            int j = v.size() - 1;

            while (i <= j)
            {
                int m = i + (j - i) / 2;
                if ((v[m] == value) && ((m == 0) || (v[m - 1] < value)))
                    return m;

                if (i == j)
                    break;

                if (v[m] >= value)
                    j = m;
                else
                    i = m + 1;
            }

            return -1;
        }

        void test()
        {
            vector<int> v = { 1, 2, 3, 3, 3, 4, 4, 5 };
            VERIFY(-1 == bsearch(v, 6));
            VERIFY(-1 == bsearch(v, 0));
            VERIFY(1 == bsearch(v, 2));
            VERIFY(2 == bsearch(v, 3));
            VERIFY(7 == bsearch(v, 5));
            VERIFY(5 == bsearch(v, 4));
        }
    };

    /* Insert in a Sorted Circular Linked List */
    // We have a sorted circular linked list. You are being passed a pointer to one of the node and
    // an integer. Write a function to insert a node in that linked list, with the given value such
    // that the linked list thus formed is also sorted.
    struct p27
    {
        struct ListNode
        {
            int val;
            ListNode* next;
        };

        //ListNode* insert(ListNode* n, ListNode* q)
        //{
        //    q->next = q;

        //    ListNode* p = n;
        //    if (p)
        //    {
        //        if (p->val > q->val)
        //        {
        //            while (p->next != n)
        //                p = p->next;
        //        }
        //        else
        //        {
        //            while ((p->next != n) && (p->next->val < q->val))
        //                p = p->next;
        //        }

        //        q->next = p->next;
        //        p->next = q;
        //    }

        //    return q;
        //}

        ListNode* insert(ListNode* n, ListNode* x)
        {
            // assert(q);
            x->next = x;

            if (n)
            {
                ListNode* p = nullptr;
                ListNode* q = n;
                do
                {
                    p = q;
                    q = p->next;

                    // reached the 1st node with a higher or equal value
                    if ((q->val >= x->val) && (p->val <= x->val))
                        break;

                    // reached the end of the sorted list (where we're jumping from larger to a smaller
                    // value and break if we know the value to be inserted is outside the range.
                    if ((q->val < p->val) && ((q->val > x->val) || (p->val < x->val)))
                        break;
                } while (q != n);

                x->next = q;
                p->next = x;
            }

            return x;
        }

        void printCircularList(ListNode* n)
        {
            ListNode* p = n;
            while ((p->next->val >= p->val) && (p->next != n))
                p = p->next;
            p = p->next;

            ListNode* q = p;
            do
            {
                cout << q->val << " -> ";
                q = q->next;
            } while (q != p);

            cout << endl;
        }

        void test()
        {
            vector<ListNode> v;
            v.reserve(10);

            ListNode* n = nullptr;
            auto addNode = [&](int i)
            {
                v.push_back(ListNode{ i });
                n = insert(n, &v.back());
                printCircularList(n);
            };

            //v.clear();
            //addNode(2);
            //addNode(1);

            //v.clear();
            //addNode(2);
            //addNode(3);

            v.clear();
            addNode(2);
            addNode(3);
            addNode(1);

            v.clear();
            addNode(2);
            addNode(3);
            addNode(4);

            v.clear();
            addNode(2);
            addNode(4);
            addNode(3);

            v.clear();
            for (auto x : getRandom(0, 10, 10))
                addNode(x);
        }
    };

    /* Square Matrix Transpose */
    // Do a matrix transpose of NxN matrix
    struct p28
    {
        void transposeNbyN(vector<vector<int>>& m)
        {
            // assert(m.size() == m[0].size());

            for (int i = 1; i < m.size(); ++i)
                for (int j = 0; j < i; ++j)
                    swap(m[i][j], m[j][i]);
        }

        void flipNbyN(vector<vector<int>>& m, bool horizontal)
        {
            // assert(m.size() == m[0].size());
            int n = m.size();

            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n / 2; ++j)
                    if (horizontal)
                        swap(m[j][i], m[j][n - i - 1]);
                    else
                        swap(m[i][j], m[i][n - j - 1]);
        }

        void test()
        {
            vector<vector<int>> m = {
                { 1, 1, 1 },
                { 2, 2, 2 },
                { 3, 3, 3 },
            };

            printVector(m);

            transposeNbyN(m);
            printVector(m);

            flipNbyN(m, false);
            printVector(m);
        }
    };

    /* Fraud Detection */
    // Imagine you are working for a small bank, attempting to analyze fraudulent credit card transactions.
    // You are given a list of strings describing credit card transactions for a single day.All
    // strings are pipe‐delimited and will take the form of
    // "<person name>|<integer whole dollar amount>|<location>|<integer time in minutes since 00:00>".
    // The list is sorted in ascending order by time.
    // Your job is to return a list of people's names whose accounts reflect suspicious activity.
    // A person's account reflects suspicious activity if you see of the following :
    // 1. A transaction spending more than $3000
    // 2. A transaction for which the next transaction for the same person differs in location, and
    //    is less than an hour later
    // The list you return should be ordered by when the first suspicious was detected.For the second
    // type of fraud, consider the "first suspicious activity" to be the earlier of the two transactions.
    //    You have to complete the function getSuspiciousActivity to return the list of suspicious
    // activities.The list you return should contain the person names as they appeared in the input.
    // Please note that the first line of the input is the number of transactions in the array.
    //    Sample Input 1:
    //    Shilpa | 500 | California | 63
    //    Tom | 25 | New York | 615
    //    Krasi | 9000 | California | 1230
    //    Tom | 25 | New York | 1235
    //    Tom | 25 | New York | 1238
    //    Shilpa | 50 | Michigan | 1300
    //    Matt | 90000 | Georgia | 1305
    //    Jay | 100000 | Virginia | 1310
    //    Krasi | 49 | Florida | 1320
    //    Krasi | 83 | California | 1325
    //    Shilpa | 50 | California | 1350
    //    Sample Output 1:
    //    Krasi
    //    Shilpa
    //    Matt
    //    Jay
    struct p29
    {
        static vector<string> splitString(string s, string delimiter)
        {
            vector<string> splits;

            int i = 0;
            string token;
            int j;
            while ((j = s.find(delimiter, i)) != string::npos)
            {
                splits.push_back(s.substr(i, j - i));
                i = j + delimiter.size();
            }

            splits.push_back(s.substr(i));
            return splits;
        }

        struct XlogEntry
        {
            string name;
            unsigned int amount;
            string location;
            unsigned int time;

            XlogEntry(string s = "")
            {
                if (s.size() <= 0)
                    return;

                vector<string> sp = splitString(s, " | ");
                // assert(sp.size() == 4);

                name = sp[0];
                amount = stoi(sp[1]);
                location = sp[2];
                time = stoi(sp[3]);
            }

            //XlogEntry(string s)
            //{
            //    int i = 0;
            //    for (int k = 0; k < 3; ++k)
            //    {
            //        int j = s.find(" | ", i);
            //        if (j < 0)
            //            break;

            //        string token = s.substr(i, j - i);
            //        switch (k)
            //        {
            //        case 0:
            //            name = token;
            //            break;
            //        case 1:
            //            amount = atoi(token.c_str());
            //            break;
            //        case 2:
            //            location = token;
            //            break;
            //        default:
            //            // assert
            //            break;
            //        }

            //        i = j + 3;
            //    }

            //    time = atoi(s.substr(i).c_str());
            //}
        };

        vector<string> getSuspiciousActivity(vector<string>& xlog)
        {
            unordered_map<string, XlogEntry> m;
            vector<pair<int, string>> a;

            for (int i = 0; i < xlog.size(); ++i)
            {
                XlogEntry x(xlog[i]);
                if (x.amount > 3000)
                {
                    // rule#1
                    a.emplace_back(x.time, x.name);
                }
                else if (m.find(x.name) != m.end() &&
                    (x.location != m[x.name].location) &&
                    (x.time - m[x.name].time < 60))
                {
                    // rule#2
                    a.emplace_back(m[x.name].time, x.name);;
                }

                m[x.name] = x;
            }

            sort(a.begin(), a.end());

            vector<string> r;
            unordered_set<string> as;
            for (int i = 0; i < a.size(); ++i)
            {
                if (as.find(a[i].second) == as.end())
                    r.push_back(a[i].second);
                as.insert(a[i].second);
            }

            return r;
        }

        void test()
        {
            vector<string> l = {
                "Shilpa | 500 | California | 63",
                "Tom | 25 | New York | 615",
                "Krasi | 9000 | California | 1230",
                "Tom | 25 | New York | 1235",
                "Tom | 25 | New York | 1238",
                "Shilpa | 50 | Michigan | 1300",
                "Matt | 90000 | Georgia | 1305",
                "Jay | 100000 | Virginia | 1310",
                "Krasi | 49 | Florida | 1320",
                "Krasi | 83 | California | 1325",
                "Shilpa | 50 | California | 1350",
            };

            VERIFY((getSuspiciousActivity(l) == vector<string>{ "Krasi", "Shilpa", "Matt", "Jay" }));
        }
    };

    // #33 - #??
    // http://www.ardendertat.com/2012/01/09/programming-interview-questions/

    /* Convert Array by interleaving */
    // Given an array[a1, a2, …, aN, b1, b2, …, bN, c1, c2, …, cN]  convert it to [a1, b1, c1, a2,
    // b2, c2, …, aN, bN, cN] in-place using constant extra space.
    struct p30
    {
        void convertArray(vector<int>& v, int N)
        {
            int c = v.size() / N;
            for (int i = 1; i < v.size(); ++i)
            {
                int j = (i % N) * c + (i / N);
                if (j > i)
                    swap(v[i], v[j]);
            }
        }

        void test()
        {
            vector<int> v = { 1, 1, 1, 2, 2, 2, 3, 3, 3 };
            printVector(v);

            convertArray(v, 3);
            printVector(v);
        }
    };

    /* All Permutations of a string */
    // Generate all permutations of a string
    struct p31
    {
        void getStringPermutionsBt(vector<string>& r, string& c, int i, int j)
        {
            if (i == j)
            {
                r.push_back(c);
                return;
            }

            for (int k = i; k <= j; ++k)
            {
                swap(c[i], c[k]);
                getStringPermutionsBt(r, c, i + 1, j);
                swap(c[i], c[k]);
            }
        }

        vector<string> getStringPermutions(string s)
        {
            vector<string> r;
            getStringPermutionsBt(r, s, 0, s.size() - 1);

            return r;
        }

        string getNextPermutation(string s)
        {
            int k = s.size() - 2;
            while ((k >= 0) && (s[k] >= s[k + 1]))
                --k;

            if (k < 0)
                return{};

            int i = s.size() - 1;
            while (s[k] >= s[i])
                --i;

            swap(s[k], s[i]);
            reverse(s.begin() + (k + 1), s.end());

            return s;
        }

        vector<string> getUniqueStringPermutions(string s)
        {
            string n(s);
            sort(n.begin(), n.end());

            vector<string> r;
            string p;
            while (!n.empty())
            {
                if ((n != p))
                    r.push_back(n);

                p = n;
                n = getNextPermutation(n);
            }

            return r;
        }

        void test()
        {
            //printVector(getStringPermutions(""));
            //printVector(getStringPermutions("a"));
            //printVector(getStringPermutions("ab"));
            //printVector(getStringPermutions("abc"));
            //printVector(getStringPermutions("abcd"));


            auto v = getUniqueStringPermutions("abbc");
            printVector(v);

            unordered_set<string> m;
            for (auto s : v)
                if (m.find(s) != m.end())
                {
                    LOG(s);
                    LOG("FAIL");
                    break;
                }
                else
                {
                    m.insert(s);
                }

            VERIFY(12 == v.size());
        }
    };

    /* Find Next Palindrome Number */
    // Given a number, find the next smallest palindrome larger than the number.For example if the
    // number is 125, next smallest palindrome is 131.
    struct p32
    {
        int getNextPalindromeNumber(int n)
        {
            string s = to_string(n);
            int len = s.size();

            // flip upper-half to make the palindrome number
            for (int i = 0; i < len / 2; ++i)
                s[len - i - 1] = s[i];

            int m = atoi(s.c_str());
            // if the palindrome number is smaller, increment middle digit(s)
            if (m < n)
            {
                int mid = len / 2;
                s[mid] = (((s[mid] - '0') + 1) % 10) + '0';
                if ((len % 2) == 0)
                {
                    s[mid - 1] = (((s[mid - 1] - '0') + 1) % 10) + '0';
                }

                m = atoi(s.c_str());
            }

            // if incrementing didn't work (e.g. middle digits were '9'), round to the next
            // 10s and recurse.
            if (m < n)
            {
                /*
                s[0] = (((s[0] - '0') + 1) % 10) + '0';
                for (int i = 1; i < s.size(); ++i)
                s[i] = '0';
                m = atoi(s.c_str());
                */

                // pick the MSB digit, increment it and multiply by decimal factor (e.g. 1xx => 200)
                int x = static_cast<int>(pow(10, len - 1));
                m = ((m / x) + 1) * x;

                return getNextPalindromeNumber(m);
            }

            return m;
        }

        void test()
        {
            VERIFY(131 == getNextPalindromeNumber(125));
            VERIFY(404 == getNextPalindromeNumber(397));
            VERIFY(1 == getNextPalindromeNumber(1));
            VERIFY(1001 == getNextPalindromeNumber(1000));
            VERIFY(1001 == getNextPalindromeNumber(1001));
            VERIFY(1331 == getNextPalindromeNumber(1225));
            VERIFY(1111 == getNextPalindromeNumber(1002));
            VERIFY(938839 == getNextPalindromeNumber(938292));
        }
    };

    /* Find Odd Occurring Element */
    // Given an integer array, one element occurs odd number of times and all others have even
    // occurrences. Find the element with odd occurrences.
    struct p33
    {
        int getOddOccuringNumber(vector<int> v)
        {
            int x = v[0];
            for (int i = 1; i < v.size(); ++i)
                x ^= v[i];

            return x;
        }

        void test()
        {
            VERIFY(1 == getOddOccuringNumber({ 1, 2, 2, 3, 3 }));
            VERIFY(1 == getOddOccuringNumber({ 1, 1, 1, 2, 2, 3, 3 }));
            VERIFY(2 == getOddOccuringNumber({ 1, 1, 2, 2, 2, 3, 3 }));

            vector<int> v(100, 0);
            for (int i = 0; i < 50; ++i)
                v[i * 2] = v[i * 2 + 1] = i;

            v.push_back(1);
            random_shuffle(v.begin(), v.end());

            VERIFY(1 == getOddOccuringNumber(v));
        }
    };

    /* Remove Duplicate Characters in String */
    // Remove duplicate characters in a given string keeping only the first occurrences.For example,
    // if the input is ‘tree traversal’ the output will be ‘tre avsl’.
    struct p34
    {
        string removeDuplicates(const string& str)
        {
            string s(str);

            unordered_set<char> m;
            int i = 0;
            int j = 0;
            while (i < s.size())
            {
                if (m.find(s[i]) == m.end())
                {
                    m.insert(s[i]);
                    s[j++] = s[i];
                }

                ++i;
            }

            return s.substr(0, j);
        }

        void test()
        {
            VERIFY("tre avsl" == removeDuplicates("tree traversal"));
            VERIFY("bio-dgtal jz" == removeDuplicates("bio-digital jazz"));
            VERIFY("spech lagirm" == removeDuplicates("speech plagiarism"));
        }
    };

    /* Trim Binary Search Tree */
    // Given the root of a binary search tree and 2 numbers min and max, trim the tree such that all
    // the numbers in the new tree are between min and max(inclusive).The resulting tree should
    // still be a valid binary search tree.
    struct p35
    {
        void trimBinaryTreeHelper(BinarySearchTree<int>& b, PTreeNode<int> p, int min, int max)
        {
            if (!p)
                return;

            if (p->val < min)
            {
                b.deleteTree(p->left);

                // BUG p->right could be null
                // p = p->right;
                // b.remove(p->parent);
                PTreeNode<int> d = p;
                p = p->right;
                b.remove(d);

                trimBinaryTreeHelper(b, p, min, max);
            }
            else if (p->val > max)
            {
                b.deleteTree(p->right);

                PTreeNode<int> d = p;
                p = p->left;
                b.remove(d);

                trimBinaryTreeHelper(b, p, min, max);
            }
            else
            {
                trimBinaryTreeHelper(b, p->left, min, max);
                trimBinaryTreeHelper(b, p->right, min, max);
            }
        }

        void trimBinaryTree(BinarySearchTree<int>& b, int min, int max)
        {
            trimBinaryTreeHelper(b, b.getRoot(), min, max);
        }

        void test()
        {
            BinarySearchTree<int> b = getBinaryTree(BinarySearchTree<int>(), { 8, 3, 10, 1, 6, 14, 4, 7, 13 });
            auto printLevelOrder = [&b]()
            {
                //for (auto s : p6().getLevelOrderPrint(b.getRoot()))
                //    LOG(s);
            };

            //      6
            //   4      9
            // 3   5  7   10
            //          8
            //vector<int> v = { 6, 4, 9, 3, 5, 7, 10, 8 };
            //for (auto i : v)
            //    b.insert(i);
            //printLevelOrder();

            //trimBinaryTree(b, 5, 8);
            //printLevelOrder();

            printLevelOrder();

            trimBinaryTree(b, 5, 13);
            printLevelOrder();
        }
    };

    /* Implement add without '+' */
    struct p36
    {
        int getSum(int a, int b)
        {
            while (b)
            {
                int c = (a & b) << 1;
                a ^= b;
                b = c;
            }

            return a;
        }

        /*
        int getSum(int a, int b)
        {
        while (b)
        {
        // b -= 1;
        int mask = 0x1;
        while (true)
        {
        if (b & mask)
        {
        b &= ~mask;
        break;
        }
        else
        {
        b |= mask;
        mask <<= 1;
        }
        }

        // a += 1
        mask = 0x1;
        while (true)
        {
        if (!(a & mask))
        {
        a |= mask;
        break;
        }
        else
        {
        a &= ~mask;
        mask <<= 1;
        }
        }
        }

        return a;
        }
        */

        void test()
        {
            VERIFY(0 == getSum(0, 0));
            VERIFY(1 == getSum(0, 1));
            VERIFY(1 == getSum(1, 0));
            VERIFY(2 == getSum(1, 1));
            VERIFY((382 + 251) == getSum(382, 251));
        }
    };

    /* Longest Palindrome */
    struct p37
    {
        pair<int, int> getLongestPalindrome_0(const string& s)
        {
            pair<int, int> r{ 0, 0 };

            vector<vector<int>> dp(s.size(), vector<int>(s.size()));
            // k is the offset length of the string starting at index i, i.e. i = 0, k = 0 is a string
            // of length 1 starting at index 0.
            for (int k = 0; k < dp.size(); ++k)
            {
                for (int i = 0; i < dp.size(); ++i)
                {
                    if (k == 0)
                    {
                        dp[i][i] = 1;
                        continue;
                    }

                    int j = i + k;
                    if (j >= s.size())
                        break;

                    if (s[i] == s[j])
                        dp[i][j] = dp[i + 1][j - 1] + 1;
                    else
                        dp[i][j] = 0;

                    if ((r.second - r.first + 1) < dp[i][j])
                    {
                        r.first = i;
                        r.second = j;
                    }
                }
            }

            return r;
        }

        pair<int, int> getLongestPalindrome(const string& s)
        {
            // dp[i][k]
            // i string end index
            // k is the offset length of the string ending at index i
            vector<vector<bool>> dp(s.size(), vector<bool>(s.size()));
            for (int i = 0; i < dp.size(); ++i)
            {
                dp[i][0] = true;
                dp[i][1] = (i > 0) && (s[i] == s[i - 1]);
            }

            pair<int, int> r{ 0, 0 };
            for (int i = 2; i < dp.size(); ++i)
            {
                for (int k = 2; i - k >= 0; ++k)
                {
                    if ((s[i] == s[i - k]) && dp[i - 1][k - 2])
                    {
                        dp[i][k] = true;
                        if (k >(r.second - r.first))
                        {
                            r.first = i - k;
                            r.second = i;
                        }
                    }
                }
            }

            return r;
        }

        void test()
        {
            auto p = pair<int, int>(5, 11);
            VERIFY(p == getLongestPalindrome("fabagtacocats"));
        }
    };

    /* Column-Fit */
    struct p38
    {
        vector<string> getColumnFit1(const string& s, int n)
        {
            // split input into words
            deque<string> words;
            int i = s.find_first_not_of(' ', 0);
            int j = string::npos;
            while ((j = s.find_first_of(' ', i)) != string::npos)
            {
                words.push_back(s.substr(i, j - i));
                i = s.find_first_not_of(' ', j);
                if (i == string::npos)
                    break;
            }

            if (i != string::npos)
                words.push_back(s.substr(i));

            vector<string> r;
            while (!words.empty())
            {
                deque<string> cline;
                int len = 0;
                while (!words.empty() && (len + words.front().size() + cline.size() - 1 < n))
                {
                    cline.push_back(words.front());
                    words.pop_front();
                    len += cline.back().size();
                }

                // distribute extra whitespaces
                int i = 0;
                int nw = n - len;
                while ((nw-- > 0) && !words.empty())
                {
                    cline[i++].push_back(' ');
                    i %= cline.size() - (cline.size() > 1 ? 1 : 0);
                }

                r.push_back("");
                while (!cline.empty())
                {
                    r.back() += cline.front();
                    cline.pop_front();
                }
            }

            return r;
        }

        vector<string> getColumnFit2(const string& s, int n)
        {
            vector<string> r;

            int i = 0;
            while (i < s.size())
            {
                // pick the end pointer
                int j = i + n > s.size() ? s.size() - 1 : i + n - 1;

                // backtrack until we're pointing at a non-WS followed by a WS
                while ((s[j] == ' ') || ((j < s.size() - 1) && s[j + 1] != ' ') && (j > i))
                    --j;

                // make the cut
                string l = s.substr(i, j - i + 1);

                // distribute the whitespaces
                int k = 0;
                int nw = n - l.size();
                // do this except for the last line
                while ((nw-- > 0) && (j != s.size() - 1))
                {
                    k = l.find_first_of(' ', k);

                    if (k == string::npos)
                        k = l.find_first_of(' ', 0);

                    if (k == string::npos)
                        break;

                    l.insert(l.begin() + k, ' ');

                    k = l.find_first_not_of(' ', k);
                    if (k == string::npos)
                        k = 0;
                }

                r.push_back(l);

                i = s.find_first_not_of(' ', j + 1);
                if (i == string::npos)
                    break;
            }

            return r;
        }

        void test()
        {
            string t =
                "We the People of the United States, in Order to form a more perfect Union, establish Justice, \
insure domestic Tranquility, provide for the common defence, promote the general Welfare, and \
secure the Blessings of Liberty to ourselves and our Posterity, do ordain and establish this \
Constitution for the United States of America.";

            auto v1 = getColumnFit1(t, 40);
            auto v2 = getColumnFit2(t, 40);

            VERIFY(v1 == v2);

            LOG("#1");
            for (auto& l : v1)
                LOG(l);

            LOG("#2");
            for (auto& l : v2)
                LOG(l);
        }
    };

    /* Array intersect */
    struct p39
    {
        vector<int> getIntersect1(const vector<int>& v1, const vector<int>& v2)
        {
            vector<int> iv;

            if (v1.empty() || v2.empty())
                return iv;

            int i = 0;
            int j = 0;
            while ((i < v1.size()) && (j < v2.size()))
            {
                if (v1[i] == v2[j])
                {
                    iv.push_back(v1[i]);
                    ++i;
                    ++j;
                }
                else if (v1[i] < v2[j])
                    ++i;
                else
                    ++j;
            }

            return iv;
        }

        vector<int> getIntersect2(const vector<int>& v1, const vector<int>& v2)
        {
            unordered_set<int> s;
            for (int i = 0; i < v1.size(); ++i)
                s.insert(v1[i]);

            vector<int> iv;
            for (int i = 0; i < v2.size(); ++i)
                if (s.find(v2[i]) != s.end())
                    iv.push_back(v2[i]);

            return iv;
        }

        void test()
        {
            VERIFY(vector<int>({ 1, 3, 8 }) == getIntersect1({ 1, 2, 3, 4, 5, 7, 8 }, { 0, 1, 3, 6, 8, 9 }));
            VERIFY(vector<int>({ 1, 3, 8 }) == getIntersect2({ 1, 2, 3, 4, 5, 7, 8 }, { 0, 1, 3, 6, 8, 9 }));
        }
    };

    /* Query Range Sum */
    struct p40
    {
        /*
        int getRangeSum(const vector<int>& v, int start, int end)
        {
        vector<int> s(v);

        for (int i = 1; i < s.size(); ++i)
        s[i] += s[i - 1];

        return s[end] - (start > 0 ? s[start - 1] : 0);
        }
        */

        template<typename T>
        using SegTreeNode = tuple<int, int, T>;

        int querySegmentTree(vector<SegTreeNode<int>>& t, int i, int start, int end)
        {
            int s = get<0>(t[i]);
            int e = get<1>(t[i]);

            if ((s > end) || (e < start))
                return 0;

            if ((s >= start) && (e <= end))
                return get<2>(t[i]);

            return
                querySegmentTree(t, 2 * i + 1, start, end) +
                querySegmentTree(t, 2 * i + 2, start, end);
        }

        void buildSegmentTree(vector<SegTreeNode<int>>& t, int i, const vector<int>& v, int start, int end)
        {
            get<0>(t[i]) = start;
            get<1>(t[i]) = end;

            if (start == end)
            {
                get<2>(t[i]) = v[start];
                return;
            }

            int mid = start + (end - start) / 2;
            buildSegmentTree(t, 2 * i + 1, v, start, mid);
            buildSegmentTree(t, 2 * i + 2, v, mid + 1, end);

            get<2>(t[i]) = get<2>(t[2 * i + 1]) + get<2>(t[2 * i + 2]);
        }

        int getRangeSum(const vector<int>& v, int start, int end)
        {
            double treeSize = pow(2, ceil(log2(v.size())) + 1);
            vector<SegTreeNode<int>> t(static_cast<int>(treeSize));

            buildSegmentTree(t, 0, v, 0, v.size() - 1);

            return querySegmentTree(t, 0, start, end);
        }

        void test()
        {
            VERIFY(22 == getRangeSum({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, 3, 6));
        }
    };

    /* Subsequence sum of k */
    struct p41
    {
        bool isSequenceSum(const vector<int>& v, int sum)
        {
            if (v.size() == 0)
                return sum == 0;

            int i = 0;
            int csum = v[i];
            int j = i;
            while ((j < v.size() - 1) && (csum != sum))
            {
                csum += v[++j];
                while ((csum > sum) && (i < j))
                    csum -= v[i++];
            }

            return csum == sum;
        }

        void test()
        {
            VERIFY(isSequenceSum({}, 0));
            VERIFY(!isSequenceSum({}, 20));
            VERIFY(isSequenceSum({ 20 }, 20));
            VERIFY(!isSequenceSum({ 1, 2, 3, }, 0));
            VERIFY(!isSequenceSum({ 10, 20, 30, }, 5));
            VERIFY(isSequenceSum({ 23, 5, 4, 7, 2, 11 }, 20));
            VERIFY(isSequenceSum({ 1, 3, 5, 23, 2 }, 8));
            VERIFY(!isSequenceSum({ 1, 3, 5, 23, 2 }, 7));
        }
    };

    /* All Palindromes */
    struct p42
    {
        vector<string> getAllPalindromes(const string& s)
        {
            vector<vector<bool>> dp(s.size(), vector<bool>(s.size()));
            for (int i = 0; i < dp.size(); ++i)
            {
                dp[i][0] = true;
                dp[i][1] = (i > 0) && (s[i] == s[i - 1]);
            }

            for (int i = 2; i < dp.size(); ++i)
                for (int k = 2; i - k >= 0; ++k)
                {
                    dp[i][k] = dp[i - 1][k - 2] && (s[i] == s[i - k]);
                }

            vector<string> r;
            for (int i = 0; i < dp.size(); ++i)
                for (int k = 0; k < dp[0].size(); ++k)
                {
                    if (dp[i][k])
                        r.push_back(s.substr(i - k, k + 1));
                }

            return r;
        }

        void test()
        {
            VERIFY(vector<string>({ "a", "a", "aa", "b" }) == getAllPalindromes("aab"));
        }
    };

    static void run()
    {
        p01().test();
    }

    //REGISTER_RUNNABLE(oth2)
}
