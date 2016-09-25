#include "stdafx.h"

#include <map>
#include <set>

namespace lcd5
{
    /* Isomorphic Strings */
    // LeetCode#205
    // Given two strings s and t, determine if they are isomorphic. Two strings are isomorphic if
    // the characters in s can be replaced to get t. For example, "egg" and "add" are isomorphic,
    // "foo" and "bar" are not.
    struct p205
    {
        bool isIsomorphic(const string& s1, const string& s2)
        {
            if (s1.size() != s2.size())
                return false;

            unordered_map<char, char> tf;
            unordered_map<char, char> tr;
            for (int i = 0; i < s1.size(); ++i)
            {
                // if it's a new char
                if (tf.find(s1[i]) == tf.end())
                {
                    // unless the corresp. char in the 2nd string was already used
                    if (tr.find(s2[i]) != tr.end())
                        return false;

                    // add the mapping
                    tf[s1[i]] = s2[i];
                    tr[s2[i]] = s1[i];
                    continue;
                }

                if (tf[s1[i]] != s2[i])
                    return false;
            }

            return true;
        }

        void test()
        {
            VERIFY(isIsomorphic("add", "egg"));
            VERIFY(!isIsomorphic("foo", "bar"));
            VERIFY(!isIsomorphic("addx", "eggg"));
        }
    };

    /* House Robber */
    // LeetCode#213
    /* House Robber II */
    // After robbing those houses on that street, the thief has found himself a new place for his 
    // thievery so that he will not get too much attention. This time, all houses at this place are 
    // arranged in a circle. That means the first house is the neighbor of the last one. Meanwhile, the 
    // security system for these houses remain the same as for those in the previous street.
    // Given a list of non-negative integers representing the amount of money of each house, 
    // determine the maximum amount of money you can rob tonight without alerting the police.
    struct p213
    {
        void getMaxBinaryRobValueHelper(PTreeNode<int>& p, int& incMax, int& excMax)
        {
            if (!p)
            {
                incMax = excMax = 0;
                return;
            }

            if (!p->left && p->right)
            {
                incMax = p->val;
                excMax = 0;

                return;
            }

            int lIncMax = 0;
            int lExcMax = 0;
            getMaxBinaryRobValueHelper(p->left, lIncMax, lExcMax);

            int rIncMax = 0;
            int rExcMax = 0;
            getMaxBinaryRobValueHelper(p->right, rIncMax, rExcMax);

            incMax = lExcMax + rExcMax + p->val;
            excMax = max(lIncMax, lExcMax) + max(rIncMax, rExcMax);
        }

        int getMaxBinaryRobValue(BinaryTree<int>& b)
        {
            int incMax = 0;
            int excMax = 0;
            getMaxBinaryRobValueHelper(b.getRoot(), incMax, excMax);

            return max(incMax, excMax);
        }

        void test()
        {
            auto b = getRandom(BinaryTree<int>(), getRandom(0, 25, 10));
            LOG(b.to_string(b.getRoot()));
            LOG(getMaxBinaryRobValue(b));
        }
    };

    /* Shortest Palindrome */
    // LeetCode#214
    // Given a string S, you are allowed to convert it to a palindrome by adding characters in front
    // of it. Find and return the shortest palindrome you can find by performing this transformation.
    // For example, given "aacecaaa", return "aaacecaaa"; given "abcd", return "dcbabcd".
    struct p214
    {
        string shortestPalindrome(const string& s)
        {
            int i = s.size() - 1;
            while (i >= 0)
            {
                if (s[i] == s[0])
                {
                    int j = 0;
                    int k = i;
                    while ((j < k) && (s[j] == s[k]))
                        ++j, --k;

                    if (j == k)
                        break;
                }

                --i;
            }

            string suffix = s.substr(i + 1);
            string prefix(suffix);
            reverse(prefix.begin(), prefix.end());

            return prefix + s.substr(0, i + 1) + suffix;
        }

        void test()
        {
            VERIFY("aaacecaaa" == shortestPalindrome("aacecaaa"));
            VERIFY("dcbabcd" == shortestPalindrome("abcd"));
        }
    };

    /* Kth Largest Element in Array */
    // LeetCode#215
    // Given an array of integers find the kth element in the sorted order(not the kth distinct
    // element).So, if the array is[3, 1, 2, 1, 4] and k is 3 then the result is 2, because it’s
    // the 3rd element in sorted order(but the 3rd distinct element is 3).
    struct p215
    {
        int partition(vector<int>& v, int lo, int hi)
        {
            int mid = lo + (hi - lo) / 2;
            swap(v[mid], v[hi]);

            int j = lo;
            for (int i = lo; i < hi; ++i)
            {
                if (v[i] <= v[hi])
                    swap(v[i], v[j++]);
            }

            swap(v[j], v[hi]);
            return j;
        }

        int getKthLargest(vector<int> v, int k)
        {
            int pk = v.size() - k;

            int lo = 0;
            int hi = v.size() - 1;
            while (lo <= hi)
            {
                int p = partition(v, lo, hi);

                if (p == pk)
                    return v[pk];

                if (p < pk)
                    lo = p + 1;
                else
                    hi = p - 1;
            }

            return -1;
        }

        void test()
        {
            VERIFY(7 == getKthLargest({ 1, 2, 3, 4, 5, 6, 7 }, 1));
            VERIFY(3 == getKthLargest({ 1, 2, 3, 4, 5, 6, 7 }, 5));
            VERIFY(5 == getKthLargest({ 1, 2, 3, 4, 5, 6, 7 }, 3));

            VERIFY(1 == getKthLargest({ 1 }, 1));

            int i;
            auto v = getRandom(0, 10, 100);

            i = getKthLargest(v, 10);
            sort(v.begin(), v.end());
            VERIFY(i == v[v.size() - 10]);

            i = getKthLargest(v, 25);
            VERIFY(i == v[v.size() - 25]);
        }
    };

    /* The Skyline Problem */
    // LeetCode#218
    // A city's skyline is the outer contour of the silhouette formed by all the buildings in that city 
    // when viewed from a distance.
    struct p218
    {
        vector<pair<int, int>> getSkyline(const vector<vector<int>>& buildings)
        {
            map<int, vector<pair<int, bool>>> m;
            for (int i = 0; i < buildings.size(); ++i)
            {
                m[buildings[i][0]].push_back(make_pair(buildings[i][2], true));
                m[buildings[i][1]].push_back(make_pair(buildings[i][2], false));
            }

            vector<pair<int, int>> r;
            multiset<int, greater<int>> s;
            for (auto& p : m)
            {
                for (auto& h : p.second)
                {
                    if (h.second)
                        s.insert(h.first);
                    else
                        s.erase(s.find(h.first));
                }

                int h = s.empty() ? 0 : *s.begin();
                if (r.empty() || (r.back().second != h))
                    r.emplace_back(p.first, h);
            }

            return r;
        }

        void test()
        {
            auto r = vector<pair<int, int>>({
                { 2, 10 },
                { 3, 15 },
                { 7, 12 },
                { 12, 0 },
                { 15, 10 },
                { 20, 8 },
                { 24, 0 },
            });
            //VERIFY(r == getSkyline({
            //    { 2, 9, 10 },
            //    { 3, 7, 15 },
            //    { 5, 12, 12 },
            //    { 15, 20, 10 },
            //    { 19, 24, 8 },
            //}));

            r = vector<pair<int, int>>({
                { 0, 3 },
                { 7, 0 },
            });
            VERIFY(r == getSkyline({
                { 0, 3, 3 },
                { 1, 5, 3 },
                { 2, 4, 3 },
                { 3, 7, 3 },
            }));
        }
    };

    /* Summary Ranges */
    // LeetCode#228
    // Given a sorted integer array without duplicates, return the summary of its ranges for 
    // consecutive numbers. For example, given [0,1,2,4,5,7], return ["0->2","4->5","7"].
    struct p228
    {
        string getSummaryRanges(const vector<int>& v)
        {
            string r;
            if (v.empty())
                return r;

            int c = 0;
            for (int i = 1; i <= v.size(); ++i)
            {
                if ((i == v.size()) || (v[i] - v[i - 1] > 1))
                {
                    if (!r.empty())
                        r += ",";

                    int hi = v[i - 1];
                    int lo = hi - c;
                    if (hi > lo)
                        r += to_string(lo) + "-" + to_string(hi);
                    else
                        r += to_string(hi);

                    c = 0;
                }
                else
                {
                    ++c;
                }
            }

            //if (!r.empty())
            //    r += ",";

            //int hi = v.back();
            //int lo = hi - c;
            //if (hi > lo)
            //    r += to_string(lo) + "-" + to_string(hi);
            //else
            //    r += to_string(hi);

            return r;
        }

        //string getSummaryRanges(const vector<int>& v)
        //{
        //    string r;

        //    if (v.empty())
        //        return r;

        //    int c = 0;
        //    int i = 1;
        //    while (i <= v.size())
        //    {
        //        if ((i == v.size()) || (v[i] - v[i - 1] != 1))
        //        {
        //            if (!r.empty())
        //                r += ",";

        //            if (c > 0)
        //                r += to_string(v[i - 1 - c]) + "-" + to_string(v[i - 1]);
        //            else
        //                r += to_string(v[i - 1]);

        //            c = 0;
        //        }
        //        else
        //        {
        //            ++c;
        //        }

        //        ++i;
        //    }

        //    return r;
        //}

        void test()
        {
            VERIFY("" == getSummaryRanges({}));
            VERIFY("0-1,3-5,8-11,13" == getSummaryRanges({ 0, 1, 3, 4, 5, 8, 9, 10, 11, 13 }));
            VERIFY("0,3-5,8-11,13" == getSummaryRanges({ 0, 3, 4, 5, 8, 9, 10, 11, 13 }));
            VERIFY("0" == getSummaryRanges({ 0 }));
            VERIFY("0-2" == getSummaryRanges({ 0, 1, 2 }));
        }
    };

    /* Sliding Window Maximum */
    // LeetCode#239
    // Given an array nums, there is a sliding window of size k which is moving from the very left of 
    // the array to the very right. You can only see the k numbers in the window. Each time the sliding 
    // window moves right by one position.
    // For example,
    // Given nums = [1,3,-1,-3,5,3,6,7], and k = 3.
    // Therefore, return the max sliding window as [3,3,5,5,6,7]
    struct p239
    {
        vector<int> maxSlidingWindow(const vector<int>& nums, int k)
        {
            vector<int> r;

            deque<pair<int, int>> dq;
            for (int i = 0; i < nums.size(); ++i)
            {
                while (!dq.empty() && (dq.back().first < nums[i]))
                    dq.pop_back();

                dq.emplace_back(nums[i], i);

                if (i - dq.front().second + 1 > k)
                    dq.pop_front();

                if (i >= k - 1)
                    r.push_back(dq.front().first);
            }

            return r;
        }

        void test()
        {
            VERIFY(vector<int>({ 3, 3, 5, 5, 6, 7 })
                == maxSlidingWindow({ 1, 3, -1, -3, 5, 3, 6, 7 }, 3));
        }
    };

    /* Valid Anagram */
    // LeetCode#242
    // Given two strings s and t, write a function to determine if t is an anagram of s.
    struct p242
    {
        bool isAnagram_0(string s1, string s2)
        {
            if (s1.size() != s2.size())
                return false;

            sort(s1.begin(), s1.end());
            sort(s2.begin(), s2.end());

            return s1 == s2;
        }

        bool isAnagram(const string& s1, const string& s2)
        {
            if (s1.size() != s2.size())
                return false;

            unordered_map<char, int> m;
            for (int i = 0; i < s1.size(); ++i)
                m[s1[i]] += 1;

            for (int i = 0; i < s2.size(); ++i)
            {
                if ((m.find(s2[i]) == m.end()) || (m[s2[i]] == 0))
                    return false;

                m[s2[i]] -= 1;
            }

            return true;
        }

        void test()
        {
            VERIFY(isAnagram("sample", "elmsap"));
            VERIFY(!isAnagram("sample", "elmsapa"));
            VERIFY(isAnagram("a", "a"));
            VERIFY(isAnagram("aa", "aa"));
            VERIFY(isAnagram("", ""));
        }
    };

    /* Strobogrammatic Number */
    // LeetCode#248
    // A strobogrammatic number is a number that looks the same when rotated 180 degrees(looked at
    // upside down). Write a function to determine if a number is strobogrammatic. The number is
    // represented as a string. For example, the numbers "69", "88", and "818" are all strobogrammatic.
    struct p248
    {
        vector<string> findStrobogrammatic(int n)
        {
            vector<string> result;
            if (n & 1) {
                result = { "0", "1", "8" };
            }
            else {
                result = { "" };
            }
            vector<string> bases = { "00", "11", "88", "69", "96" };
            int m = bases.size();
            while (n > 1) {
                n -= 2;
                vector<string> temp;
                for (string strobo : result) {
                    for (int i = (n < 2 ? 1 : 0); i < m; i++) {
                        temp.push_back(bases[i].substr(0, 1) + strobo + bases[i].substr(1));
                    }
                }

                swap(temp, result);
            }

            return result;
        }

        void findStrobogrammaticBt(int n, vector<string>& r, vector<int>& c, const vector<pair<char, char>>& sp, int p)
        {
            if (c.size() == n / 2)
            {
                string s;
                for (int i = 0; i < c.size(); ++i)
                {
                    s = s + sp[c[i]].first;
                    s.push_back(sp[c[i]].second);
                }

                r.push_back(s);
                return;
            }

            for (int i = p; i < sp.size(); ++i)
            {
                c.push_back(i);
                findStrobogrammaticBt(n, r, c, sp, p + 1);
                c.pop_back();
            }
        }

        vector<string> findStrobogrammatic2h(int n, bool isOutermost)
        {
            static vector<pair<char, char>> sp = {
                { '0', '0' },
                { '1', '1' },
                { '6', '9' },
                { '8', '8' },
                { '9', '6' },
            };

            if (n == 0)
                return vector<string>{ "" };

            if (n == 1)
                return vector<string>{ "0", "1", "8" };

            vector<string> r;

            for (int i = 0; i < sp.size(); ++i)
            {
                if (isOutermost && (i == 0))
                    continue;

                for (auto s : findStrobogrammatic2h(n - 2, false))
                    r.push_back(sp[i].first + s + sp[i].second);
            }

            return r;
        }

        vector<string> findStrobogrammatic2(int n)
        {
            return findStrobogrammatic2h(n, true);
        }

        vector<string> findStrobogrammatic3(int n)
        {
            vector<pair<char, char>> sp = {
                { '0', '0' },
                { '1', '1' },
                { '6', '9' },
                { '8', '8' },
                { '9', '6' },
            };

            vector<string> result;

            // odd n strings start with char in the middle
            if (n % 2)
            {
                result.push_back("0");
                result.push_back("1");
                result.push_back("8");
            }
            else
            {
                result.push_back("");
            }

            while (n > 1)
            {
                vector<string> v;
                for (int i = 0; i < sp.size(); ++i)
                {
                    // if we're at the outermost layer, skip the 1st pair ('0', '0')
                    if ((n < 4) && (i == 0))
                        continue;

                    for (auto r : result)
                        v.push_back(sp[i].first + r + sp[i].second);
                }

                result = v;
                n -= 2;
            }

            return result;
        }

        void test()
        {
            //lc::Solution().findStrobogrammatic(4);

            //auto v = lc::findStrobogrammatic2(4);
            //for (auto i : v)
            //    cout << i.c_str() << endl;
        }
    };

    /* Group Shifted Strings */
    // LeetCode#249
    // Given a string, we can "shift" each of its letter to its successive letter, for example:
    // "abc"->"bcd". We can keep "shifting" which forms the sequence: "abc" -> "bcd" -> ... -> "xyz"
    // Given a list of strings which contains only lowercase alphabets, group all strings that belong
    // to the same shifting sequence. For example,
    // Given :
    //  ["abc", "bcd", "acef", "xyz", "az", "ba", "a", "z"],
    // Return:
    //  [["abc", "bcd", "xyz"], ["az", "ba"], ["acef"], ["a", "z"]]
    // Note: For the return value, each inner list’s elements must follow the lexicographic order.
    struct p249
    {
        vector<vector<string>> getShiftedGroups(const vector<string>& v)
        {
            auto getCharDiff = [](char c1, char c2)
            {
                static int range = ('z' - 'a' + 1);
                //return c1 <= c2 ? c2 - c1 : c2 - c1 + ('z' - 'a' + 1);
                return (c2 - c1 + range) % range;
            };

            unordered_map<string, vector<string>> m;
            for (auto& s : v)
            {
                string d;
                for (int i = 1; i < s.size(); ++i)
                    d.push_back(getCharDiff(s[i - 1], s[i]) + '0');

                m[d].push_back(s);
            }

            vector<vector<string>> r;
            for (auto& p : m)
            {
                sort(p.second.begin(), p.second.end());
                r.push_back(p.second);
            }

            return r;
        }

        void test()
        {
            VERIFY(getShiftedGroups({ "abc", "bcd", "acef", "xyz", "az", "ba", "a", "z" }) ==
                vector<vector<string>>({
                    { "abc", "bcd", "xyz" },
                    { "az", "ba" },
                    { "acef" },
                    { "a", "z" },
            }));
        }
    };

    static void run()
    {
        p239().test();
    }

    //REGISTER_RUNNABLE(lcd5)
}
