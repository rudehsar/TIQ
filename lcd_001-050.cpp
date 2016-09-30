#include "stdafx.h"

namespace lcd1
{
    /* Two Sum */
    // LeetCode#1
    // Given an array of integers, find two numbers such that they add up to a specific target number.
    // The function twoSum should return indices of the two numbers such that they add up to the
    // target, where index1 must be less than index2. Please note that your returned answers (both
    // index1 and index2) are not zero-based.
    struct p001
    {
        vector<pair<int, int>> getPairsBySum(const vector<int>& v1, const vector<int>& v2, int sum)
        {
            unordered_map<int, int> m;
            for (int i = 0; i < v1.size(); ++i)
                m[sum - v1[i]] = i;

            vector<pair<int, int>> r;
            for (int i = 0; i < v2.size(); ++i)
                if (m.find(v2[i]) != m.end())
                    r.emplace_back(m[v2[i]], i);

            return r;
        }

        void test()
        {
            vector<int> v1 = { 1, 3, 5 };
            vector<int> v2 = { 6, 7, 8 };

            auto r = vector<pair<int, int>>{
                { 1, 0 },
                { 0, 2 },
            };
            VERIFY(r == getPairsBySum(v1, v2, 9));
        }
    };

    /* Add Two Numbers */
    // LeetCode#002
    // You are given two linked lists representing two non-negative numbers. The digits are stored
    // in reverse order and each of their nodes contain a single digit. Add the two numbers and
    // return it as a linked list.
    // Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
    // Output: 7 -> 0 -> 8
    struct p002
    {
        PListNode<int> getSum(PListNode<int> p1, PListNode<int> p2)
        {
            PListNode<int> s;
            PListNode<int> p;

            int carry = 0;
            while (p1 || p2)
            {
                int d = carry;
                if (p1)
                {
                    d += p1->data;
                    p1 = p1->next;
                }

                if (p2)
                {
                    d += p2->data;
                    p2 = p2->next;
                }

                carry = (d >= 10) ? 1 : 0;
                d = d % 10;

                PListNode<int> n = make_shared<ListNode<int>>();
                n->data = d;
                n->next = nullptr;

                if (p)
                    p->next = n;
                else
                    s = n;

                p = n;
            }

            if (carry)
            {
                p->next = make_shared<ListNode<int>>();
                p->next->data = carry;
            }

            return s;
        }

        PListNode<int> convert_from(int n)
        {
            PListNode<int> h;
            if (n == 0)
                return h;

            h = make_shared<ListNode<int>>();
            h->data = n % 10;
            n /= 10;

            PListNode<int> p = h;
            while (n)
            {
                PListNode<int> q = make_shared<ListNode<int>>();
                q->data = n % 10;
                n /= 10;

                p->next = q;
                p = q;
            }

            return h;
        }

        int convert_from(PListNode<int> h)
        {
            int n = 0;
            int m = 1;
            while (h)
            {
                n += h->data * m;
                m *= 10;

                h = h->next;
            }

            return n;
        }

        void test()
        {
            int x = 246;
            int y = 837;
            VERIFY(convert_from(getSum(convert_from(x), convert_from(y))) == (x + y));
        }
    };

    /* Longest Substring Without Repeating Characters */
    // LeetCode#3
    // Given a string, find the length of the longest substring without repeating characters. For
    // example, the longest substring without repeating letters for "abcabcbb" is "abc", which the
    // length is 3. For "bbbbb" the longest substring is "b", with the length of 1.
    struct p003
    {
        int getMaxWithNoRepeats(const string& s)
        {
            unordered_set<char> m;
            int i = 0;
            int mx = 0;
            for (int j = 0; j < s.size(); ++j)
            {
                if (m.find(s[j]) != m.end())
                {
                    while (m.find(s[j]) != m.end())
                        m.erase(s[i++]);
                }

                m.insert(s[j]);
                mx = max(mx, j - i + 1);
            }

            return mx;
        }

        void test()
        {
            VERIFY(0 == getMaxWithNoRepeats(""));
            VERIFY(3 == getMaxWithNoRepeats("abcabcbb"));
            VERIFY(1 == getMaxWithNoRepeats("bbb"));
            VERIFY(3 == getMaxWithNoRepeats("abc"));
        }
    };

    /* Median of Two Sorted Arrays */
    // There are two sorted arrays nums1 and nums2 of size m and n respectively. Find the median of 
    // the two sorted arrays. The overall run time complexity should be O(log(m+n)).
    struct p004
    {
        double findMedian_0(const vector<int>& v1, const vector<int>& v2)
        {
            if ((v1.size() + v2.size()) == 0)
                return 0.0;

            int m = (v1.size() + v2.size()) / 2;

            int i = 0;
            int j = 0;

            int m1 = 0;
            int m2 = 0;
            while ((i + j) <= m)
            {
                m1 = m2;
                if (i >= v1.size())
                    m2 = v2[j++];
                else if (j >= v2.size())
                    m2 = v1[i++];
                else
                    m2 = (v1[i] < v2[j]) ? v1[i++] : v2[j++];
            }

            return (((v1.size() + v2.size()) % 2) == 0) ? (m1 + m2) / 2.0 : m2;
        }

        int findKth(const vector<int>& v1, const vector<int>& v2, int k)
        {
            if (v1.empty() || v2.empty())
                return v1.empty() ? v2[k] : v1[k];

            if (k == 0)
                return min(v1[0], v2[0]);

            if (v1.size() > v2.size())
                return findKth(v2, v1, k);

            int m1 = min(k / 2, static_cast<int>(v1.size() - 1));
            int m2 = k - m1 - 1;

            if (v1[m1] < v2[m2])
                return findKth(vector<int>(v1.begin() + m1 + 1, v1.end()), v2, k - m1 - 1);
            else if (v1[m1] > v2[m2])
                return findKth(v1, vector<int>(v2.begin() + m2 + 1, v2.end()), k - m2 - 1);
            else
                return v1[m1];
        }

        double findMedianSortedArrays(const vector<int>& v1, const vector<int>& v2)
        {
            if (v1.empty() && v2.empty())
                return 0.0;

            int len = v1.size() + v2.size();

            return (len % 2)
                ? findKth(v1, v2, len / 2)
                : (findKth(v1, v2, len / 2 - 1) + findKth(v1, v2, len / 2)) / 2.0;
        }

        void test()
        {
            VERIFY(2.0 == findMedianSortedArrays({ 1, 3 }, { 2 }));
            VERIFY(2.5 == findMedianSortedArrays({ 1, 2 }, { 3, 4 }));
            VERIFY(1.5 == findMedianSortedArrays({ 1, 2 }, { 1, 2 }));

            VERIFY(4.0 == findMedianSortedArrays({ 1, 2, 3, 4 }, { 5, 6, 7 }));
            VERIFY(4.5 == findMedianSortedArrays({ 1, 2, 3, 4 }, { 5, 6, 7, 8 }));

            VERIFY(1.0 == findMedianSortedArrays({ 1 }, {}));
            VERIFY(1.0 == findMedianSortedArrays({}, { 1 }));
            VERIFY(0.0 == findMedianSortedArrays({}, {}));
        }
    };

    /* Longest Palindromic Substring */
    // LeetCode#5
    // Given a string S, find the longest palindromic substring in S. You may assume that the maximum 
    // length of S is 1000, and there exists one unique longest palindromic substring.
    struct p005
    {
        string getLongestPalindrome_0(const string& s)
        {
            // dp[i][j] palindrome ending at index i of length (j + 1)
            vector<vector<bool>> dp(s.size(), vector<bool>(s.size()));

            // trivial cases
            for (int i = 0; i < dp.size(); ++i)
            {
                // single char is a palindrome of length 1
                dp[i][0] = true;

                // twins are palindromes of length 2
                if ((i > 0) && (s[i] == s[i - 1]))
                    dp[i][1] = true;
            }

            // start from 3rd char with offset 2 since we have already considered offset length
            // of 0 and 1 above.
            pair<int, int> mp;
            for (int i = 2; i < dp.size(); ++i)
                for (int j = 2; j <= i; ++j)
                {
                    if ((s[i] == s[i - j]) && (dp[i - 1][j - 2]))
                    {
                        dp[i][j] = true;
                        if (j > mp.second)
                            mp = { i - j, j };
                    }
                }

            printVector(dp);
            return mp.second > 0 ? s.substr(mp.first, mp.second + 1) : "";
        }

        /* Optimized */
        string getLongestPalindrome_opt(const string& s)
        {
            vector<vector<int>> dp(s.size(), vector<int>(2));

            int mi;
            int mj;
            int mlen = INT_MIN;
            int c0 = 0;
            int c1 = 1;
            for (int j = 0; j < s.size(); ++j)
            {
                for (int i = j; i >= 0; --i)
                {
                    if (i == j)
                        dp[i][c0] = 1;
                    else if ((j - i) == 1)
                        dp[i][c0] = s[i] == s[j];
                    else
                        dp[i][c0] = (s[i] == s[j]) && dp[i + 1][c1];

                    if ((dp[i][c0]) && ((j - i + 1) > mlen))
                    {
                        mi = i, mj = j;
                        mlen = mj - mi + 1;
                    }
                }

                swap(c0, c1);
            }

            return s.substr(mi, mj - mi + 1);
        }

        string getLongestPalindrome(const string& s)
        {
            // dp[i][j] indicates s[i..j] is a palindrome 
            vector<vector<bool>> dp(s.size(), vector<bool>(s.size()));

            // longest palindromic string
            string lp;

            for (int j = 0; j < dp.size(); ++j)
                for (int i = j; i >= 0; --i)
                {
                    if (i == j)
                        dp[i][j] = true;
                    else if ((j - i) == 1)
                        dp[i][j] = s[i] == s[j];
                    else
                        dp[i][j] = (s[i] == s[j]) && dp[i + 1][j - 1];

                    // determine longest palindrome
                    if (dp[i][j] && ((j - i + 1) > lp.size()))
                        lp = s.substr(i, j - i + 1);
                }

            return lp;
        }

        void test()
        {
            VERIFY("tacocat" == getLongestPalindrome("footacocatbar"));
            VERIFY("aa" == getLongestPalindrome("aab"));
            VERIFY("a" == getLongestPalindrome("a"));
            VERIFY("cc" == getLongestPalindrome("ccd"));
        }
    };

    /* Zigzag Conversion */
    // LeetCode#6
    // The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like
    // this: (you may want to display this pattern in a fixed font for better legibility) 
    // P   A   H   N
    // A P L S I I G
    // Y   I   R
    // And then read line by line : "PAHNAPLSIIGYIR"
    // Write the code that will take a string and make this conversion given a number of rows
    struct p006
    {
        string convertToZigzag_0(const string& s, int n)
        {
            if (s.size() <= n)
                return s;

            string o;
            for (int i = 0; i < n; i++)
            {
                int j = i;
                while (j < s.size())
                {
                    // add the current char
                    o += s[j];

                    // for any rows other than 1st and last,
                    if (i % (n - 1))
                    {
                        // add the char that crosses in between

                        // (old code) why?
                        //int j2 = j + 2 * (n - i) - 2;

                        // jump size is (2 * n - 2), so (n - 1) is half-way in-between
                        int j2 = j + n - 1;
                        if (j2 < s.size())
                            o += s[j2];
                    }

                    // increment cursor to the next hop
                    j += 2 * n - 2;
                }
            }

            return o;
        }

        string convertToZigzag(const string& s, int n)
        {
            if ((s.size() <= n) || (n == 1))
                return s;

            vector<string> v(n);
            int vi = 0; // current row index
            int vid = 1; // direction of row index (incr/decr)
            for (int i = 0; i < s.size(); i++)
            {
                v[vi].push_back(s[i]);
                vi += vid;

                // if at either top or bottom row, change direction
                if ((vi == 0) || (vi == (n - 1)))
                    vid *= -1;
            }

            string o;
            for (int i = 0; i < n; i++)
                o += v[i];

            return o;
        }

        void test()
        {
            VERIFY("PAHNAPLSIIGYIR" == convertToZigzag_0("PAYPALISHIRING", 3));
            VERIFY("PAHNAPLSIIGYIR" == convertToZigzag("PAYPALISHIRING", 3));
            VERIFY("AB" == convertToZigzag("AB", 1));
        }
    };

    /* String to Integer */
    // LeetCode#8
    // Implement atoi to convert a string to an integer.
    // Hint: Carefully consider all possible input cases.If you want a challenge, please do not see
    // below and ask yourself what are the possible input cases.
    struct p008
    {
        int myAtoi_0(const string& s)
        {
            constexpr unsigned int mx = numeric_limits<unsigned int>::max() / 2 + 1;

            unsigned int n = 0;
            int sign = 1;
            for (int i = 0; i < s.size(); ++i)
            {
                if (s[i] == '-')
                    sign = -1;

                int d = s[i] - '0';
                if ((d < 0) || (d > 9))
                    continue;

                unsigned int m = n;

                n *= 10;
                n += d;

                if ((n < m) || (n > mx))
                    throw overflow_error("overflow");
            }

            n *= sign;
            return n;
        }

        bool isDigit(char c)
        {
            return ((c - '0') >= 0) && ((c - '0') <= 9);
        }

        bool isOverflowProne(int n, int x, bool isAddOperation)
        {
            return isAddOperation
                ? ((INT_MAX - x) < n) || ((INT_MIN + x) > n)
                : ((INT_MAX / x) < n) || ((INT_MIN / x) > n);
        }

        int myAtoi(const string& str) {
            int i = 0;
            int signum = 1;
            while ((i < str.size()) && !isDigit(str[i]))
            {
                if (str[i] == '-')
                {
                    signum = -1;
                }

                ++i;
            }

            int n = 0;
            while ((i < str.size()) && isDigit(str[i]))
            {
                int d = str[i] - '0';

                if (isOverflowProne(n, 10, false /* isAddOperation */))
                    return signum > 0 ? INT_MAX : INT_MIN;
                n *= 10;

                if (isOverflowProne(n, d, true /* isAddOperation */))
                    return signum > 0 ? INT_MAX : INT_MIN;
                n += d;

                ++i;
            }

            return n * signum;
        }

        void test()
        {
            VERIFY(INT_MAX == myAtoi("2147483647"));
            VERIFY(INT_MIN == myAtoi("-2147483648"));
            VERIFY(0 == myAtoi("0"));
            VERIFY(-1 == myAtoi("-1"));
            VERIFY(1 == myAtoi("1"));
            VERIFY(20842 == myAtoi("20842"));
            VERIFY(5 == myAtoi("05"));
            VERIFY(-519 == myAtoi("-519"));

            VERIFY(INT_MAX == myAtoi("209482029382502"));
            VERIFY(INT_MAX == myAtoi("2147483649"));
            VERIFY(INT_MIN == myAtoi("-2147483649"));
        }
    };

    /* Regular Expression Matching */
    // LeetCode#10
    // '.' Matches any single character.
    // '*' Matches zero or more of the preceding element.
    // The matching should cover the entire input string(not partial).
    // The function prototype should be :
    // bool isMatch(const char *s, const char *p)
    // Some examples :
    // isMatch("aa", "a") ? false
    // isMatch("aa", "aa") ? true
    // isMatch("aaa", "aa") ? false
    // isMatch("aa", "a*") ? true
    // isMatch("aa", ".*") ? true
    // isMatch("ab", ".*") ? true
    // isMatch("aab", "c*a*b") ? true
    struct p010
    {
        bool isRegexMatch(const string& s, const string& p)
        {
            if ((p.size() > 1) && (p[1] == '*'))
            {
                int i = 0;
                // try one or more matches
                while ((i < s.size()) && ((s[i] == p[0]) || (p[0] == '.')))
                {
                    if (isRegexMatch(s.substr(i + 1), p.substr(2)))
                        return true;

                    ++i;
                }

                // try zero matches
                return isRegexMatch(s, p.substr(2));
            }

            if ((s.size() == 0) || (p.size() == 0))
                return s.size() == p.size();

            return ((s[0] == p[0]) || (p[0] == '.')) && isRegexMatch(s.substr(1), p.substr(1));
        }

        void test()
        {
            VERIFY(isRegexMatch("aab", "c*a*b"));
            VERIFY(!isRegexMatch("aa", "a"));
            VERIFY(!isRegexMatch("aaa", "aa"));
            VERIFY(isRegexMatch("aa", "a*"));
            VERIFY(isRegexMatch("aa", ".*"));
            VERIFY(isRegexMatch("ab", ".*"));
            VERIFY(isRegexMatch("gggt", "g*t"));
            VERIFY(isRegexMatch("gggoat", ".*t"));
            VERIFY(isRegexMatch("a", "ab*"));
            VERIFY(!isRegexMatch("aaa", "a*b"));
        }
    };

    /* Longest Common Prefix */
    // LeetCode#14
    // Write a function to find the longest common prefix string amongst an array of strings.
    struct p014
    {
        string getLongestCommonPrefix(const vector<string>& v)
        {
            if (v.empty())
                return "";

            if (v.size() < 2)
                return v[0];

            int j = 0;
            while (true)
            {
                for (int i = 1; i < v.size(); ++i)
                {
                    if (j >= v[i].size())
                        return v[i];

                    if (v[i][j] != v[i - 1][j])
                        return v[i].substr(0, j);
                }

                ++j;
            }
        }

        void test()
        {
            VERIFY("foo" == getLongestCommonPrefix({ "fooa", "foobc", "foodang", "foomin" }));
            VERIFY("fo" == getLongestCommonPrefix({ "foo", "foobc", "fo", "foomin" }));
        }
    };

    /* 3Sum Closest */
    // LeetCode#16
    // Given an array S of n integers, find three integers in S such that the sum is closest to a
    // given number, target. Return the sum of the three integers. You may assume that each input
    // would have exactly one solution.
    // For example, Given array S = {-1 2 1 -4}, and target = 1. The sum that is closest to the
    // target is 2. (-1 + 2 + 1 = 2).
    struct p016
    {
        // if sorting is not allowed and/or triplet index asked.
        int get3SumClosest_0(const vector<int>& v, int x)
        {
            int cx = numeric_limits<int>::max();
            tuple<int, int, int> ct;

            for (int j = 1; j < v.size() - 1; ++j)
                for (int i = 0; i < j; ++i)
                    for (int k = v.size() - 1; k > j; --k)
                    {
                        int cs = v[i] + v[j] + v[k];
                        if (abs(cs - x) < abs(cx - x))
                        {
                            cx = cs;
                            get<0>(ct) = i;
                            get<1>(ct) = j;
                            get<2>(ct) = k;
                        }
                    }

            return cx;
        }

        int get3SumClosest(const vector<int>& a, int x)
        {
            vector<int> v(a);
            sort(v.begin(), v.end());

            // for each middle element 1..(v.size() - 2) i.e. all possible middle indexes leaving
            // at least 1 element on each side.
            int cx = numeric_limits<int>::max();
            for (int j = 1; j < v.size() - 1; ++j)
            {
                int i = 0;
                int k = v.size() - 1;
                while ((i < j) && (j < k))
                {
                    int cs = v[i] + v[j] + v[k];
                    int d = abs(cs - x);

                    // if we have reached target, no point looking any further
                    if (d == 0)
                        return cs;

                    // have we got any closer than before?
                    if (d < abs(cx - x))
                        cx = cs;

                    // adjust the "window" based on current sum, e.g. if less than target, "slide"
                    // it forward (to include larger value as the array is sorted).
                    if (cs < x)
                        ++i;
                    else
                        --k;
                }
            }

            return cx;
        }

        void test()
        {
            VERIFY(2 == get3SumClosest({ -1, 2, 1, -4 }, 1));
        }
    };

    /* Letter Combinations of a Phone Number */
    // LeetCode#17
    struct p017
    {
        void getPhoneNumberTextsbt(vector<string>& r, string& c, int i, string& s)
        {
            static unordered_map<char, vector<char>> lt = {
                { '0',{ '+' } },
                { '1',{ '1' } },
                { '2',{ 'a', 'b', 'c' } },
                { '3',{ 'd', 'e', 'f' } },
                { '4',{ 'g', 'h', 'i' } },
                { '5',{ 'j', 'k', 'l' } },
                { '6',{ 'm', 'n', 'o' } },
                { '7',{ 'p', 'q', 'r', 's' } },
                { '8',{ 't', 'u', 'v' } },
                { '9',{ 'w', 'x', 'y', 'z' } },
            };

            if (i >= s.size())
            {
                r.push_back(c);
                return;
            }

            for (int j = 0; j < lt[s[i]].size(); ++j)
            {
                c.push_back(lt[s[i]][j]);
                getPhoneNumberTextsbt(r, c, i + 1, s);
                c.pop_back();
            }
        }

        vector<string> getPhoneNumberTexts(string s)
        {
            vector<string> r;
            string c;
            getPhoneNumberTextsbt(r, c, 0, s);

            return r;
        }

        void test()
        {
            auto v = getPhoneNumberTexts("6866");
            LOG(v.size());
            printVector(v);
        }
    };

    /* Valid Parentheses */
    // LeetCode#20
    // Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if
    // the input string is valid. The brackets must close in the correct order, "()" and "()[]{}"
    // are all valid but "(]" and "([)]" are not.
    struct p020
    {
        bool isValidParens(const string& s)
        {
            static string op = "[{(";
            static string cp = "]})";

            stack<char> st;
            for (int i = 0; i < s.size(); ++i)
            {
                if (op.find_first_of(s[i]) != string::npos)
                {
                    st.push(s[i]);
                }
                else if (cp.find_first_of(s[i]) != string::npos)
                {
                    if (st.empty())
                        return false;

                    int j = cp.find_first_of(s[i]);
                    if (op[j] != st.top())
                        return false;

                    st.pop();
                }
            }

            return st.empty();
        }

        void test()
        {
            VERIFY(isValidParens(""));
            VERIFY(isValidParens("()"));
            VERIFY(isValidParens("[{()}]"));
            VERIFY(!isValidParens("[{()}"));
            VERIFY(!isValidParens("("));
            VERIFY(isValidParens("a(b)c"));
            VERIFY(isValidParens("abc"));
            VERIFY(!isValidParens(")"));
            VERIFY(!isValidParens("[(])"));
            VERIFY(!isValidParens(")("));
        }
    };

    /* Merge K Sorted Lists */
    // LeetCode#21
    struct p021
    {
        vector<int> mergeKWay(const vector<vector<int>>& v)
        {
            // Priority queue item which stores individual min values from i'th row and j'th position.
            typedef pair<int, pair<int, int>> pq_item;

            // use greater to build a max heap.
            priority_queue<pq_item, vector<pq_item>, greater<pq_item>> pq;
            for (int i = 0; i < v.size(); ++i)
                if (v[i].size() > 0)
                    pq.emplace(v[i][0], make_pair(i, 0));

            vector<int> a;
            while (!pq.empty())
            {
                auto p = pq.top();
                pq.pop();

                a.push_back(p.first);

                int i = p.second.first;
                int j = p.second.second;

                if (++j < v[i].size())
                    pq.emplace(v[i][j], make_pair(i, j));
            }

            return a;
        }

        void test()
        {
            vector<vector<int>> v;
            vector<int> mv;

            int n = getRandom(2, 5);
            for (int i = 0; i < n; ++i)
            {
                auto a = getRandom(0, 25, getRandom(0, 10));
                mv.insert(mv.begin(), a.begin(), a.end());

                sort(a.begin(), a.end());
                v.push_back(a);
            }

            auto ma = mergeKWay(v);
            //printVector(v);
            //printVector(ma);

            sort(mv.begin(), mv.end());
            VERIFY(mv == ma);

            v = vector<vector<int>>{
                { 17, 24 },
                {},
                { 7, 14, 15, 17 },
                { 7, 14, 15, 17, 17, 24 },
                { 17, 24 },
            };

            VERIFY(vector<int>({ 7, 7, 14, 14, 15, 15, 17, 17, 17, 17, 17, 24, 24, 24 }) == mergeKWay(v));
        }
    };

    /* Generate Parentheses */
    // LeetCode#22
    // Given n pairs of parentheses, write a function to generate all combinations of well-formed 
    // parentheses. For example, given n = 3, a solution set is:
    // [
    //   "((()))",
    //   "(()())",
    //   "(())()",
    //   "()(())",
    //   "()()()"
    // ]
    struct p022
    {
        void generateParenthesisHelper(vector<string>& r, string s, int lc, int rc)
        {
            //LOG(s << ":" << lc << "," << rc);

            if (lc > rc)
                return;

            if ((lc == 0) && (rc == 0))
            {
                r.push_back(s);
                return;
            }

            if (lc > 0)
                generateParenthesisHelper(r, s + "(", lc - 1, rc);

            if (rc > 0)
                generateParenthesisHelper(r, s + ")", lc, rc - 1);
        }

        vector<string> generateParenthesis(int n) {
            vector<string> r;
            generateParenthesisHelper(r, "", n, n);

            return r;
        }

        void test()
        {
            VERIFY(vector<string>({
                "((()))",
                "(()())",
                "(())()",
                "()(())",
                "()()()"
            }) == generateParenthesis(3));
        }
    };

    /* Reverse Nodes in k-Group */
    // LeetCode#25
    // Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.
    // If the number of nodes is not a multiple of k then left-out nodes in the end should remain as 
    // it is. You may not alter the values in the nodes, only nodes itself may be changed. Only 
    // constant memory is allowed. For example,
    // Given this linked list: 1->2->3->4->5
    // For k = 2, you should return: 2->1->4->3->5
    // For k = 3, you should return: 3->2->1->4->5
    struct p025
    {
        void reverseList(PListNode<int> h, PListNode<int> t)
        {
            PListNode<int> p;
            PListNode<int> c = h;
            while (p != t)
            {
                PListNode<int> n = c->next;
                c->next = p;
                p = c;
                c = n;
            }
        }

        PListNode<int> reverseKGroup_0(PListNode<int> head, int k)
        {
            PListNode<int> p;
            PListNode<int> h = head;

            while (h)
            {
                // find the tail
                PListNode<int> t = h;
                int i = k;
                while ((--i > 0) && t)
                    t = t->next;

                // less than k nodes left
                if (!t)
                    return head;

                // save the right link
                PListNode<int> n = t->next;

                // reverse (head, tail)
                reverseList(h, t);

                // fix the left link
                if (p)
                    p->next = t;
                else
                    head = t;

                // fix the right link
                h->next = n;

                // roll-over (note, after the reversal, h/t switched role)
                p = h;
                h = n;
            }

            return head;
        }

        PListNode<int> reverseKGroup(PListNode<int> head, int k)
        {
            // find the tail node at (k + 1)'th position
            PListNode<int> t = head;
            for (int i = 0; i < k; ++i)
            {
                // not enough nodes
                if (!t)
                    return head;

                t = t->next;
            }

            PListNode<int> p = t;
            PListNode<int> c = head;
            while (c != t)
            {
                PListNode<int> n = c->next;
                c->next = p;
                p = c;
                c = n;
            }

            head->next = reverseKGroup(t, k);
            return p;
        }

        void test()
        {
            VERIFY(convert_from(reverseKGroup(convert_to(vector<int>{ 1, 2, 3, 4, 5 }), 2)) ==
                vector<int>({ 2, 1, 4, 3, 5 }));
            VERIFY(convert_from(reverseKGroup(convert_to(vector<int>{ 1, 2, 3, 4, 5 }), 3)) ==
                vector<int>({ 3, 2, 1, 4, 5 }));
            VERIFY(convert_from(reverseKGroup(convert_to(vector<int>{ 1, 2, 3, 4 }), 2)) ==
                vector<int>({ 2, 1, 4, 3 }));
        }
    };

    /* Remove Duplicates from Sorted Array */
    // LeetCode#26
    // Given a sorted array, remove the duplicates in place such that each element appear only once
    // and return the new length. Do not allocate extra space for another array, you must do this in
    // place with constant memory. For example, given input array A = [1, 1, 2], your function
    // should return length = 2, and A is now [1, 2].
    struct p026
    {
        string removeDuplicates(const string& str)
        {
            // assert(sorted(str))
            string s(str);

            int rp = 1;
            int wp = 1;
            while (rp < s.size())
            {
                if (s[wp - 1] != s[rp])
                    s[wp++] = s[rp];

                ++rp;
            }

            s = s.substr(0, wp);
            return s;
        }

        string removeDuplicates(const string& str, int maxAllowed)
        {
            // assert(sorted(str))
            string s(str);

            int rp = 1;
            int wp = 1;
            int seenCount = 1;
            while (rp < s.size())
            {
                if (s[wp - 1] != s[rp])
                    seenCount = 0;

                if (seenCount < maxAllowed)
                {
                    s[wp++] = s[rp];
                    ++seenCount;
                }

                ++rp;
            }

            s = s.substr(0, wp);
            return s;
        }

        void test()
        {
            VERIFY("abc" == removeDuplicates("abc"));
            VERIFY("abc" == removeDuplicates("aabc"));
            VERIFY("abc" == removeDuplicates("abbbc"));
            VERIFY("a" == removeDuplicates("aaa"));
            VERIFY("" == removeDuplicates(""));
            VERIFY("abc" == removeDuplicates("abccc"));

            VERIFY("abc" == removeDuplicates("abc", 1));
            VERIFY("abc" == removeDuplicates("aabc", 1));
            VERIFY("abc" == removeDuplicates("abbbc", 1));
            VERIFY("a" == removeDuplicates("aaa", 1));
            VERIFY("" == removeDuplicates("", 1));
            VERIFY("abc" == removeDuplicates("abccc", 1));

            VERIFY("aabc" == removeDuplicates("aabc", 2));
            VERIFY("aabbc" == removeDuplicates("aabbbc", 2));
            VERIFY("abcc" == removeDuplicates("abcccc", 2));
            VERIFY("aa" == removeDuplicates("aaa", 2));
            VERIFY("" == removeDuplicates("", 2));
        }
    };

    /* Implement strStr */
    // LeetCode#28
    // Returns the index of the first occurrence of needle in haystack, or -1 if needle is not part
    // of haystack.
    struct p028
    {
        vector<int> getKMPTable(const string& w)
        {
            // T[i] => length of largest prefix ending at w[i - 1]. Another way to think about it 
            // is that index x can be "snapped" to T[x] such that all the previous characters line
            // up perfectly with a copy of itself pushed at least a single position to the right
            // (Notice the offset part, otherwise T[x] could simply be set to x...). That's why the
            // table T comes handy in matching strings. When a match fails at some position between
            // the text and the pattern, we can use T[.] to do the next best line up and continue
            // matching from that position.
            vector<int> T(w.size());

            // By definition, as index 0 and 1  doesn't contain any suffix. -1 is set to simplify 
            // the computation of the table.
            T[0] = -1;
            T[1] = 0;

            int j = 0;
            for (int i = 2; i < w.size(); ++i)
            {
                // j points to the last set value of computed during [i - 1] i.e. largest prefix
                // length ending at [i - 2]. So we if the previous char (at [i - 1]) matches [j]
                // (note, length value happens to point to the next char position because of 0-value
                // indexing), we simply skip to increment the match count.
                while ((j >= 0) && (w[j] != w[i - 1]))
                    j = T[j];

                j += 1;
                T[i] = j;
            }

            return T;
        }

        int getSubstringIndex(const string& s, const string& p)
        {
            vector<int> t = getKMPTable(p);

            int i = 0;
            while (i < s.size() - p.size() + 1)
            {
                int j = 0;
                while (j < p.size() && (s[i + j] == p[j]))
                {
                    if (++j == p.size())
                        return i;
                }

                i += j - t[j];
            }

            return -1;
        }

        void test()
        {
            VERIFY(14 == getSubstringIndex("A BAOBA ISN'T BAOBAB.", "BAOBAB"));
        }
    };

    /* Substring with Concatenation of All Words */
    // LeetCode#30
    // You are given a string, s, and a list of words, words, that are all of the same length. Find
    // all starting indices of substring(s) in s that is a concatenation of each word in words
    // exactly once and without any intervening characters. For example,
    // Given: s = "barfoothefoobarman" & words = ["foo", "bar"], return[0, 9].
    struct p030
    {
        pair<int, int> getMaxWordSubstring(const string& s, const unordered_set<string>& w)
        {
            pair<int, int> r = { -1, -1 };

            int wlen = w.begin()->size();
            int i = -1;
            for (int j = wlen - 1; j < s.size(); j += wlen)
            {
                string cw = s.substr(j - wlen + 1, wlen);
                if (w.find(cw) == w.end())
                {
                    if (i >= 0)
                    {
                        if ((j - i - wlen) > (r.second - r.first + 1))
                        {
                            r = { i, j - wlen };
                            i = -1;
                        }
                    }
                }
                else
                {
                    if (i < 0)
                        i = j - wlen + 1;
                }
            }

            return r;
        }

        void test()
        {
            VERIFY(make_pair(0, 5) == getMaxWordSubstring(
                "barfoothefoobarman", { { "foo" },{ "bar" } }));
        }
    };

    /* Next Permutation */
    // LeetCode#31
    // Given a set of integers, determine the next permutation sequence.
    struct p031
    {
        /*
        vector<int> getNextPermutation(const vector<int>& v)
        {
        vector<int> iv(v.size());
        iota(iv.begin(), iv.end(), 0);

        int k = v.size() - 1;
        while ((k > 0) && (v[k - 1] >= v[k]))
        --k;

        if (k == 0)
        return{};

        swap(iv[k - 1], iv[k]);
        sort(iv.begin() + k, iv.end(), [&v](auto& i, auto& j)
        {
        return v[i] < v[j];
        });

        return iv;
        }
        */

        vector<int> getNextPermutation(vector<int> v)
        {
            // find the right-most ordered pair
            int i = v.size() - 1;
            while ((i > 0) && (v[i - 1] >= v[i]))
                --i;

            // if not found i.e. the sequence is in complete reverse order, we have reached the end
            if (i == 0)
                return{};

            // swap the smaller element with the right-most larger one. No need to check bounds as
            // the larger of the pair will act as the sentinel.
            int j = v.size() - 1;
            while (v[j] < v[i - 1])
                --j;

            // once found, swap and sort the rest of the sequence
            swap(v[i - 1], v[j]);
            sort(v.begin() + i, v.end());

            return v;
        }

        void test()
        {
            //VERIFY(vector<int>({ 0, 2, 1 }) == getNextPermutation({ 2, 1, 3 }));
            VERIFY(vector<int>({ 1, 2, 0 }) == getNextPermutation({ 1, 0, 2 }));

            vector<int> v = { 1, 2, 3 };
            do
            {
                printVector(v);
                v = getNextPermutation(v);
            } while (!v.empty());
        }
    };

    /* Longest Valid Parentheses */
    // LeetCode#32
    // Given a string containing just the characters '(' and ')', find the length of the longest
    // valid (well-formed) parentheses substring. For "(()", the longest valid parentheses substring
    // is "()", which has length = 2. Another example is ")()())", where the longest valid
    // parentheses substring is "()()", which has length = 4.
    struct p032
    {
        int getLongestValidParens(const string& s)
        {
            static string op = "[{(";
            static string cp = "]})";

            int x = 0;
            stack<pair<char, int>> st;
            for (int i = 0; i < s.size(); ++i)
            {
                if (op.find_first_of(s[i]) != string::npos)
                {
                    st.push({ s[i], i });
                }
                else if (cp.find_first_of(s[i]) != string::npos)
                {
                    int j = cp.find_first_of(s[i]);
                    if (st.empty() || (op[j] != st.top().first))
                    {
                        st.push({ s[i], i });
                    }
                    else
                    {
                        st.pop();
                        x = max(x, st.empty() ? i + 1 : i - st.top().second);
                    }
                }
            }

            return x;
        }

        void test()
        {
            VERIFY(0 == getLongestValidParens(""));
            VERIFY(2 == getLongestValidParens("()"));
            VERIFY(4 == getLongestValidParens("((())"));
            VERIFY(4 == getLongestValidParens("{}[()()["));
        }
    };

    /* Search for a Range */
    // LeetCode#34
    // Given a sorted array of integers, find the starting and ending position of a given target
    // value. Your algorithm's runtime complexity must be in the order of O(log n). If the target is
    // not found in the array, return [-1, -1]. For example, given [5, 7, 7, 8, 8, 10] and target
    // value 8, return [3, 4].
    struct p034
    {
        int bsearchSkewed(const vector<int>& v, int target, bool findLeftmost)
        {
            int lo = 0;
            int hi = v.size() - 1;
            while (lo <= hi)
            {
                int mid = lo + (hi - lo) / 2;

                // *Note* depending left-most or right-most search, we choose left or right
                // partition in case of equality resp.
                if (findLeftmost)
                {
                    if ((v[mid] == target) && ((mid == 0) || (v[mid - 1] < target)))
                        return mid;
                    
                    if (v[mid] >= target)
                        hi = mid - 1;
                    else
                        lo = mid + 1;
                }
                else
                {
                    if ((v[mid] == target) && ((mid == v.size() - 1) || (v[mid + 1] > target)))
                        return mid;

                    if (v[mid] <= target)
                        lo = mid + 1;
                    else
                        hi = mid - 1;
                }
            }

            return -1;
        }

        pair<int, int> getRange(const vector<int>& v, int x)
        {
            return{
                bsearchSkewed(v, x, true),
                bsearchSkewed(v, x, false)
            };
        }

        void test()
        {
            VERIFY((pair<int, int>{3, 4}) == getRange(vector<int>{ 5, 7, 7, 8, 8, 10 }, 8));
            VERIFY((pair<int, int>{0, 1}) == getRange(vector<int>{ 2, 2 }, 2));
        }
    };

    /* Valid Sudoku */
    // LeetCode#36
    // Check whether a 9 x 9 2D array representing a partially completed Sudoku is valid.
    // Specifically, check that no row, column, and 3 x 3 2D subarray contains duplicates.
    // A 0-value in the 2D array indicates that entry is blank; every other entry is in [1,9].
    // (EPI Problem 6.14)
    struct p036
    {
        bool isValidSudoku(const vector<vector<int>>& b)
        {
            // assert 9x9 array
            // assert(b.size() == 9);
            // assert(b[0].size() == 9);

            // column/row/square lookup vectors
            vector<vector<bool>> r(9, vector<bool>(9, false));
            vector<vector<bool>> c(9, vector<bool>(9, false));
            vector<vector<bool>> s(9, vector<bool>(9, false));

            for (int i = 0; i < b.size(); ++i)
            {
                for (int j = 0; j < b[0].size(); ++j)
                {
                    if (b[i][j] == 0)
                        continue;

                    int x = b[i][j];
                    int k = (i / 3) * 3 + (j / 3);

                    if (r[i][x] || c[j][x] || s[k][x])
                        return false;

                    r[i][x] = c[j][x] = s[k][x] = true;
                }
            }

            return true;
        }
    };

    /* Sudoku Solver */
    // LeetCode#37
    struct p037
    {
        vector<vector<int>> genSudokuBoard(double fillFactor = 0.25)
        {
            vector<vector<int>> b(9, vector<int>(9));

            vector<vector<bool>> rset(9, vector<bool>(9));
            vector<vector<bool>> cset(9, vector<bool>(9));
            vector<vector<bool>> sset(9, vector<bool>(9));

            auto eng = default_random_engine(random_device()());
            auto dist = uniform_int_distribution<>(1, 9);

            for (int i = 0; i < b.size(); ++i)
                for (int j = 0; j < b[0].size(); ++j)
                {
                    if (dist(eng) > dist.max() * fillFactor)
                        continue;

                    int k = (i / 3) * 3 + (j / 3);

                    int maxtries = 9;
                    do
                    {
                        int n = dist(eng);
                        if (!rset[i][n - 1] && !cset[j][n - 1] && !sset[k][n - 1])
                        {
                            b[i][j] = n;
                            rset[i][n - 1] = cset[j][n - 1] = sset[k][n - 1] = true;
                        }
                    } while ((b[i][j] == 0) && --maxtries);
                }

            return b;
        }

        bool isValidSudokuPosition(vector<vector<int>>& b,
            vector<vector<bool>>& rset,
            vector<vector<bool>>& cset,
            vector<vector<bool>>& sset,
            int i,
            int j)
        {
            int n = b[i][j];
            int k = (i / 3) * 3 + (j / 3);
            return (n != 0) &&
                !rset[i][n - 1] &&
                !cset[j][n - 1] &&
                !sset[k][n - 1];
        }

        bool isValidSudokuBoard(vector<vector<int>>& b, bool isComplete = false)
        {
            vector<vector<bool>> rset(9, vector<bool>(9));
            vector<vector<bool>> cset(9, vector<bool>(9));
            vector<vector<bool>> sset(9, vector<bool>(9));

            for (int i = 0; i < b.size(); ++i)
                for (int j = 0; j < b[0].size(); ++j)
                {
                    int n = b[i][j];
                    if (n == 0)
                    {
                        if (isComplete)
                            return false;

                        continue;
                    }

                    if (!isValidSudokuPosition(b, rset, cset, sset, i, j))
                        return false;

                    int k = (i / 3) * 3 + (j / 3);
                    rset[i][n - 1] = cset[j][n - 1] = sset[k][n - 1] = true;
                }

            return true;
        }

        bool solveSudokuBoardBT_old(vector<vector<int>>& b,
            vector<vector<bool>>& rset,
            vector<vector<bool>>& cset,
            vector<vector<bool>>& sset,
            int i,
            int j)
        {
            bool completed = true;
            for (int ii = i; ii < b.size(); ++ii)
                for (int jj = 0; jj < b[0].size(); ++jj)
                {
                    if (b[ii][jj] != 0)
                        continue;

                    completed = false;
                    int kk = (ii / 3) * 3 + (jj / 3);
                    for (int n = 1; n <= 9; ++n)
                    {
                        if (rset[ii][n - 1] || cset[jj][n - 1] || sset[kk][n - 1])
                            continue;

                        b[ii][jj] = n;
                        rset[ii][n - 1] = cset[jj][n - 1] = sset[kk][n - 1] = true;

                        if (solveSudokuBoardBT(b, rset, cset, sset))
                            return true;

                        rset[ii][n - 1] = cset[jj][n - 1] = sset[kk][n - 1] = false;
                        b[ii][jj] = 0;
                    }

                    if (b[ii][jj] == 0)
                        return false;
                }

            return completed && isValidSudokuBoard(b, true);
        }

        bool solveSudokuBoardBT(vector<vector<int>>& b,
            vector<vector<bool>>& rset,
            vector<vector<bool>>& cset,
            vector<vector<bool>>& sset)
        {
            for (int i = 0; i < b.size(); ++i)
                for (int j = 0; j < b[0].size(); ++j)
                {
                    if (b[i][j] != 0)
                        continue;

                    // find the corresp. square#
                    int k = (i / 3) * 3 + (j / 3);

                    for (int n = 1; n <= 9; ++n)
                    {
                        // skip over invalid options
                        if (rset[i][n - 1] || cset[j][n - 1] || sset[k][n - 1])
                            continue;

                        // fill the spot with the candidate number
                        b[i][j] = n;
                        rset[i][n - 1] = cset[j][n - 1] = sset[k][n - 1] = true;

                        // and recurse...
                        if (solveSudokuBoardBT(b, rset, cset, sset))
                            return true;

                        // didn't work, backtrack
                        rset[i][n - 1] = cset[j][n - 1] = sset[k][n - 1] = false;
                        b[i][j] = 0;
                    }

                    return false;
                }

            return isValidSudokuBoard(b, true);
        }

        bool solveSudokuBoard(vector<vector<int>>& b)
        {
            vector<vector<bool>> rset(9, vector<bool>(9));
            vector<vector<bool>> cset(9, vector<bool>(9));
            vector<vector<bool>> sset(9, vector<bool>(9));

            // assert(isValidSudokuBoard(b));
            for (int i = 0; i < b.size(); ++i)
                for (int j = 0; j < b[0].size(); ++j)
                {
                    int n = b[i][j];
                    if (n == 0)
                        continue;

                    rset[i][n - 1] = cset[j][n - 1] = true;

                    int k = (i / 3) * 3 + (j / 3);
                    sset[k][n - 1] = true;
                }

            //return solveSudokuBoardBT_old(b, rset, cset, sset, 0, 0);
            return solveSudokuBoardBT(b, rset, cset, sset);
        }

        void test()
        {
            vector<vector<int>> b = {
                { 0, 0, 0, 2, 6, 0, 7, 0, 1 },
                { 6, 8, 0, 0, 7, 0, 0, 9, 0 },
                { 1, 9, 0, 0, 0, 4, 5, 0, 0 },
                { 8, 2, 0, 1, 0, 0, 0, 4, 0 },
                { 0, 0, 4, 6, 0, 2, 9, 0, 0 },
                { 0, 5, 0, 0, 0, 3, 0, 2, 8 },
                { 0, 0, 9, 3, 0, 0, 0, 7, 4 },
                { 0, 4, 0, 0, 5, 0, 0, 3, 6 },
                { 7, 0, 3, 0, 1, 8, 0, 0, 0 },
            };

            printVector(b);
            VERIFY(isValidSudokuBoard(b));

            b[0][0] = 1;
            VERIFY(!isValidSudokuBoard(b));

            b[0][0] = 0;
            VERIFY(solveSudokuBoard(b));
            printVector(b);

            for (int i = 0; i < 10; ++i)
            {
                b = genSudokuBoard(0.3);
                printVector(b);

                if (solveSudokuBoard(b))
                {
                    printVector(b);
                    break;
                }

                LOG("unsolvable#" << i);
            }
        }
    };

    /* Count and Say */
    // LeetCode#38
    // The count - and-say sequence is the sequence of integers beginning as follows:
    // 1, 11, 21, 1211, 111221, ...
    // 1 is read off as "one 1" or 11.
    // 11 is read off as "two 1s" or 21.
    // 21 is read off as "one 2, then one 1" or 1211.
    // Given an integer n, generate the nth sequence.
    struct p038
    {
        string getCountAndSays(int n)
        {
            if (n <= 0)
                return "";

            string r = "1";
            while (--n > 0)
            {
                string s = r;
                r.clear();

                int count = 1;
                for (int i = 1; i < s.size(); ++i)
                {
                    if (s[i] != s[i - 1])
                    {
                        r += to_string(count) + s.substr(i - 1, 1);
                        count = 0;
                    }

                    ++count;
                }

                r += to_string(count) + s.substr(s.size() - 1, 1);
            }

            return r;
        }

        void test()
        {
            VERIFY("1" == getCountAndSays(1));
            VERIFY("111221" == getCountAndSays(5));
            VERIFY("312211" == getCountAndSays(6));

            VERIFY("" == getCountAndSays(0));
        }
    };

    /* Combination Sum */
    // Given a set of candidate numbers (C) and a target number (T), find all unique combinations in 
    // C where the candidate numbers sums to T.
    //
    // The same repeated number may be chosen from C unlimited number of times.
    // 
    // Note:
    //  All numbers (including target) will be positive integers.
    //  The solution set must not contain duplicate combinations.
    // For example, given candidate set [2, 3, 6, 7] and target 7,
    // A solution set is: { [7], [2, 2, 3] }
    struct p039
    {
        void combinationSumHelper(vector<vector<int>>& r, vector<int>& v, vector<int>& c, int i, int t)
        {
            if (t == 0)
            {
                r.push_back(v);
                return;
            }

            for (int j = i; j < c.size(); ++j)
            {
                if (t < c[j])
                    break;

                v.push_back(c[j]);
                combinationSumHelper(r, v, c, j, t - c[j]);
                v.pop_back();
            }
        }

        vector<vector<int>> combinationSum(vector<int> candidates, int target)
        {
            sort(candidates.begin(), candidates.end());

            vector<vector<int>> result;
            vector<int> v;
            combinationSumHelper(result, v, candidates, 0, target);

            return result;
        }

        void test()
        {
            VERIFY(vector<vector<int>>({ { 2, 2, 3 }, { 7 } }) == combinationSum({ 2, 3, 6, 7 }, 7));
        }
    };

    /* Combination Sum II */
    // Given a set of candidate numbers (C) and a target number (T), find all unique combinations in 
    // C where the candidate numbers sums to T.
    //
    // The same repeated number can be chosen from C only *once*.
    //
    // Note:
    //  All numbers (including target) will be positive integers.
    //  The solution set must not contain duplicate combinations.
    // For example, given candidate set [10, 1, 2, 7, 6, 1, 5] and target 8,
    // A solution set is: { [1, 1, 6], [1, 2, 5], [1, 7], [2, 6] }
    struct p040
    {
        void combinationSumHelper(vector<vector<int>>& r, vector<int>& v, vector<int>& c, int i, int t)
        {
            if (t == 0)
            {
                r.push_back(v);
                return;
            }

            for (int j = i; j < c.size(); ++j)
            {
                if (t < c[j])
                    break;

                if ((j > i) && (c[j] == c[j - 1]))
                    continue;

                v.push_back(c[j]);
                combinationSumHelper(r, v, c, j, t - c[j]);
                v.pop_back();
            }
        }

        vector<vector<int>> combinationSum(vector<int> candidates, int target)
        {
            sort(candidates.begin(), candidates.end());

            vector<vector<int>> result;
            vector<int> v;
            combinationSumHelper(result, v, candidates, 0, target);

            return result;
        }

        void test()
        {
            VERIFY(vector<vector<int>>({ { 2, 2, 3 },{ 7 } }) == combinationSum({ 2, 3, 6, 7 }, 7));
        }
    };

    /* First Missing Positive */
    // LeetCode#41
    // Given an unsorted integer array, find the first missing positive integer. For example, given
    // [1, 2, 0] return 3 and [3, 4, -1, 1] return 2.
    // Your algorithm should run in O(n) time and uses constant space.
    struct p041
    {
        /* Not O(n) */
        int getFirstMissingPositive_0(vector<int> v)
        {
            sort(v.begin(), v.end());

            int i = 0;
            while ((i < v.size()) && (v[i] <= 0))
                ++i;

            int j = i;
            while ((i < v.size()) && (v[i] == (v[j] + i - j)))
                ++i;

            return v[i - 1] + 1;
        }

        int getFirstMissingPositive(vector<int> v)
        {
            for (int i = 0; i < v.size(); ++i)
            {
                while ((v[i] > 0) && (v[i] <= v.size()) && (v[v[i] - 1] != v[i]))
                    swap(v[v[i] - 1], v[i]);
            }

            for (int i = 0; i < v.size(); ++i)
            {
                if (v[i] != (i + 1))
                    return i + 1;
            }

            return v.size() + 1;
        }

        void test()
        {
            VERIFY(3 == getFirstMissingPositive({ 1, 2, 0 }));
            VERIFY(2 == getFirstMissingPositive({ 3, 4, -1, 1 }));
            VERIFY(1 == getFirstMissingPositive({ 0 }));
            VERIFY(2 == getFirstMissingPositive({ 1 }));
            VERIFY(1 == getFirstMissingPositive({ -4 }));
            VERIFY(1 == getFirstMissingPositive({ -4, 1000 }));
            VERIFY(3 == getFirstMissingPositive({ 0, 4, 2, 1 }));
        }
    };

    /* Trapping Rain Water */
    // LeetCode#42
    // Given n non-negative integers representing an elevation map where the width of each bar is 1, 
    // compute how much water it is able to trap after raining.
    // For example, 
    // Given [0,1,0,2,1,0,1,3,2,1,2,1], return 6.
    struct p042
    {
        int trap(const vector<int>& height)
        {
            if (height.empty())
                return 0;

            vector<int> lmx(height.size());
            lmx[0] = height[0];
            for (int i = 1; i < lmx.size(); ++i)
                lmx[i] = max(lmx[i - 1], height[i]);

            vector<int> rmx(height.size());
            rmx[rmx.size() - 1] = height[height.size() - 1];
            for (int i = rmx.size() - 2; i >= 0; --i)
                rmx[i] = max(rmx[i + 1], height[i]);

            int r = 0;
            for (int i = 0; i < height.size(); ++i)
                r += max(min(lmx[i], rmx[i]) - height[i], 0);

            return r;
        }

        void test()
        {
            VERIFY(0 == trap({}));
            VERIFY(6 == trap({ 0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1 }));
            VERIFY(2 == trap({ 2, 0, 2 }));
        }
    };

    /* Wildcard Matching */
    // LeetCode#44
    // '?' Matches any single character.
    // '*' Matches any sequence of characters(including the empty sequence).
    //
    // The matching should cover the entire input string(not partial).
    //
    // The function prototype should be :
    // bool isMatch(const char *s, const char *p)
    //
    // Some examples :
    // isMatch("aa", "a") ? false
    // isMatch("aa", "aa") ? true
    // isMatch("aaa", "aa") ? false
    // isMatch("aa", "*") ? true
    // isMatch("aa", "a*") ? true
    // isMatch("ab", "?*") ? true
    // isMatch("aab", "c*a*b") ? false
    struct p044
    {
        bool isWildcardMatch(const string& s, const string& p)
        {
            // check for a * first in order to account for matches with empty text and * pattern
            if ((p.size() > 0) && (p[0] == '*'))
            {
                int i = 0;
                // note, i needs to go all the way down to s.size() to try matching empty text
                while (i <= s.size())
                {
                    if (isWildcardMatch(s.substr(i), p.substr(1)))
                        return true;

                    ++i;
                }

                return false;
            }

            // if either text/pattern is empty, match only if both are empty
            if ((s.size() == 0) || (p.size() == 0))
                return s.size() == p.size();

            // as long as current positions match, recurse to the next set
            return ((s[0] == p[0]) || (p[0] == '?')) && isWildcardMatch(s.substr(1), p.substr(1));
        }

        bool isWildcardMatchDP(const string& s, const string& p)
        {
            if ((s.size() == 0) || (p.size() == 0))
                return s.size() == p.size();

            vector<vector<bool>> dp(p.size() + 1, vector<bool>(s.size() + 1));
            dp[0][0] = true;

            // pattern MUST be on the outer loop to account for * only patterns which should match
            // any text
            for (int i = 1; i < dp.size(); ++i)
            {
                for (int j = 1; j < dp[0].size(); ++j)
                {
                    dp[i][j] = false;
                    if (p[i - 1] == '*')
                    {
                        for (int k = j; k >= 0; --k)
                        {
                            if (dp[i - 1][k])
                            {
                                dp[i][j] = true;
                                break;
                            }
                        }
                    }
                    else if ((p[i - 1] == s[j - 1]) || (p[i - 1] == '?'))
                    {
                        dp[i][j] = dp[i - 1][j - 1];
                    }
                }
            }

            //printVector(dp);
            return dp.back().back();
        }

        void test()
        {
            VERIFY(!isWildcardMatch("aa", "a"));
            VERIFY(isWildcardMatch("aa", "aa"));
            VERIFY(!isWildcardMatch("aaa", "aa"));
            VERIFY(isWildcardMatch("aa", "*"));
            VERIFY(isWildcardMatch("aa", "a*"));
            VERIFY(!isWildcardMatch("aa", "a*b"));
            VERIFY(isWildcardMatch("ab", "?*"));
            VERIFY(!isWildcardMatch("aab", "c*a*b"));
            VERIFY(isWildcardMatch("guilt", "g*t"));
            VERIFY(!isWildcardMatch("guilty", "g*t"));
            VERIFY(!isWildcardMatch("a", "b"));
            VERIFY(isWildcardMatch("ab", "ab*"));
            VERIFY(isWildcardMatch("guilt", "g*t"));
            VERIFY(isWildcardMatch("g", "g*"));

            VERIFY(!isWildcardMatchDP("aa", "a"));
            VERIFY(isWildcardMatchDP("aa", "aa"));
            VERIFY(!isWildcardMatchDP("aaa", "aa"));
            VERIFY(isWildcardMatchDP("aa", "*"));
            VERIFY(isWildcardMatchDP("aa", "a*"));
            VERIFY(!isWildcardMatchDP("aa", "a*b"));
            VERIFY(isWildcardMatchDP("ab", "?*"));
            VERIFY(!isWildcardMatchDP("aab", "c*a*b"));
            VERIFY(isWildcardMatchDP("guilt", "g*t"));
            VERIFY(!isWildcardMatchDP("guilty", "g*t"));
            VERIFY(!isWildcardMatchDP("a", "b"));
            VERIFY(isWildcardMatchDP("ab", "ab*"));
            VERIFY(isWildcardMatchDP("guilt", "g*t"));
            VERIFY(isWildcardMatchDP("g", "g*"));
        }
    };

    /* Jump Game II */
    // LeetCode#45
    // Given an array of non-negative integers, you are initially positioned at the first index of 
    // the array. Each element in the array represents your maximum jump length at that position.
    // Your goal is to reach the last index in the minimum number of jumps. For example:
    // Given array A = [2,3,1,1,4],
    // The minimum number of jumps to reach the last index is 2. (Jump 1 step from index 0 to 1, then 
    // 3 steps to the last index.)
    // Note:
    //  You can assume that you can always reach the last index.
    struct p045
    {
        int getMinJumps(const vector<int>& v)
        {
            int e = 0; // edge
            int j = 0; // max jump
            int c = 0; // count
            for (int i = 0; i < v.size() - 1; ++i)
            {
                j = max(i + v[i], j);

                if (i == e)
                {
                    // if unreachable
                    if (j <= i)
                        return -1;

                    e = j;
                    ++c;
                }
            }

            return c;
        }

        void test()
        {
            VERIFY(0 == getMinJumps({ 0 }));
            VERIFY(0 == getMinJumps({ 1 }));
            VERIFY(2 == getMinJumps({ 2, 3, 1, 1, 4 }));
            VERIFY(1 == getMinJumps({ 4, 3, 1, 1, 4 }));
            VERIFY(1 == getMinJumps({ 2, 1 }));
            VERIFY(-1 == getMinJumps({ 1, 1, 0, 0 }));
        }
    };

    /* Permutations */
    // LeetCode#46
    // Given a collection of distinct numbers, return all possible permutations.
    struct p046
    {
        void getAllPermutationsHelper(vector<vector<int>>& r, vector<int>& c, int i)
        {
            if (i >= c.size() - 1)
            {
                r.push_back(c);
                return;
            }

            for (int j = i; j < c.size(); ++j)
            {
                swap(c[i], c[j]);
                getAllPermutationsHelper(r, c, i + 1);
                swap(c[i], c[j]);
            }
        }

        vector<vector<int>> getAllPermutations(const vector<int>& v)
        {
            vector<vector<int>> r;
            vector<int> c(v);
            getAllPermutationsHelper(r, c, 0);

            printVector(r);
            return r;
        }

        void test()
        {
            VERIFY(vector<vector<int>>({
                { 1, 2, 3 },
                { 1, 3, 2 },
                { 2, 1, 3 },
                { 2, 3, 1 },
                { 3, 2, 1 },
                { 3, 1, 2 },
            }) == getAllPermutations({ 1, 2, 3 }));
        }
    };

    /* Permutations II */
    // LeetCode#47
    // Given a collection of numbers that might contain duplicates, return all possible unique
    // permutations.
    struct p047
    {
        void getAllUniquePermutationsHelper(vector<vector<int>>& r, vector<int>& c, int i)
        {
            if (i >= c.size() - 1)
            {
                r.push_back(c);
                return;
            }

            unordered_set<int> m;
            for (int j = i; j < c.size(); ++j)
            {
                if (m.find(c[j]) != m.end())
                    continue;
                else
                    m.insert(c[j]);

                swap(c[i], c[j]);
                getAllUniquePermutationsHelper(r, c, i + 1);
                swap(c[i], c[j]);
            }
        }

        vector<vector<int>> getAllUniquePermutations(const vector<int>& v)
        {
            vector<vector<int>> r;
            vector<int> c(v);
            getAllUniquePermutationsHelper(r, c, 0);

            printVector(r);
            return r;
        }

        void test()
        {
            VERIFY(vector<vector<int>>({
                { 1, 1, 3 },
                { 1, 3, 1 },
                { 3, 1, 1 },
            }) == getAllUniquePermutations({ 1, 1, 3 }));
        }
    };

    /* Rotate Image */
    // LeetCode#48
    // You are given an n x n 2D matrix representing an image. Rotate the image by 90 degrees
    // (clockwise).
    struct p048
    {
        void rotateMatrix_0(vector<vector<char>>& m)
        {
            int nb = m.size() / 2;
            for (int b = 0; b < nb; ++b)
            {
                int i = b;
                int j = b;
                int n = m.size() - b * 2;
                for (int k = 0; k < n - 1; ++k)
                {
                    // save top-left
                    char tmp = m[i + k][j];
                    // swap with bottom-left
                    m[i + k][j] = m[n - 1 + i][j + k];
                    // swap with bottom-right
                    m[n - 1 + i][j + k] = m[n - 1 + i - k][n - 1 + j];
                    // swap with top-right
                    m[n - 1 + i - k][n - 1 + j] = m[i][n - 1 + j - k];
                    // final swap
                    m[i][n - 1 + j - k] = tmp;
                }
            }
        }

        void rotateMatrix(vector<vector<char>>& m)
        {
            // change the order of flip/tranpose to change rotation direction

            // flip vertical
            for (int i = 0; i < m.size() / 2; ++i)
                for (int j = 0; j < m[0].size(); ++j)
                    swap(m[i][j], m[m.size() - 1 - i][j]);
            //printVector(m);

            // tranpose (scan lower triangle below the diagonal)
            for (int i = 1; i < m.size(); ++i)
                for (int j = 0; j < i; ++j)
                    swap(m[i][j], m[j][i]);

            //printVector(m);
        }

        void test()
        {
            vector<vector<char>> m = {
                { '0', '1', '2', '3' },
                { '4', '5', '6', '7' },
                { '8', '9', 'a', 'b' },
                { 'c', 'd', 'e', 'f' },
            };

            printVector(m);
            rotateMatrix(m);
            printVector(m);

            VERIFY(m == vector<vector<char>>({
                { 'c', '8', '4', '0' },
                { 'd', '9', '5', '1' },
                { 'e', 'a', '6', '2' },
                { 'f', 'b', '7', '3' },
            }));

            m = {
                { 'x', 'x', ' ', ' ', 'x', 'x' },
                { 'x', 'x', ' ', ' ', 'x', 'x' },
                { ' ', ' ', ' ', ' ', 'x', 'x' },
                { 'x', 'x', 'x', 'x', 'x', 'x' },
                { 'x', 'x', 'x', 'x', 'x', 'x' },
                { ' ', ' ', ' ', ' ', ' ', ' ' },
            };

            printVector(m);
            rotateMatrix_0(m);
            printVector(m);

            VERIFY(m == vector<vector<char>>({
                { ' ', 'x', 'x', ' ', 'x', 'x' },
                { ' ', 'x', 'x', ' ', 'x', 'x' },
                { ' ', 'x', 'x', ' ', ' ', ' ' },
                { ' ', 'x', 'x', ' ', ' ', ' ' },
                { ' ', 'x', 'x', 'x', 'x', 'x' },
                { ' ', 'x', 'x', 'x', 'x', 'x' },
            }));
        }
    };

    static void run()
    {
        p026().test();
    }

    //REGISTER_RUNNABLE(lcd1)
}
