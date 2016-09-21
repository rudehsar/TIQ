#include "stdafx.h"

namespace lcd4
{
    /* Reverse Words in a String */
    // LeetCode#151
    // Given an input string, reverse the string word by word. For example,
    // Given
    //  "the sky is blue",
    // Return
    //  "blue is sky the".
    struct p151
    {
        string reverseWords_0(const string& str)
        {
            string s(str);

            int i = 0;
            while (i < s.size())
            {
                // advance to next non-ws char
                while ((i < s.size()) && (s[i] == ' '))
                    ++i;

                // find the word end
                int j = i;
                while ((j < s.size()) && (s[j] != ' '))
                    ++j;

                // flip the word
                int p = i;
                int q = j - 1;
                while (p < q)
                    swap(s[p++], s[q--]);

                i = j;
            }

            // flip the entire string
            int p = 0;
            int q = s.size() - 1;
            while (p < q)
                swap(s[p++], s[q--]);

            return s;
        }

        string reverseWords(const string& str)
        {
            string s(str);

            int i = 0;
            while (i < s.size())
            {
                i = s.find_first_not_of(' ', i);
                if (i == string::npos)
                    break;

                int j = s.find_first_of(' ', i);
                j = (j == string::npos) ? s.size() - 1 : j - 1;

                reverse(s.begin() + i, s.begin() + j + 1);

                i = j + 1;
            }

            reverse(s.begin(), s.end());
            return s;
        }

        void test()
        {
            VERIFY("degree CS   " == reverseWords("   CS degree"));
            VERIFY("degree    CS" == reverseWords("CS    degree"));
            VERIFY("   degree CS" == reverseWords("CS degree   "));

            VERIFY("blue is sky the" == reverseWords("the sky is blue"));
        }
    };

    /* Find Minimum in Rotated Sorted Array */
    // LeetCode#153
    // Suppose a sorted array is rotated at some pivot unknown to you beforehand i.e., { 0 1 2 4 5 6 7 }
    // might become { 4 5 6 7 0 1 2 }. Find the minimum element.
    // You may assume no duplicate exists in the array.
    struct p153
    {
        int getMinInRotatedArray(const vector<int>& v)
        {
            if (v.empty())
                return -1;

            int i = 0;
            int j = v.size() - 1;
            while (i <= j)
            {
                // account for duplicates (note, we are decrementing j as we want to pick the lowest
                // index in case of duplicates)
                while ((i < j) && (v[i] == v[j]))
                    --j;

                // not rotated
                if (v[i] <= v[j])
                    return i;

                int mid = i + (j - i) / 2;
                if (v[mid] > v[j])
                    i = mid + 1;
                else
                    j = mid;
            }

            return i;
        }

        void test()
        {
            VERIFY(4 == getMinInRotatedArray({ 4, 5, 6, 7, 0, 1, 2 }));
            VERIFY(3 == getMinInRotatedArray({ 5, 6, 7, 0, 1, 2, 3, 4 }));

            VERIFY(2 == getMinInRotatedArray({ 3, 3, 1, 3, 3 }));
            VERIFY(0 == getMinInRotatedArray({ 1, 1, 1 }));
        }
    };

    /* Read N Characters Given Read4 II - Call multiple times */
    // LeetCode#158
    struct p158
    {
        const int K = 4;
        vector<vector<char>> chunks;
        int cursor;

        int readKBytes(vector<char>& buffer)
        {
            buffer.insert(buffer.end(), chunks[cursor].begin(), chunks[cursor].end());
            return chunks[cursor++].size();
        }

        void readNBytes(vector<char>& buffer, int n)
        {
            static vector<char> localBuffer;

            while ((n > 0) && !localBuffer.empty())
            {
                buffer.push_back(localBuffer.front());
                localBuffer.erase(localBuffer.begin());
                --n;
            }

            while (n >= K)
            {
                int k = readKBytes(buffer);
                n -= k;

                if (k < K)
                    return;
            }

            if (n > 0)
            {
                readKBytes(localBuffer);

                while ((n > 0) && !localBuffer.empty())
                {
                    buffer.push_back(localBuffer.front());
                    localBuffer.erase(localBuffer.begin());
                    --n;
                }
            }
        }

        void test()
        {
            chunks.clear();
            chunks.push_back({ 'a', 'b', 'c', 'd' });
            chunks.push_back({ 'e', 'f', 'g' });
            chunks.push_back({ 'h', 'i', 'j', 'k' });
            chunks.push_back({ 'l', 'm' });

            vector<char> b;
            readNBytes(b, 6); // a, b, c, d, e, f

            b.clear();
            readNBytes(b, 4); // g, h, i, j

            b.clear();
            readNBytes(b, 4); // k, l, m
        }
    };

    /* Longest Substring with at Most Two Distinct Characters */
    // LeetCode#159
    // Given a string S, find the length of the longest substring T that contains at most two
    // distinct characters. For example, Given S = "eceba", T is `"ece" which its length is 3.
    struct p159
    {
        int getMaxLenDistinctKChars(const string& s, int k)
        {
            unordered_map<char, int> m;

            int maxlen = 0;
            int i = 0;
            for (int j = 0; j < s.size(); ++j)
            {
                m[s[j]] += 1;
                while (m.size() > k)
                {
                    maxlen = max(maxlen, j - i);
                    m[s[i]] -= 1;
                    if (m[s[i]] == 0)
                        m.erase(s[i]);

                    ++i;
                }
            }

            return maxlen;
        }

        int getMaxLenDistinct2Chars(const string& s)
        {
            // start index of 1st distinct char
            int i = 0;
            // start index of 2nd distinct char
            int j = -1;

            int maxlen = 0;
            for (int k = 1; k < s.size(); ++k)
            {
                if (s[k] == s[k - 1])
                    continue;

                if ((s[k] != s[i]) && (j > 0))
                {
                    maxlen = max(maxlen, k - i);
                    i = j;
                }

                j = k;
            }

            return maxlen;
        }

        void test()
        {
            VERIFY(7 == getMaxLenDistinctKChars("ecebaaaabd", 3));
            VERIFY(5 == getMaxLenDistinctKChars("ecebaacaabbd", 2));

            VERIFY(5 == getMaxLenDistinct2Chars("ecebaacaabbd"));
        }
    };

    /* One Edit Distance */
    // LeetCode#161
    // Given two strings, determine if they are both one edit distance apart.
    struct p161
    {
        bool isOneEditDistance_0(const string& s1, const string& s2)
        {
            if (s1.size() > s2.size())
                return isOneEditDistance(s2, s1);

            if ((s2.size() - s1.size()) > 1)
                return false;

            int i = 0;
            int j = 0;
            int c = 0;
            while ((i < s1.size()) && (j < s2.size()))
            {
                if (s1[i] != s2[j])
                {
                    if (++c > 1)
                        return false;

                    if (s1.size() < s2.size())
                        --i;
                }

                ++i;
                ++j;
            }

            return (i == s1.size());
        }

        bool isOneEditDistance(const string& s1, const string& s2)
        {
            // enforce s1.size <= s2.size
            if (s1.size() > s2.size())
                return isOneEditDistance(s2, s1);

            if (s2.size() - s1.size() > 1)
                return false;

            int i = 0;
            while ((i < s1.size()) && (s1[i] == s2[i]))
                ++i;

            return (i == s1.size()) ||
                (s1.substr(i) == s2.substr(i + 1)) ||
                (s1.substr(i + 1) == s2.substr(i + 1));
        }

        void test()
        {
            VERIFY(isOneEditDistance("xxx", "xxx"));
            VERIFY(isOneEditDistance("xxx", "xxy"));
            VERIFY(isOneEditDistance("xxx", "yxx"));
            VERIFY(isOneEditDistance("xxx", "xyx"));

            VERIFY(isOneEditDistance("xxx", "xxxy"));
            VERIFY(isOneEditDistance("xxx", "yxxx"));
            VERIFY(isOneEditDistance("xxx", "xxyx"));

            VERIFY(!isOneEditDistance("xxx", "xyz"));
            VERIFY(!isOneEditDistance("xxx", "xxyz"));
            VERIFY(!isOneEditDistance("xxx", "yzx"));
        }
    };

    /* Find Peak Element */
    // LeetCode#162
    // A peak element is an element that is greater than its neighbors. Given an input array where 
    // num[i] ? num[i+1], find a peak element and return its index. The array may contain multiple 
    // peaks, in that case return the index to any one of the peaks is fine. You may imagine that 
    // num[-1] = num[n] = -8. For example, in array [1, 2, 3, 1], 3 is a peak element and your function 
    // should return the index number 2.
    struct p162
    {
        int findPeakElement(const vector<int>& v)
        {
            if (v.empty())
                return -1;

            if (v.size() < 2)
                return 0;

            int i = 0;
            while (i < v.size())
            {
                if (((i == 0) || (v[i] > v[i - 1])) &&
                    ((i == v.size() - 1) || (v[i] > v[i + 1])))
                    break;

                ++i;
            }

            return i < v.size() ? i : -1;
        }

        void test()
        {
            VERIFY(-1 == findPeakElement({}));
            VERIFY(0 == findPeakElement({ 1 }));
            VERIFY(2 == findPeakElement({ 1, 2, 3, 1 }));
            VERIFY(3 == findPeakElement({ 1, 2, 3, 4 }));
        }
    };

    /* Missing Ranges */
    // LeetCode#163
    // Given a sorted integer array where the range of elements are [0, 99] inclusive, return its
    // missing ranges. For example, given [0, 1, 3, 50, 75], return ["2", "4->49", "51->74", "76->99"]
    // Example Questions Candidate Might Ask :
    // Q : What if the given array is empty ?
    // A : Then you should return["0->99"] as those ranges are missing.
    // Q : What if the given array contains all elements from the ranges?
    // A : Return an empty list, which means no range is missing.
    struct p163
    {
        vector<string> getMissingRanges(vector<int> v)
        {
            const int range_min = -1;
            const int range_max = 100;

            vector<string> s;
            for (int k = 0; k < v.size(); ++k)
            {
                int p = (k == 0) ? range_min : v[k - 1];
                if (p + 1 != v[k])
                {
                    int i = p + 1;
                    int j = v[k] - 1;

                    if (i == j)
                        s.push_back(to_string(i));
                    else
                        s.push_back(to_string(i) + "-" + to_string(j));
                }
            }

            int p = (v.size() > 0) ? v.back() : range_min;
            if (p + 1 != range_max)
            {
                int i = p + 1;
                int j = range_max - 1;

                if (i == j)
                    s.push_back(to_string(i));
                else
                    s.push_back(to_string(i) + "-" + to_string(j));
            }

            return s;
        }

        vector<string> getMissingRanges2(vector<int> v)
        {
            v.insert(v.begin(), -1);
            v.push_back(100);

            vector<string> s;
            for (int k = 1; k < v.size(); ++k)
            {
                if ((v[k - 1] + 1) != v[k])
                {
                    int i = v[k - 1] + 1;
                    int j = v[k] - 1;

                    if (i == j)
                        s.push_back(to_string(i));
                    else
                        s.push_back(to_string(i) + "-" + to_string(j));
                }
            }

            return s;
        }

        void test()
        {
            VERIFY(getMissingRanges({ 0, 1, 3, 50, 75 }) == vector<string>({ "2", "4-49", "51-74", "76-99" }));
            VERIFY(getMissingRanges({ 99 }) == vector<string>({ "0-98" }));
            VERIFY(getMissingRanges({}) == vector<string>({ "0-99" }));
        }
    };

    /* Compare Version Numbers */
    // LeetCode#165
    // Compare two version numbers version1 and version2.
    //  If version1 > version2 return 1,
    //  If version1 < version2 return -1,
    //  Otherwise return 0.
    // You may assume that the version strings are non-empty and contain only digits and the . character.
    // The . character does not represent a decimal point and is used to separate number sequences.
    // Here is an example of version numbers ordering:
    //  0.1 < 1.1 < 1.2 < 13.37
    struct p165
    {
        vector<int> getVersionSplits(const string& v)
        {
            vector<int> vd;

            int i = 0;
            while (i < v.size())
            {
                int j = v.find(".", i);

                string d;
                if (j == string::npos)
                    d = v.substr(i);
                else
                    d = v.substr(i, j - i);

                if (!d.empty())
                    vd.push_back(atoi(d.c_str()));

                i += d.size() + 1;
            }

            return vd;
        }

        int compareVersion(const string& v1, const string& v2)
        {
            vector<int> vd1 = getVersionSplits(v1);
            vector<int> vd2 = getVersionSplits(v2);

            // normalize by appending 0 on the smaller version string
            if (vd1.size() != vd2.size())
            {
                auto& t = vd1.size() < vd2.size() ? vd1 : vd2;
                int n = abs((int)vd1.size() - (int)vd2.size());
                while (n-- > 0)
                    t.push_back(0);
            }

            for (int i = 0; i < vd1.size(); ++i)
            {
                if (vd1[i] != vd2[i])
                    return vd1[i] > vd2[i] ? 1 : -1;
            }

            return 0;
        }

        void test()
        {
            VERIFY(1 == compareVersion("1.0", "0.1"));
            VERIFY(0 == compareVersion("1.0", "1.0"));
            VERIFY(0 == compareVersion("1", "1.0"));
            VERIFY(-1 == compareVersion("0.1", "1.0"));
            VERIFY(1 == compareVersion("1.3", "1.2.0"));
            VERIFY(1 == compareVersion("1.2.0.3", "1.2.0.1"));
        }
    };

    /* Dungeon Game */
    // LeetCode#174
    struct p174
    {
        int solveDungeonGame(const vector<vector<int>>& d)
        {
            vector<vector<int>> h(d.size(), vector<int>(d[0].size()));
            h.back().back() = max(1 - d.back().back(), 1);

            for (int i = h.size() - 1; i >= 0; --i)
                for (int j = h[0].size() - 1; j >= 0; --j)
                {
                    if ((i == h.size() - 1) && (j == h[0].size() - 1))
                        continue;

                    if (i == h.size() - 1) // bottom edge
                        h[i][j] = max(h[i][j + 1] - d[i][j], 1);
                    else if (j == h[0].size() - 1) // bottom edge
                        h[i][j] = max(h[i + 1][j] - d[i][j], 1);
                    else
                        h[i][j] = max(min(h[i + 1][j], h[i][j + 1]) - d[i][j], 1);
                }

            printVector(d);
            printVector(h);
            return h[0][0];
        }

        bool validateDungeonGame(const vector<vector<int>>& d, vector<vector<int>>& p, int i, int j, int v)
        {
            if ((i >= d.size()) || (j >= d[0].size()) || (d[i][j] + v <= 0))
                return false;

            p[i][j] = 1;
            if (((i == d.size() - 1) && (j == d[0].size() - 1)) ||
                validateDungeonGame(d, p, i + 1, j, d[i][j] + v) ||
                validateDungeonGame(d, p, i, j + 1, d[i][j] + v))
            {
                return true;
            }

            p[i][j] = 0;
            return false;
        }

        bool validateDungeonGame(const vector<vector<int>>& d, int v)
        {
            vector<vector<int>> p(d.size(), vector<int>(d[0].size()));
            if (validateDungeonGame(d, p, 0, 0, v))
            {
                printVector(p);
                return true;
            }

            return false;
        }

        void test()
        {
            vector<vector<int>> d = {
                { -2, -3, 3, },
                { -5, -10, 1, },
                { 10, 30, -5 },
            };
            //VERIFY(validateDungeonGame(d, solveDungeonGame(d)));

            d = {
                { 2, -4, -10, 2, -6 },
                { -10, -9, -1, 9, 6, },
                { 1, -5, -10, 7, 6, },
                { -9, 0, -7, -8, 8, },
            };
            VERIFY(validateDungeonGame(d, solveDungeonGame(d)));

            d.clear();
            d.push_back(getRandom(-10, 10, getRandom(3, 6)));
            for (int i = 0; i < getRandom(3, 5); ++i)
                d.push_back(getRandom(-10, 10, d[0].size()));

            //VERIFY(validateDungeonGame(d, solveDungeonGame(d)));
        }
    };

    /* Reverse Words in a String II */
    // LeetCode#186
    // Given an input string, reverse the string word by word. A word is defined as a sequence of
    // non-space characters. The input string does not contain leading or trailing spaces and the
    // words are always separated by a single space. For example,
    // Given
    //  "the sky is blue",
    // Return
    //  "blue is sky the".
    //
    // Could you do it in - place without allocating extra space?
    struct p186
    {
        string reverseWords(string s)
        {
            int i = 0;
            int j = 0;
            while (i < s.size())
            {
                // skip past leading wspaces
                if ((i = s.find_first_not_of(' ', j)) == string::npos)
                    break;

                if ((j = s.find_first_of(' ', i)) == string::npos)
                    j = s.size();

                reverse(s.begin() + i, s.begin() + j);
            }

            reverse(s.begin(), s.end());
            return s;
        }

        void test()
        {
            VERIFY("blue is sky the" == reverseWords("the sky is blue"));
            VERIFY("   blue is sky the   " == reverseWords("   the sky is blue   "));
        }
    };

    /* Best Time to Buy and Sell Stock IV */
    // LeetCode#188
    // Say you have an array for which the ith element is the price of a given stock on day i. Design 
    // an algorithm to find the maximum profit. You may complete at most k transactions.
    // Note:
    // You may not engage in multiple transactions at the same time (ie, you must sell the stock 
    // before you buy again).
    struct p188
    {
        int getMaxProfit(const vector<int>& v, int tx)
        {
            if (v.size() < 2)
                return 0;

            vector<vector<int>> dp(tx + 1, vector<int>(v.size()));
            for (int k = 1; k < dp.size(); ++k)
            {
                // Let
                //  T_i = Max { dp[k - 1][j] + v[i] - v[j] } where j = 0..i-1
                //      = Max { dp[k - 1][j] - v[j] } + v[i] where j = 0..i-1
                // Then (we have a recurring formula for computing T_i as,
                //  T_i+ = Max { T_i, dp[k - 1][i] - v[i] } + v[i+]

                int pv = INT_MIN;
                for (int i = 1; i < dp[0].size(); ++i)
                {
                    // Note, this is equivalent to the following
                    //      for (int j = 0; j < i; ++j)
                    //          pv = max(pv, dp[k - 1][j] - v[j]);
                    pv = max(pv, dp[k - 1][i - 1] - v[i - 1]);

                    // Either skip i'th day transaction or take the max of all profits on an
                    // intermediate day with 1 less transaction
                    dp[k][i] = max(dp[k][i - 1], pv + v[i]);
                }
            }

            printVector(dp);
            return dp.back().back();
        }

        void test()
        {
            //VERIFY(0 == getMaxProfit({ 100, 90, 80, 70, 60 }, 1));
            //VERIFY(4 == getMaxProfit({ 8, 3, 6, 4, 7 }, 1));
            VERIFY(10 == getMaxProfit({ 2, 5, 7, 1, 4, 3, 1, 3 }, 3));
            VERIFY(0 == getMaxProfit({}, 1));
            VERIFY(1 == getMaxProfit({ 1, 2 }, 1));
        }
    };

    /* Rotate Array */
    // LeetCode#189
    // Rotate an array of n elements to the right by k steps.
    // For example, with n = 7 and k = 3, the array,
    //  [1, 2, 3, 4, 5, 6, 7]
    // is rotated to,
    //  [5, 6, 7, 1, 2, 3, 4]
    struct p189
    {
        void rotateArray_1(vector<int>& v, int k)
        {
            // make a copy
            vector<int> r(v);

            int N = v.size();
            for (int i = 0; i < N; ++i)
                v[(i + k) % N] = r[i];
        }

        void rotateArray_2(vector<int>& v, int k)
        {
            int N = v.size();
            k = k % N;

            if (k < N / 2)
            {
                // copy the last k values to temporary array
                vector<int> tmp(v.begin() + N - k, v.end());

                // shift 0..(N - 1) to the right by k positions
                for (int i = N - k - 1; i >= 0; --i)
                    v[i + k] = v[i];

                // copy over temporary array at 0..(k - 1)
                for (int i = 0; i < k; ++i)
                    v[i] = tmp[i];
            }
            else
            {
                // copy the first N - k values to temporary array
                vector<int> tmp(v.begin(), v.begin() + N - k);

                // shift (N - k)..(N - 1) to the left by (N - k) positions
                for (int i = N - k; i < N; ++i)
                    v[i - N + k] = v[i];

                // copy over temporary array at (N - k + 1)..(N - 1)
                for (int i = 0; i < N - k; ++i)
                    v[i + k] = tmp[i];
            }
        }

        void rotateArray_3(vector<int>& v, int k)
        {
            int N = v.size();
            k = k % N;

            // reverse 0..(N - k - 1)
            int i = 0;
            int j = N - k - 1;
            while (i < j)
                swap(v[i++], v[j--]);

            // reverse (N - k)..(N - 1)
            i = N - k;
            j = N - 1;
            while (i < j)
                swap(v[i++], v[j--]);

            // reverse 0..(N - 1)
            i = 0;
            j = N - 1;
            while (i < j)
                swap(v[i++], v[j--]);
        }

        void rotateArray_4(vector<int>& v, int k)
        {
            int N = v.size();
            k = k % N;

            // do a pair-wise swap of each pair in the array for k times. each pass is equivalent to 
            // percolating the last element all the way to the front.
            while (k-- > 0)
            {
                int i = N - 1;
                while (i > 0)
                {
                    swap(v[i - 1], v[i]);
                    --i;
                }
            }
        }

        void rotateArray_5(vector<int>& v, int k)
        {
            int N = v.size();
            for (int i = 0; i < N; ++i)
            {
                // determine whether i is the lead index of a swap-loop
                int j = (i + k) % N;
                while ((j != i) && (j > i))
                    j = (j + k) % N;

                // if i isn't the lead index, skip (as i will be covered by some other index)
                if (j != i)
                    continue;

                // start the full swap-loop
                j = (i + k) % N;
                int tmp = v[i];
                while (j != i)
                {
                    swap(tmp, v[j]);
                    j = (j + k) % N;
                }

                v[j] = tmp;
            }
        }

        void test()
        {
            for (auto& pf : {
                &p189::rotateArray_1,
                &p189::rotateArray_2,
                &p189::rotateArray_3,
                &p189::rotateArray_4,
                &p189::rotateArray_5,
            })
            {
                vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                vector<int> r0(v);
                vector<int> r3 = { 7, 8, 9, 0, 1, 2, 3, 4, 5, 6 };
                vector<int> r7 = { 3, 4, 5, 6, 7, 8, 9, 0, 1, 2 };

                vector<int> c(v);
                (this->*pf)(c, 3);
                VERIFY(r3 == c);

                c.clear();
                c.insert(c.begin(), v.begin(), v.end());
                (this->*pf)(c, 7);
                VERIFY(r7 == c);

                c.clear();
                c.insert(c.begin(), v.begin(), v.end());
                (this->*pf)(c, 13);
                VERIFY(r3 == c);

                c.clear();
                c.insert(c.begin(), v.begin(), v.end());
                (this->*pf)(c, 0);
                VERIFY(r0 == c);
            }
        }
    };

    /* House Robber */
    // LeetCode#198
    // You are a professional robber planning to rob houses along a street. Each house has a certain 
    // amount of money stashed, the only constraint stopping you from robbing each of them is that 
    // adjacent houses have security system connected and it will automatically contact the police if 
    // two adjacent houses were broken into on the same night.
    // Given a list of non-negative integers representing the amount of money of each house, 
    // determine the maximum amount of money you can rob tonight without alerting the police.
    struct p198
    {
        int getMaxHouseRobValue(const vector<int>& v, const int lo, const int hi)
        {
            if (hi - lo < 2)
                return v[lo];

            vector<int> s(hi - lo + 1);

            s[0] = v[lo];
            s[1] = max(s[0], v[lo + 1]);

            for (int i = 2; i < s.size(); ++i)
                s[i] = max(s[i - 2] + v[i + lo], s[i - 1]);

            return s.back();
        }

        int getMaxHouseRobValue(const vector<int>& v)
        {
            return max(
                getMaxHouseRobValue(v, 0, v.size() - 2),
                getMaxHouseRobValue(v, 1, v.size() - 1));
        }

        void test()
        {
            VERIFY(20 == getMaxHouseRobValue({ 1, 10, 0, 10, 1 }));
            VERIFY(21 == getMaxHouseRobValue({ 1, 10, 0, 10, 0, 1 }));
        }
    };

    static void run()
    {
        p188().test();
    }

    //REGISTER_RUNNABLE(lcd4)
}
