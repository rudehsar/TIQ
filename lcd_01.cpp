#include "stdafx.h"

// Get-Content .\LCD_01.cpp | findstr "struct p*" | ?{ $_ -match "struct p(\d+)" } | Measure-Object

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
            if (s.size() == 0)
                return 0;

            unordered_map<char, int> m;

            int mx = 0;
            int i = 0;
            m[s[i]] = i;
            for (int j = 1; j < s.size(); ++j)
            {
                if (m.find(s[j]) != m.end())
                {
                    mx = max(mx, j - i);
                    while (m.find(s[j]) != m.end())
                        m.erase(s[i++]);
                }

                m[s[j]] = j;
            }

            return mx;
        }

        void test()
        {
            VERIFY(0 == getMaxWithNoRepeats(""));
            VERIFY(3 == getMaxWithNoRepeats("abcabcbb"));
            VERIFY(1 == getMaxWithNoRepeats("bbb"));
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

            int m1 = min(k / 2, static_cast<int>(v1.size() - 1));
            int m2 = min(k / 2, static_cast<int>(v2.size() - 1));

            return (v1[m1] < v2[m2])
                ? findKth(vector<int>(v1.begin() + m1 + 1, v1.end()), v2, k - m1 - 1)
                : findKth(v1, vector<int>(v2.begin() + m2 + 1, v2.end()), k - m2 - 1);
        }

        double findMedian(const vector<int>& v1, const vector<int>& v2)
        {
            if ((v1.size() + v2.size()) == 0)
                return 0.0;

            int mid = (v1.size() + v2.size()) / 2;

            double m = findKth(v1, v2, mid);
            if (((v1.size() + v2.size()) % 2) == 0)
                m = (m + findKth(v1, v2, mid - 1)) / 2.0;

            return m;
        }

        void test()
        {
            VERIFY(2.0 == findMedian({ 1, 3 }, { 2 }));
            VERIFY(2.5 == findMedian({ 1, 2 }, { 3, 4 }));

            VERIFY(4.0 == findMedian({ 1, 2, 3, 4 }, { 5, 6, 7 }));
            VERIFY(4.5 == findMedian({ 1, 2, 3, 4 }, { 5, 6, 7, 8 }));

            VERIFY(1.0 == findMedian({ 1 }, {}));
            VERIFY(1.0 == findMedian({}, { 1 }));
            VERIFY(0.0 == findMedian({}, {}));
        }
    };

    /* Longest Palindromic Substring */
    // LeetCode#5
    // Given a string S, find the longest palindromic substring in S. You may assume that the maximum 
    // length of S is 1000, and there exists one unique longest palindromic substring.
    struct p005
    {
        string getLongestPalindrome(const string& s)
        {
            vector<vector<int>> dp(s.size(), vector<int>(s.size(), 0));

            // trivial cases
            for (int i = 0; i < dp.size(); ++i)
            {
                // single char is a palindrome of length 1
                dp[i][0] = 1;

                // twins are palindromes of length 2
                if (i > 0)
                    dp[i][1] = s[i] == s[i - 1] ? 2 : 0;
            }

            // start from 3rd char with offset 2 since we have already considered offset length
            // of 0 and 1 above.
            pair<int, int> mp;
            for (int i = 2; i < dp.size(); ++i)
                for (int j = 2; j <= i; ++j)
                {
                    if ((s[i] == s[i - j]) && (dp[i - 1][j - 2] > 0))
                        dp[i][j] = dp[i - 1][j - 2] + 2;

                    if (dp[i][j] > mp.second)
                        mp = { i - j, dp[i][j] };
                }

            printVector(dp);
            return mp.second > 0 ? s.substr(mp.first, mp.second) : "";
        }

        void test()
        {
            VERIFY("tacocat" == getLongestPalindrome("footacocatbar"));
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
            if (s.size() <= n)
                return s;

            vector<string> v(n);

            // current row index
            int vi = 0;

            // direction of row index (incr/decr)
            int vid = 1;

            for (int i = 0; i < s.size(); i++)
            {
                v[vi].push_back(s[i]);
                vi += vid;

                // if at either top or bottom row, change direction
                if (!(vi % (n - 1)))
                {
                    vid *= -1;
                }
            }

            string o;
            for (int i = 0; i < n; i++)
            {
                o += v[i];
            }

            return o;
        }

        void test()
        {
            VERIFY("PAHNAPLSIIGYIR" == convertToZigzag_0("PAYPALISHIRING", 3));
            VERIFY("PAHNAPLSIIGYIR" == convertToZigzag("PAYPALISHIRING", 3));
        }
    };

    /* String to Integer (atoi) */
    // LeetCode#8
    // Implement atoi()
    struct p008
    {
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

        int myAtoi(string str) {
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
                while ((s[i] == p[0]) || (p[0] == '.'))
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
            VERIFY(isRegexMatch("aab", "c*a*b"));
            VERIFY(!isRegexMatch("aa", "a"));
            VERIFY(!isRegexMatch("aaa", "aa"));
            VERIFY(isRegexMatch("aa", "a*"));
            VERIFY(isRegexMatch("aa", ".*"));
            VERIFY(isRegexMatch("ab", ".*"));
            VERIFY(isRegexMatch("aab", "c*a*b"));
            VERIFY(isRegexMatch("gggt", "g*t"));
            VERIFY(isRegexMatch("gggoat", ".*t"));
            VERIFY(isRegexMatch("gggoat", ".*t"));
            VERIFY(isRegexMatch("a", "ab*"));
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
    // given number, target.Return the sum of the three integers. You may assume that each input
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

    /* Merge K Sorted Lists */
    // LeetCode#21
    struct p021
    {
        vector<int> mergeKWay(const vector<vector<int>>& v)
        {
            // Priority queue item which stores individual min values from i'th row and j'th position.
            typedef pair<int, pair<int, int>> pq_item;
            auto comp = [](pq_item& p1, pq_item& p2)
            {
                return p1.first > p2.first;
            };

            // use custom greater comparer as priorirty_queue by default uses less<> to build a max
            // heap.
            priority_queue<pq_item, vector<pq_item>, decltype(comp)> pq(comp);
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
                {
                    s[wp++] = s[rp];
                    seenCount = 1;
                }
                else if (seenCount < maxAllowed)
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
                // 
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
                        if ((j - i - wlen) >(r.second - r.first + 1))
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

                if (findLeftmost)
                {
                    if ((v[mid] == target) && ((mid == 0) || v[mid - 1] < target))
                        return mid;

                    if (v[mid] < target)
                        lo = mid + 1;
                    else
                        hi = mid - 1;
                }
                else
                {
                    if ((v[mid] == target) && ((mid == v.size() - 1) || v[mid + 1] > target))
                        return mid;

                    if (v[mid] > target)
                        hi = mid - 1;
                    else
                        lo = mid + 1;
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
                if ((v[i] > 0) && (v[i] < v.size()) && (v[v[i] - 1] != v[i]))
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
        void rotateMatrix(vector<vector<char>>& m)
        {
            // change the order of flip/tranpose to change rotation direction

            // flip vertical
            for (int i = 0; i < m.size() / 2; ++i)
                for (int j = 0; j < m[0].size(); ++j)
                    swap(m[i][j], m[m.size() - 1 - i][j]);
            //printVector(m);

            // tranpose
            for (int i = 0; i < m.size(); ++i)
                for (int j = i; j < m[0].size(); ++j)
                    swap(m[i][j], m[j][i]);
            //printVector(m);
        }

        void test()
        {
            vector<vector<char>> m = {
                { 'a', 'a', 'b', 'b' },
                { 'a', 'a', 'b', 'b' },
                { 'x', 'x', 'y', 'y' },
                { 'x', 'x', 'y', 'y' },
            };

            printVector(m);
            rotateMatrix(m);
            printVector(m);

            VERIFY(m == vector<vector<char>>({
                { 'x', 'x', 'a', 'a' },
                { 'x', 'x', 'a', 'a' },
                { 'y', 'y', 'b', 'b' },
                { 'y', 'y', 'b', 'b' },
            }));
        }
    };

    /* Maximum Subarray */
    // LeetCode#52
    struct p052
    {
        /*
        pair<int, int> findMaxSubArray(const vector<int>& v)
        {
        int sum = v[0];
        int minsum = sum;
        int minsumi = -1;
        int maxdiff = 0;

        pair<int, int> r;
        for (int i = 1; i < v.size(); ++i)
        {
        sum += v[i];
        if (sum < minsum)
        {
        minsum = sum;
        minsumi = i;
        }

        if ((sum - minsum) > maxdiff)
        {
        maxdiff = sum - minsum;
        r.first = minsumi + 1;
        r.second = i;
        }
        }

        return r;
        }
        */

        template<typename TComp = less<int>>
        pair<int, int> findMaxSubArray(const vector<int>& v)
        {
            int j = 0;
            int sum = v[0];
            int max = sum;

            TComp comp;
            pair<int, int> r;
            for (int i = 1; i < v.size(); ++i)
            {
                if (comp(sum + v[i], v[i]))
                {
                    sum = v[i];
                    j = i;
                }
                else
                {
                    sum += v[i];
                }

                if (comp(max, sum))
                {
                    max = sum;
                    r = { j, i };
                }
            }

            return r;
        }

        pair<int, int> findCircularMaxSubArray(const vector<int>& v)
        {
            auto r1 = findMaxSubArray(v);
            auto r1sum = accumulate(v.cbegin() + r1.first, v.cbegin() + r1.second, 0);

            auto r2 = findMaxSubArray<greater<int>>(v);
            auto r2sum = accumulate(v.cbegin(), v.cend(), 0) -
                accumulate(v.cbegin() + r2.first, v.cbegin() + r2.second, 0);

            if (r1sum < r2sum)
                return{ (r2.second + 1) % v.size(), (r2.first - 1 + v.size()) % v.size() };

            return r1;
        }

        void test()
        {
            VERIFY(make_pair(0, 0) == findMaxSubArray({ 0 }));
            VERIFY(make_pair(0, 0) == findMaxSubArray({ 0, 0 }));
            VERIFY(make_pair(0, 0) == findMaxSubArray({ 1 }));
            VERIFY(make_pair(0, 1) == findMaxSubArray({ 1, 1 }));
            VERIFY(make_pair(0, 0) == findMaxSubArray({ -1 }));
            VERIFY(make_pair(3, 6) == findMaxSubArray({ -2, 1, -3, 4, -1, 2, 1, -5, 4 }));

            vector<int> v = { 904, 40, 523, 12, -335, -385, -124, 481, -31 };
            VERIFY(make_pair(0, 3) == findMaxSubArray(v));
            VERIFY(make_pair(0, 4) == findMaxSubArray({ 10, -4, 5, -4, 10 }));

            VERIFY(make_pair(7, 3) == findCircularMaxSubArray(v));
            VERIFY(make_pair(2, 0) == findCircularMaxSubArray({ 10, -4, 5, -4, 10 }));

            //vector<int> psv;
            //partial_sum(v.cbegin(), v.cend(), back_inserter(psv));
            //printVector(v);
            //printVector(psv);
        }
    };

    /* Maximum Subarray (Max contiguous sum) */
    // LeetCode#53
    // Find the contiguous subarray within an array (containing at least one number) which has the 
    // largest sum.
    // For example, given the array [-2,1,-3,4,-1,2,1,-5,4], the contiguous subarray [4,-1,2,1] has 
    // the largest sum = 6.
    // Programming Pearls (7.7.3)
    struct p053
    {
        pair<int, int> getMaxContiguousSum(const vector<int>& v)
        {
            int ms = INT_MIN;
            pair<int, int> mp = { -1, -1 };

            int s = 0;
            int i = 0;
            for (int j = 0; j < v.size(); ++j)
            {
                s += v[j];

                if (v[j] > s)
                {
                    i = j;
                    s = v[j];
                }

                if (s > ms)
                {
                    ms = s;
                    mp = { i, j };
                }
            }

            return mp;
        }

        void test()
        {
            vector<int> v = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
            VERIFY((make_pair<int, int>(2, 6) == getMaxContiguousSum(v)));

            VERIFY((make_pair<int, int>(2, 2) == getMaxContiguousSum({ -1, -2, 3 })));

            VERIFY((make_pair<int, int>(0, 0) == getMaxContiguousSum({ -1, -2, -3 })));
            VERIFY((make_pair<int, int>(1, 1) == getMaxContiguousSum({ -2, -1, -3 })));
        }
    };

    /* Spiral Matrix */
    // LeetCode#54
    // Given a matrix of m x n elements(m rows, n columns), return all elements of the matrix in
    // spiral order.
    struct p054
    {
        template<typename TCallback>
        void peelMatrixBorder(
            const vector<vector<int>>& m,
            int x,
            int y,
            int rows,
            int cols,
            TCallback callback)
        {
            int i = x;
            int j = y;

            // top
            while (j < (y + cols))
                callback(m[i][j++]);
            --j;

            // right
            ++i;
            while (i < (x + rows))
                callback(m[i++][j]);
            --i;

            // bottom
            --j;
            if (i > x)
            {
                while (j >= y)
                    callback(m[i][j--]);
                ++j;
            }

            // left
            --i;
            if (j < y + cols - 1)
            {
                while (i > x)
                    callback(m[i--][j]);
                ++i;
            }
        }

        vector<int> getMatrixSpiral(const vector<vector<int>>& m)
        {
            vector<int> r;

            int rows = m.size();
            int cols = m[0].size();
            int i = 0;
            int j = 0;
            while ((rows > 0) && (cols > 0))
            {
                peelMatrixBorder(m, i, j, rows, cols, [&r](auto& item) { r.push_back(item); });

                ++i;
                ++j;
                rows -= 2;
                cols -= 2;
            }

            return r;
        }

        void test()
        {
            VERIFY(vector<int>({ 0, 1, 2, 3 }) == getMatrixSpiral({
                { 0, 1, 2, 3 },
            }));

            VERIFY(vector<int>({ 0, 1, 2, 3 }) == getMatrixSpiral({
                { 0 },
                { 1 },
                { 2 },
                { 3 },
            }));

            VERIFY(vector<int>({ 0, 1, 2, 3 }) == getMatrixSpiral({
                { 0, 1 },
                { 3, 2 },
            }));

            VERIFY(vector<int>({ 0, 1, 2, 3, 4, 5 }) == getMatrixSpiral({
                { 0, 1, 2 },
                { 5, 4, 3 },
            }));

            VERIFY(vector<int>({ 0, 1, 2, 5, 8, 7, 6, 3, 4 }) == getMatrixSpiral({
                { 0, 1, 2 },
                { 3, 4, 5 },
                { 6, 7, 8 },
            }));

            VERIFY(vector<int>({ 0, 1, 2, 3, 7, 1, 0, 9, 8, 4, 5, 6 }) == getMatrixSpiral({
                { 0, 1, 2, 3 },
                { 4, 5, 6, 7 },
                { 8, 9, 0, 1 },
            }));

            VERIFY(vector<int>({ 0, 1, 2, 3, 7, 1, 5, 4, 3, 2, 8, 4, 5, 6, 0, 9 }) == getMatrixSpiral({
                { 0, 1, 2, 3 },
                { 4, 5, 6, 7 },
                { 8, 9, 0, 1 },
                { 2, 3, 4, 5 },
            }));
        }
    };

    /* Merge Intervals */
    // LeetCode#56
    // Given a collection of intervals, merge all overlapping intervals. For example,
    // Given
    //  [1, 3], [2, 6], [8, 10], [15, 18],
    // Return
    //  [1, 6], [8, 10], [15, 18]
    struct p056
    {
        using interval = pair<int, int>;

        vector<interval> mergeIntervals_0(const vector<interval>& v)
        {
            vector<interval> r;
            priority_queue<interval, vector<interval>, greater<interval>> pq(v.begin(), v.end());

            interval i = pq.top();
            pq.pop();
            while (!pq.empty())
            {
                interval j = pq.top();
                pq.pop();

                // is overlapping intervals?
                if (i.second >= j.first)
                {
                    j.first = min(i.first, j.first);
                    j.second = max(i.second, j.second);
                }
                else
                {
                    r.push_back(i);
                }

                i = j;
            }

            r.push_back(i);
            return r;
        }

        vector<interval> mergeIntervals(const vector<interval>& v)
        {
            if (v.empty())
                return v;

            vector<interval> r(v);
            sort(r.begin(), r.end());

            auto i = r.begin();
            while (i + 1 != r.end())
            {
                auto j = i + 1;
                if (i->second >= j->first)
                {
                    j->first = min(i->first, j->first);
                    j->second = max(i->second, j->second);

                    i = r.erase(i);
                }
                else
                {
                    ++i;
                }
            }

            return r;
        }

        void test()
        {
            VERIFY(vector<interval>({
                { 1, 6 },
                { 8, 10 },
                { 15, 18 },
            }) == mergeIntervals(vector<interval>({
                { 1, 3 },
                { 2, 6 },
                { 8, 10 },
                { 15, 18 },
            })));

            VERIFY(vector<interval>({
                { 1, 6 },
                { 8, 10 },
            }) == mergeIntervals(vector<interval>({
                { 1, 6 },
                { 8, 10 },
            })));

            VERIFY(vector<interval>({
                { 1, 6 },
            }) == mergeIntervals(vector<interval>({
                { 1, 6 },
            })));

            VERIFY(vector<interval>({
                { 1, 6 },
            }) == mergeIntervals(vector<interval>({
                { 1, 3 },
                { 2, 6 },
            })));

            VERIFY(vector<interval>({
            }) == mergeIntervals(vector<interval>({
            })));
        }
    };

    /* Insert Interval */
    // LeetCode#57
    // Given a set of non-overlapping and sorted intervals, insert a new interval into the
    // intervals (merge if necessary).
    // Example#1
    //  Given intervals [1, 3], [6, 9], insert and merge [2, 5] as [1, 5], [6, 9].
    // Example#2
    //  Given [1, 2], [3, 5], [6, 7], [8, 10], [12, 16], insert and merge [4, 9] as [1, 2], [3, 10], [12, 16].
    // This is because the new interval [4, 9] overlaps with[3, 5], [6, 7], [8, 10].
    struct p057
    {
        using interval = pair<int, int>;

        // optmized, uses binary search to find the closest predecessor and starts from there
        vector<interval> insertInterval_0(const vector<interval>& v, const interval& x)
        {
            if (v.empty())
                return{ x };

            // v is assumed sorted and contains only non-overlapping intervals

            // binary search to find the right-most interval with start earlier than the interval
            // to be inserted
            int lo = 0;
            int hi = v.size() - 1;
            while (lo <= hi)
            {
                int mid = lo + (hi - lo) / 2;
                if ((v[mid].first < x.first) && ((mid == v.size()) || (v[mid + 1].first >= x.first)))
                {
                    lo = mid;
                    break;
                }

                if (v[mid].first < x.first)
                    lo = mid + 1;
                else
                    hi = mid - 1;
            }

            // There could of one of 3 cases,
            // 1) interval to be inserted overlaps with the interval on the left
            //  => merge with r[lo] and start merging down from there to cover case#2
            // 2) interval to be inserted overlaps with the interval on the right
            //  => insert after r[lo] and increment lo to point to the newly inserted interval and 
            // start merging down.
            // 3) interval to be inserted doesn't overlap
            //  => insert after r[lo] and we're done.
            vector<interval> r(v);
            if (r[lo].second >= x.first)
                r[lo].second = max(r[lo].second, x.second);
            else
                r.insert(++lo + r.begin(), x);

            while (lo < r.size() - 1)
            {
                if (r[lo].second >= r[lo + 1].first)
                {
                    r[lo + 1].first = r[lo].first;
                    r[lo + 1].second = max(r[lo].second, r[lo + 1].second);

                    r.erase(r.begin() + lo);
                }
                else
                    ++lo;
            }

            return r;
        }

        // using BST
        vector<interval> insertInterval_1(const vector<interval>& v, const interval& x)
        {
            if (v.empty())
                return{ x };

            BinarySearchTree<interval> b;
            for (auto& i : v)
                b.insert(i);

            auto n = b.insert(x);
            auto p = b.inOrderPredecessor(n);
            if (p && (p->val.second >= n->val.first))
            {
                p->val.second = max(p->val.second, n->val.second);
                b.remove(n);

                n = p;
            }

            auto s = b.inOrderSuccessor(n);
            while (s && (n->val.second >= s->val.first))
            {
                n->val.second = max(n->val.second, s->val.second);
                b.remove(s);

                s = b.inOrderSuccessor(n);
            }

            vector<interval> r;
            b.traverse([&r](auto& p) { r.push_back(p->val); });

            return r;
        }

        // simplest, linear scan the array and insert/merge as appropriate
        vector<interval> insertInterval(const vector<interval>& v, const interval& x)
        {
            if (v.empty())
                return{ x };

            vector<interval> r;
            interval n = x;
            for (int i = 0; i < v.size(); ++i)
            {
                if (v[i].second < n.first) // strictly to the the left
                {
                    r.push_back(v[i]);
                }
                else if (v[i].first > n.second) // strictly to the right
                {
                    r.push_back(n);
                    n = v[i];
                }
                else // overlapping, merge
                {
                    n.first = min(v[i].first, n.first);
                    n.second = max(v[i].second, n.second);
                }
            }

            r.push_back(n);
            return r;
        }

        void test()
        {
            VERIFY(vector<interval>({
                { 1, 5 },
                { 6, 9 },
            }) == insertInterval(vector<interval>({
                { 1, 3 },
                { 6, 9 },
            }),
            { 2, 5 }));

            VERIFY(vector<interval>({
                { 1, 2 },
                { 3, 10 },
                { 12, 16 },
            }) == insertInterval(vector<interval>({
                { 1, 2 },
                { 3, 5 },
                { 6, 7 },
                { 8, 10 },
                { 12, 16 },
            }),
            { 4, 9 }));

            VERIFY(vector<interval>({
                { 1, 2 },
                { 3, 5 },
                { 6, 9 },
            }) == insertInterval(vector<interval>({
                { 1, 2 },
                { 6, 9 },
            }),
            { 3, 5 }));
        }
    };

    /* Permutation Sequence */
    // LeetCode#60
    // The set [1, 2, 3, ..., n] contains a total of n! unique permutations. By listing and labeling
    // all of the permutations in order, We get the following sequence(ie, for n = 3):
    //    1."123"
    //    2."132"
    //    3."213"
    //    4."231"
    //    5."312"
    //    6."321"
    // Given n and k, return the kth permutation sequence. Note: Given n will be between 1 and 9
    // inclusive.
    struct p060
    {
        string getPermutationSequence(int n, int k)
        {
            const string maxSequence = "123456789";

            // make k 0-based
            k -= 1;

            // get n - 1!
            int nf = 1;
            for (int i = 1; i < n; ++i)
                nf *= i;

            string s = maxSequence.substr(0, n);
            for (int i = 0; i < n - 1; ++i)
            {
                int p = k / nf;

                // move p'th element to the front
                s.insert(s.begin() + i, s[i + p]);
                s.erase(s.begin() + i + p + 1);

                // next segment size
                nf /= n - 1 - i;
                // remaining offset for the next segment
                k -= p * nf;
            }

            return s;
        }

        void test()
        {
            VERIFY("231" == getPermutationSequence(3, 4));
        }
    };

    /* Rotate List */
    // LeetCode#61
    // Given a list, rotate the list to the right by k places, where k is non-negative.
    // For example:
    // Given 1->2->3->4->5->NULL and k = 2,
    // return 4->5->1->2->3->NULL.
    struct p061
    {
        PListNode<int> rotateRight(PListNode<int> head, int k) {
            if (!head || (k == 0))
                return head;

            PListNode<int> t = head;
            int c = 0;
            while (t)
            {
                t = t->next;
                ++c;
            }

            k %= c;
            if (k == 0)
                return head;

            PListNode<int> p = head;
            t = p;
            while (k-- > 0)
                t = t->next;

            while (t->next)
            {
                p = p->next;
                t = t->next;
            }

            t->next = head;
            head = p->next;
            p->next = nullptr;

            return head;
        }

        void test()
        {
            VERIFY(convert_from(rotateRight(convert_to(vector<int>({ 0, 1, 2, 3, 4 })), 2)) ==
                vector<int>({ 3, 4, 0, 1, 2 }));
            VERIFY(convert_from(rotateRight(convert_to(vector<int>({ 0, 1, 2, 3, 4 })), 0)) ==
                vector<int>({ 0, 1, 2, 3, 4 }));
            VERIFY(convert_from(rotateRight(convert_to(vector<int>({ 0, 1, 2, 3, 4 })), 5)) ==
                vector<int>({ 0, 1, 2, 3, 4 }));
            VERIFY(convert_from(rotateRight(convert_to(vector<int>({ 0, 1, 2, 3, 4 })), 6)) ==
                vector<int>({ 4, 0, 1, 2, 3 }));
            VERIFY(convert_from(rotateRight(convert_to(vector<int>({})), 6)) ==
                vector<int>({}));
            VERIFY(convert_from(rotateRight(convert_to(vector<int>({ 0 })), 0)) ==
                vector<int>({ 0 }));
            VERIFY(convert_from(rotateRight(convert_to(vector<int>({ 0 })), 1)) ==
                vector<int>({ 0 }));
        }
    };

    /* Valid Number */
    // LeetCode#64
    // Validate if a given string is numeric.
    struct p065
    {
        bool isValidNumber(const string& s)
        {
            enum class Q
            {
                Invalid,
                Pre,
                Sign,
                Digits,
                DigitsDot,
                Dot,
                DotDigits,
                E,
                ESign,
                EDigits,
                Post,
            };

            enum class A
            {
                Blank,
                Sign,
                Digit,
                Dot,
                E,
            };

            const vector<vector<int>> t = {
                // b  s  d  .  e
                { 0, 0, 0, 0, 0 },
                { 1, 2, 3, 5, 0 }, //  1 Pre
                { 0, 0, 3, 5, 0 }, //  2 Sign
                { 10, 0, 3, 4, 7 }, //  3 Digits
                { 10, 0, 6, 0, 7 }, //  4 DigitsDot
                { 0, 0, 6, 0, 0 }, //  5 Dot
                { 10, 0, 6, 0, 7 }, //  6 DotDigits
                { 0, 8, 9, 0, 0 }, //  7 E
                { 0, 0, 9, 0, 0 }, //  8 ESign
                { 10, 0, 9, 0, 0 }, //  9 EDigits
                { 10, 0, 0, 0, 0 }, // 10 Post
            };

            const Q q0 = Q::Pre;
            const unordered_set<Q> f = { Q::Post };

            Q q = q0;
            for (auto& c : s)
            {
                A a;
                if (isblank(c))
                    a = A::Blank;
                else if ((c == '+') || (c == '-'))
                    a = A::Sign;
                else if (c == '.')
                    a = A::Dot;
                else if (isdigit(c))
                    a = A::Digit;
                else if (c == 'e')
                    a = A::E;
                else
                    return false;

                if ((q = (Q)t[(int)q][(int)a]) == Q::Invalid)
                    return false;
            }

            // pretend a blank insert at the end to account for end-of-string
            if ((q = (Q)t[(int)q][(int)A::Blank]) == Q::Invalid)
                return false;

            return f.find(q) != f.end();
        }

        void test()
        {
            VERIFY(isValidNumber("0"));
            VERIFY(isValidNumber(" 0.1 "));
            VERIFY(!isValidNumber("abc"));
            VERIFY(!isValidNumber("1 a"));
            VERIFY(isValidNumber("2e10"));
            VERIFY(isValidNumber(".1"));
            VERIFY(!isValidNumber(""));
            VERIFY(!isValidNumber(".1.0"));
            VERIFY(!isValidNumber("e0"));
            VERIFY(!isValidNumber("e0"));
            VERIFY(!isValidNumber("e0"));
            VERIFY(isValidNumber("1 "));
            VERIFY(isValidNumber("0."));
            VERIFY(!isValidNumber(" -."));
            VERIFY(isValidNumber("46.e3"));
        }
    };

    /* Text Justification */
    // LeetCode#68
    // Given an array of words and a length L, format the text such that each line has exactly L
    // characters and is fully(left and right) justified.You should pack your words in a greedy
    // approach; that is, pack as many words as you can in each line.Pad extra spaces ' ' when necessary
    // so that each line has exactly L characters.Extra spaces between words should be distributed as
    // evenly as possible.If the number of spaces on a line do not divide evenly between words, the
    // empty slots on the left will be assigned more spaces than the slots on the right.For the last
    // line of text, it should be left justified and no extra space is inserted between words.
    struct p068
    {
        class LineBuffer
        {
        public:
            LineBuffer(int maxWidth) :
                m_maxWidth(maxWidth),
                m_wordsLength(0)
            {
            }

            bool append(const string& w)
            {
                if (size() + 1 + w.size() > m_maxWidth)
                    return false;

                m_words.push_back(w);
                m_wordsLength += w.size();

                return true;
            }

            void clear()
            {
                m_words.clear();
                m_wordsLength = 0;
            }

            int size() const
            {
                // total size of the words + number of spaces required in between
                return m_wordsLength + m_words.size() - 1;
            }

            operator string() const
            {
                if (m_words.empty())
                    return "";

                vector<string> sp;
                if ((m_words.size() > 1) && (size() > (m_maxWidth / 2)))
                {
                    int minsp = (m_maxWidth - m_wordsLength) / (m_words.size() - 1);
                    int extsp = m_maxWidth - m_wordsLength - (minsp * (m_words.size() - 1));
                    for (int i = 0; i < m_words.size() - 1; ++i)
                    {
                        sp.push_back(string(minsp, ' '));
                        if (extsp-- > 0)
                            sp[i] += ' ';
                    }
                }

                string line;
                for (int i = 0; i < m_words.size(); ++i)
                {
                    line += m_words[i];
                    if (i < m_words.size() - 1)
                        line += sp[i];
                }

                return line;
            }

        private:
            int m_maxWidth;

            int m_wordsLength;
            vector<string> m_words;
        };

        vector<string> columnFit_0(const string& s, int columnWidth)
        {
            vector<string> r;

            auto iss = istringstream(s);
            LineBuffer lb(columnWidth);
            for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>(); ++it)
            {
                if (!lb.append(*it))
                {
                    r.push_back(lb);

                    lb.clear();
                    lb.append(*it);
                }
            }

            r.push_back(lb);
            return r;
        }

        vector<string> columnFit(const string& s, int columnWidth)
        {
            vector<string> r;
            auto appendLine = [&r](deque<string>& words, int len, int columnWidth)
            {
                if ((words.size() > 1) && ((len + words.size() - 1) > (columnWidth / 2)))
                {
                    // distribute extra whitespaces
                    int i = 0;
                    int nw = columnWidth - len;
                    while (nw-- > 0)
                    {
                        words[i++].push_back(' ');
                        i %= words.size() - (words.size() > 1 ? 1 : 0);
                    }
                }

                r.push_back("");
                while (!words.empty())
                {
                    r.back() += words.front();
                    words.pop_front();
                }
            };

            // split input into words
            deque<string> words = deque<string>(
                istream_iterator<string>(istringstream(s)),
                istream_iterator<string>());

            deque<string> lineWords;
            int len = 0;
            while (!words.empty())
            {
                if ((len + lineWords.size() + words.front().size()) > columnWidth)
                {
                    appendLine(lineWords, len, columnWidth);
                    lineWords.clear();
                    len = 0;
                }

                lineWords.push_back(words.front());
                words.pop_front();
                len += lineWords.back().size();
            }

            appendLine(lineWords, len, columnWidth);
            return r;
        }

        void test()
        {
            string s = "Given an array of words and a length L, format the text such  that " \
                "each line has exactly L characters and is fully (left and right) justified. You " \
                "should pack your words in a greedy approach; that is, pack as many words as you " \
                "can in each line. Pad extra spaces when necessary so that each line has exactly " \
                "L characters.";

            for (auto& l : columnFit(s, 80))
                LOG(l.size() << "=" << l);

            s = "This is an example of text justification.";
            VERIFY(columnFit(s, 16) == vector<string>({
                "This    is    an",
                "example  of text",
                "justification.",
            }));
        }
    };

    /* Square-root of a Number */
    // LeetCode#69
    // Implement int sqrt(int x). Compute and return the square root of x.
    //
    // Find the squareroot of a given number rounded down to the nearest integer, without using the
    // sqrt function. For example, squareroot of a number between[9, 15] should return 3, and[16, 24]
    // should be 4.
    struct p069
    {
        int getSqrtFloor_0(int n)
        {
            int i = 0;
            int j = 1 + n / 2;

            while (i + 1 < j)
            {
                int mid = i + (j - i) / 2;
                int mid2 = mid * mid;
                if ((mid2) == n)
                    return mid;
                else
                    if (mid2 < n)
                        i = mid;
                    else
                        j = mid;
            }

            return i;
        }

        int getSqrtFloor_1(int n)
        {
            int i = 1;
            int j = n / 2;

            int m = i;
            int md = INT_MAX;

            while (i <= j)
            {
                int mid = i + (j - i) / 2;
                int mid2 = mid * mid;
                if ((mid2) == n)
                    return mid;
                else
                    if (mid2 < n)
                        i = mid + 1;
                    else
                        j = mid - 1;

                int d = n - mid2;
                if ((d > 0) && (d < md))
                {
                    md = d;
                    m = mid;
                }
            }

            return m;
        }

        int getSqrtFloor(int n)
        {
            if (n <= 1)
                return n;

            int i = 1;
            int j = n / 2;

            while (i <= j)
            {
                int mid = i + (j - i) / 2;
                int mid2 = mid * mid;
                if ((mid2) == n)
                    return mid;
                else
                    if (mid2 < n)
                        i = mid + 1;
                    else
                        j = mid - 1;
            }

            return i - 1;
        }

        void test()
        {
            VERIFY(1 == getSqrtFloor(1));
            VERIFY(3 == getSqrtFloor(9));
            VERIFY(3 == getSqrtFloor(12));
            VERIFY(3 == getSqrtFloor(15));
            VERIFY(4 == getSqrtFloor(16));
            VERIFY(4 == getSqrtFloor(18));
            VERIFY(4 == getSqrtFloor(24));
        }
    };

    /* Simplify Path */
    // LeetCode#71
    // Given an absolute path for a file (Unix-style), simplify it.
    struct p071
    {
        string getSimplifiedPath(const string& s)
        {
            stack<string> st;

            int i = 0;
            while (i < s.size())
            {
                int j = s.find("/", i);

                string p;
                if (j == string::npos)
                    p = s.substr(i);
                else
                    p = s.substr(i, j - i);

                i += p.size() + 1;

                if ((p == "..") && !st.empty())
                    st.pop();
                else if ((p != "") && (p != ".") && (p != ".."))
                    st.push(p);
            }

            string r;
            while (!st.empty())
            {
                if (r.empty())
                    r = st.top();
                else
                    r = st.top() + "/" + r;

                st.pop();
            }

            if (r.empty() || (s[0] == '/'))
                r = "/" + r;

            return r;
        }

        void test()
        {
            VERIFY("/home" == getSimplifiedPath("/home/"));
            VERIFY("/c" == getSimplifiedPath("/a/./b/../../c/"));
            VERIFY("/" == getSimplifiedPath("/../"));
            VERIFY("/home/foo" == getSimplifiedPath("/home//foo/"));
            VERIFY("home/foo" == getSimplifiedPath("home//foo/"));
        }
    };

    /* Edit Distance */
    // LeetCode#72
    // Min edit distance (aka Levenshtein distance) given two strings
    struct p072
    {
        /* DP */
        int getMinEditDistance_0(const string& s1, const string& s2)
        {
            vector<vector<int>> dp(s1.size() + 1, vector<int>(s2.size() + 1));

            // distance with an empty string is same as the string length
            for (int i = 1; i < dp.size(); ++i)
                dp[i][0] = i;

            for (int j = 1; j < dp[0].size(); ++j)
                dp[0][j] = j;

            for (int i = 1; i < dp.size(); ++i)
                for (int j = 1; j < dp[0].size(); ++j)
                {
                    if (s1[i - 1] == s2[j - 1])
                        dp[i][j] = dp[i - 1][j - 1];
                    else
                        dp[i][j] = min(min(dp[i][j - 1], dp[i - 1][j]), dp[i - 1][j - 1]) + 1;
                }

            //printVector(dp);
            return dp.back().back();
        }

        /* Recursive */
        int getMinEditDistance(const string& s1, int i, const string& s2, int j)
        {
            if ((i == s1.size()) || (j == s2.size()))
                return max(s1.size() - i, s2.size() - j);

            if (s1[i] == s2[j])
                return getMinEditDistance(s1, i + 1, s2, j + 1);
            else
                return min(
                    min(
                        getMinEditDistance(s1, i, s2, j + 1),  // remove
                        getMinEditDistance(s1, i + 1, s2, j)), // insert
                    getMinEditDistance(s1, i + 1, s2, j + 1))  // replace
                + 1;
        }

        int getMinEditDistance(const string& s1, const string& s2)
        {
            return getMinEditDistance(s1, 0, s2, 0);
        }

        void test()
        {
            VERIFY(3 == getMinEditDistance("foo", ""));
            VERIFY(3 == getMinEditDistance("foo", "bar"));
            VERIFY(3 == getMinEditDistance("good", "bad"));
            VERIFY(1 == getMinEditDistance("ab", "ac"));
            VERIFY(2 == getMinEditDistance("gambol", "gumbo"));
        }
    };

    /* Minimum Window Substring */
    // LeetCode#76
    // Given a string S and a string T, find the minimum window in S which will contain all the
    // characters in T in complexity O(n). For example, S = "ADOBECODEBANC", T = "ABC", minimum
    // window is "BANC".
    struct p076
    {
        string minWindowSubstring(const string& s, const string& t)
        {
            auto isSuperSet = [](auto& m1, auto& m2)
            {
                if (m1.size() < m2.size())
                    return false;

                for (auto& p : m1)
                    if ((m2.find(p.first) == m2.end()) || (m2[p.first] > p.second))
                        return false;

                return true;
            };

            unordered_map<char, int> mt;
            for (int i = 0; i < t.size(); ++i)
                mt[t[i]] += 1;

            int minlen = INT_MAX;
            int minstart = -1;
            int start = -1;

            unordered_map<char, int> ms;
            for (int i = 0; i < s.size(); ++i)
            {
                // ignore chars that doesn't exist in t
                if (mt.find(s[i]) == mt.end())
                    continue;

                // unless already done, mark start of "window"
                if (start < 0)
                    start = i;

                ms[s[i]] += 1;

                while (isSuperSet(ms, mt))
                {
                    // record minlen if applicable
                    if (minlen >(i - start))
                    {
                        minlen = i - start + 1;
                        minstart = start;
                    }

                    // trim "window" by incrementing start
                    ms[s[start]] -= 1;
                    start += 1;

                    // skip past chars that doesn't exist in t
                    while ((start < s.size()) && (mt.find(s[start]) == mt.end()))
                        ++start;
                }
            }

            return minstart < 0 ? "" : s.substr(minstart, minlen);
        }

        void test()
        {
            VERIFY("BANC" == minWindowSubstring("ADOBECODEBANC", "ABC"));
            VERIFY("AFBCA" == minWindowSubstring("ADEAFBCAGCAH", "AABC"));
        }
    };

    /* String to Integer */
    // LeetCode#8
    // Implement atoi to convert a string to an integer.
    // Hint: Carefully consider all possible input cases.If you want a challenge, please do not see
    // below and ask yourself what are the possible input cases.
    struct p080
    {
        int custom_atoi(const string& s)
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

        void test()
        {
            VERIFY(INT_MAX == custom_atoi("2147483647"));
            VERIFY(INT_MIN == custom_atoi("-2147483648"));
            VERIFY(0 == custom_atoi("0"));
            VERIFY(-1 == custom_atoi("-1"));
            VERIFY(1 == custom_atoi("1"));
            VERIFY(20842 == custom_atoi("20842"));
            VERIFY(5 == custom_atoi("05"));
            VERIFY(-519 == custom_atoi("-519"));

            for (auto& s : { "209482029382502", "2147483649", "-2147483649" })
            {
                bool caught_overflow_error = false;
                try
                {
                    custom_atoi(s);
                }
                catch (overflow_error&)
                {
                    caught_overflow_error = true;
                };

                VERIFY(caught_overflow_error);
            }
        }
    };

    /* Partition List */
    // LeetCode#86
    // Given a linked list and a value x, partition it such that all nodes less than x come before 
    // nodes greater than or equal to x. You should preserve the original relative order of the nodes 
    // in each of the two partitions. For example,
    // Given
    //  1->4->3->2->5->2 and x = 3,
    // Return
    //  1->2->2->4->3->5.
    struct p086
    {
        /*
        PListNode<int> partitionList(PListNode<int> head, int pivot)
        {
            PListNode<int> ppn;
            PListNode<int> pn = head;
            while (pn && (pn->data != pivot))
            {
                ppn = pn;
                pn = pn->next;
            }

            // move all smaller nodes before pivot
            PListNode<int> p = pn;
            while (p->next)
            {
                if (p->next->data < pivot)
                {
                    // get a handle to target node
                    PListNode<int> t = p->next;

                    // remove after p
                    p->next = t->next;

                    // insert after ppn
                    if (ppn)
                    {
                        t->next = ppn->next;
                        ppn->next = t;
                    }
                    else
                    {
                        t->next = head;
                        head = t;
                    }

                    // update previous node pointer
                    ppn = t;
                }
                else
                {
                    ppn = p;
                    p = ppn->next;
                }
            }

            // move all larger nodes after pivot
            ppn.reset();
            p = head;
            PListNode<int> tn = pn;
            while (p != pn)
            {
                if (p->data > pivot)
                {
                    // remove after ppn
                    if (ppn)
                        ppn->next = p->next;
                    else
                        head = head->next;

                    // insert after tail node
                    p->next = tn->next;
                    tn->next = p;
                    tn = p;

                    // update current node pointer
                    if (ppn)
                        p = ppn->next;
                    else
                        p = head;
                }
                else
                {
                    ppn = p;
                    p = ppn->next;
                }
            }

            return head;
        }
        */

        void insertAfter(PListNode<int>& head, PListNode<int> p, PListNode<int> n)
        {
            assert(head);
            assert(n);

            if (p)
            {
                n->next = p->next;
                p->next = n;
            }
            else
            {
                n->next = head;
                head = n;
            }
        }

        PListNode<int> removeNext(PListNode<int>& head, PListNode<int> p)
        {
            assert(head);

            PListNode<int> n;
            if (p)
            {
                n = p->next;
                if (n)
                    p->next = n->next;
            }
            else
            {
                n = head;
                if (n)
                    head = n->next;
            }

            return n;
        }

        PListNode<int> partitionList(PListNode<int>& head, int pivot)
        {
            PListNode<int> ppn;
            PListNode<int> pn = head;
            while (pn && (pn->data != pivot))
            {
                ppn = pn;
                pn = pn->next;
            }

            if (!pn)
                return head;

            // move all the smaller nodes after pivot and insert immediately before pivot node
            PListNode<int> p = pn;
            while (p->next)
            {
                if (p->next->data < pivot)
                {
                    PListNode<int> n = removeNext(head, p);
                    insertAfter(head, ppn, n);

                    ppn = n;
                }
                else
                {
                    ppn = p;
                    p = p->next;
                }
            }

            // move all the larger nodes before pivot and insert after the pivot node (in order)
            ppn.reset();
            p = head;
            PListNode<int> npn = pn;
            while (p != pn)
            {
                if (p->data > pivot)
                {
                    p = p->next;

                    PListNode<int> n = removeNext(head, ppn);
                    insertAfter(head, npn, n);

                    npn = n;
                }
                else
                {
                    ppn = p;
                    p = p->next;
                }
            }

            return head;
        }

        void test()
        {
            VERIFY(vector<int>({ 1, 2, 3, 4, 5 }) == convert_from(
                partitionList(convert_to(vector<int>{ 1, 4, 3, 2, 5 }), 3)));
            VERIFY(vector<int>({ 1, 4, 3, 2, 5 }) == convert_from(
                partitionList(convert_to(vector<int>{ 1, 4, 3, 2, 5 }), 1)));
            VERIFY(vector<int>({ 1, 4, 3, 2, 5 }) == convert_from(
                partitionList(convert_to(vector<int>{ 1, 4, 3, 2, 5 }), 5)));
            VERIFY(vector<int>({ 1, 3, 4, 2, 5 }) == convert_from(
                partitionList(convert_to(vector<int>{ 3, 4, 1, 2, 5 }), 1)));
            VERIFY(vector<int>({ 1, 4, 5, 2, 3 }) == convert_from(
                partitionList(convert_to(vector<int>{ 4, 5, 1, 2, 3 }), 1)));
        }
    };

    /* Merge Sorted Arrays */
    // LeetCode#88
    // Given two sorted integer arrays A and B, merge B into A as one sorted array.
    // Note: You may assume that A has enough space to hold additional elements from B. The number
    // of elements initialized in A and B are m and n respectively.
    struct p088
    {
        vector<int> mergeSortedArrays(const vector<int>& v1, const vector<int>& v2)
        {
            vector<int> m;

            int i = 0;
            int j = 0;
            while ((i < v1.size()) && (j < v2.size()))
            {
                if (v1[i] < v2[j])
                    m.push_back(v1[i++]);
                else
                    m.push_back(v2[j++]);
            }

            while (i < v1.size())
                m.push_back(v1[i++]);

            while (j < v2.size())
                m.push_back(v2[j++]);

            return m;
        }

        void test()
        {
            VERIFY((vector<int>{ 0 } == mergeSortedArrays({ 0 }, {})));
            VERIFY((vector<int>{ 0 } == mergeSortedArrays({}, { 0 })));
            VERIFY((vector<int>{ 0, 0 } == mergeSortedArrays({ 0 }, { 0 })));
            VERIFY((vector<int>{ 1, 2, 3 } == mergeSortedArrays({ 1 }, { 2, 3 })));
            VERIFY((vector<int>{ 1, 2, 3 } == mergeSortedArrays({ 1, 2 }, { 3 })));
        }
    };

    /* Maximal Rectangle */
    // LeetCode#85
    // Given N non-negative integers representing the histogram's bar height where the width of each
    // bar is 1, find the area of largest rectangle in the histogram.
    struct p085
    {
        int getMaxRectangleArea(const vector<int>& v)
        {
            stack<int> st;
            int maxrect = 0;
            for (int i = 0; i < v.size(); ++i)
            {
                if (st.empty() || (v[i] >= st.top()))
                {
                    st.push(v[i]);
                    continue;
                }

                int w = 1;
                while (!st.empty() && (v[i] < st.top()))
                {
                    maxrect = max(maxrect, st.top() * w);
                    st.pop();
                    ++w;
                }
            }

            int w = 1;
            while (!st.empty())
            {
                maxrect = max(maxrect, st.top() * w);
                st.pop();
                ++w;
            }

            return maxrect;
        }

        void test()
        {
            VERIFY(10 == getMaxRectangleArea({ 2, 1, 5, 6, 2, 3 }));
        }
    };

    /* Scramble String */
    // LeetCode#87
    // Given a string s1, we may represent it as a binary tree by partitioning it to two non-empty
    // substrings recursively.
    struct p087
    {
        bool isScramble_0(string s1, string s2)
        {
            if (s1.size() != s2.size())
                return false;

            if (s1.size() == 1)
                return s1[0] == s2[0];

            unordered_set<char> m1;
            unordered_set<char> m2;
            for (int i = 0; i < s1.size(); ++i)
                m1.insert(s1[i]), m2.insert(s2[i]);

            if (m1 != m2)
                return false;

            for (int i = 1; i < s2.size(); ++i)
            {
                // cut at length i from left for both strings
                if (isScramble_0(s1.substr(0, i), s2.substr(0, i)) &&
                    isScramble_0(s1.substr(i), s2.substr(i)))
                    return true;

                // cut at length i from left on s1 and from right on s2 
                if (isScramble_0(s1.substr(0, i), s2.substr(s2.size() - i, i)) &&
                    isScramble_0(s1.substr(i), s2.substr(0, s2.size() - i)))
                    return true;
            }

            return false;
        }

        bool isScrambleHelper(string s1, string s2, bool isReversed)
        {
            if (s1.size() != s2.size())
                return false;

            if (s1.size() == 1)
                return s1[0] == s2[0];

            vector<int> p;
            unordered_multiset<char> m1;
            unordered_multiset<char> m2;
            for (int i = 0; i < s1.size(); ++i)
            {
                m1.insert(s1[i]);
                m2.insert(s2[i]);

                if (m1 == m2)
                    p.push_back(i);
            }

            if (m1 != m2)
                return false;

            p.pop_back();
            for (int i : p)
            {
                if (isScrambleHelper(s1.substr(0, i + 1), s2.substr(0, i + 1), false) &&
                    isScrambleHelper(s1.substr(i + 1), s2.substr(i + 1), false))
                    return true;
            }

            if (!isReversed)
            {
                reverse(s2.begin(), s2.end());
                return isScrambleHelper(s1, s2, true);
            }

            return false;
        }

        bool isScramble(string s1, string s2)
        {
            return isScrambleHelper(s1, s2, false);
        }

        void test()
        {
            VERIFY(isScramble("great", "rgtae"));
            VERIFY(isScramble("abc", "bca"));
            VERIFY(isScramble("abc", "abc"));
            VERIFY(!isScramble("abcd", "bdac"));
        }
    };

    /* Decode Ways */
    // LeetCode#91
    struct p091
    {
        int getNumDecodingWays(const string& s)
        {
            if (s.size() < 2)
                return s.size() > 0 ? s[0] != '0' : 0;

            vector<int> dp(s.size());

            // special case#1: 1st char
            dp[dp.size() - 1] = s[s.size() - 1] != '0';

            // special case#2: 2nd char
            dp[dp.size() - 2] = s[s.size() - 2] != '0';
            if (s[s.size() - 2] != '0')
            {
                dp[dp.size() - 2] = 1;
                dp[dp.size() - 2] += atoi(s.substr(s.size() - 2, 2).c_str()) <= 26;
            }

            for (int i = s.size() - 3; i >= 0; --i)
            {
                dp[i] = 0;
                if (s[i] == '0')
                    continue;

                dp[i] += dp[i + 1];

                if (atoi(s.substr(i, 2).c_str()) <= 26)
                    dp[i] += dp[i + 2];
            }

            //printVector(dp);
            return dp[0];
        }

        void test()
        {
            VERIFY(1 == getNumDecodingWays("1"));
            VERIFY(1 == getNumDecodingWays("101"));
            VERIFY(0 == getNumDecodingWays("23051324734"));
            VERIFY(8 == getNumDecodingWays("23651324734"));
        }
    };

    /* Interleaving String */
    // LeetCode#97
    // Given s1, s2, s3, find whether s3 is formed by the interleaving of s1 and s2.
    // For example,
    // Given:
    //  s1 = "aabcc",
    //  s2 = "dbbca",
    // When s3 = "aadbbcbcac", return true.
    // When s3 = "aadbbbaccc", return false.
    struct p097
    {
        struct tuple_hasher
        {
            size_t operator()(const tuple<int, int, int>& t) const
            {
                size_t h = 7;
                h ^= hash<int>()(get<0>(t));
                h *= 11;

                h ^= hash<int>()(get<1>(t));
                h *= 11;

                h ^= hash<int>()(get<2>(t));
                h *= 11;

                return h;

                //return
                //    hash<int>()(get<0>(t)) ^
                //    hash<int>()(get<1>(t)) ^
                //    hash<int>()(get<2>(t));
            }
        };

        bool isInterleaveHelper(
            const string& s1,
            const string& s2,
            const string& s3,
            unordered_set<tuple<int, int, int>, tuple_hasher>& m)
        {
            if (s1.empty() && s2.empty() && s3.empty())
                return true;

            if ((s1.size() + s2.size()) != s3.size())
                return false;

            if (m.find(make_tuple<int, int, int>(s1.size(), s2.size(), s3.size())) != m.end())
            {
                LOG("<<{" << s1.size() << "," << s2.size() << "," << s3.size() << "}");
                return false;
            }

            LOG(s1 << "|" << s2 << "|" << s3);

            if (s1[0] == s3[0])
            {
                if (isInterleaveHelper(s1.substr(1), s2, s3.substr(1), m))
                    return true;

                LOG(">>{" << (s1.size() - 1) << "," << s2.size() << "," << (s3.size() - 1) << "}");
                m.insert(make_tuple<int, int, int>(s1.size() - 1, s2.size(), s3.size() - 1));
            }

            if (s2[0] == s3[0])
            {
                if (isInterleaveHelper(s1, s2.substr(1), s3.substr(1), m))
                    return true;

                LOG(">>{" << s1.size() << "," << (s2.size() - 1) << "," << (s3.size() - 1) << "}");
                m.insert(make_tuple<int, int, int>(s1.size(), s2.size() - 1, s3.size() - 1));
            }

            return false;
        }

        bool isInterleave_0(const string& s1, const string& s2, const string& s3)
        {
            if (s1.empty() && s2.empty() && s3.empty())
                return true;

            if ((s1.size() + s2.size()) != s3.size())
                return false;

            //LOG(s1 << "|" << s2 << "|" << s3);

            return
                ((s1[0] == s3[0]) && isInterleave_0(s1.substr(1), s2, s3.substr(1))) ||
                ((s2[0] == s3[0]) && isInterleave_0(s1, s2.substr(1), s3.substr(1)));
        }

        bool isInterleave(const string& s1, const string& s2, const string& s3)
        {
            unordered_set<tuple<int, int, int>, tuple_hasher> m;
            return isInterleaveHelper(s1, s2, s3, m);
        }

        void test()
        {
            //LOG(hash<int>()(1));
            //LOG((hash<int>()(1) ^ hash<int>()(1)));
            //LOG(hash<int>()(2));
            LOG(tuple_hasher()(make_tuple(1, 1, 0)));
            LOG(tuple_hasher()(make_tuple(2, 2, 0)));

            //VERIFY(isInterleave("aabcc", "dbbca", "aadbbcbcac"));
            //VERIFY(!isInterleave("aabcc", "dbbca", "aadbbbaccc"));
        }
    };

    /* Binary Tree Level Order Traversal */
    // LeetCode#102
    // Given a binary tree, return the level order traversal of its nodes' values. (ie, from left to 
    // right, level by level). For example:
    // Given binary tree [3,9,20,null,null,15,7],
    // Return its level order traversal as:
    // [
    //   [3],
    //   [9,20],
    //   [15,7]
    // ]
    struct p102
    {
        vector<vector<int>> getLevelOrderPrint_0(PTreeNode<int>& root)
        {
            if (!root)
                return{};

            queue<PTreeNode<int>> q;
            q.push(root);

            int c = 1;
            int n = 0;

            vector<vector<int>> r;
            r.push_back(vector<int>());
            while (!q.empty())
            {
                r.back().push_back(q.front()->val);

                if (q.front()->left)
                {
                    q.push(q.front()->left);
                    ++n;
                }

                if (q.front()->right)
                {
                    q.push(q.front()->right);
                    ++n;
                }

                q.pop();
                --c;

                if (c == 0)
                {
                    if (!q.empty())
                        r.push_back(vector<int>());
                    swap(c, n);
                }
            }

            return r;
        }

        vector<vector<int>> getLevelOrderPrint(PTreeNode<int>& root)
        {
            if (!root)
                return{};

            queue<PTreeNode<int>> q;
            q.push(root);

            vector<vector<int>> r;
            while (!q.empty())
            {
                int n = q.size();

                r.push_back({});
                while (n-- > 0)
                {
                    r.back().push_back(q.front()->val);

                    if (q.front()->left)
                        q.push(q.front()->left);

                    if (q.front()->right)
                        q.push(q.front()->right);

                    q.pop();
                }
            }

            return r;
        }

        void test()
        {
            BinarySearchTree<int> b = getBinaryTree(BinarySearchTree<int>(), { 1, 2, 3, 4, 5 }, true);
            printBinaryTree(b);

            VERIFY(vector<vector<int>>({ { 3 },{ 1, 4 },{ 2, 5 } }) == getLevelOrderPrint(b.getRoot()));
        }
    };

    /* Binary Tree Maximum Path Sum */
    // LeetCode#124
    // Given a binary tree, find the maximum path sum.
    // For this problem, a path is defined as any sequence of nodes from some starting node to any
    // node in the tree along the parent-child connections. The path does not need to go through
    // the root.
    struct p124
    {
        pair<int, int> getMaximumPathSumHelper(PTreeNode<int>& p)
        {
            if (!p->left && !p->right)
                return{ p->val, p->val };

            if (!p->left || !p->right)
            {
                auto r = getMaximumPathSumHelper(p->left ? p->left : p->right);
                return{ r.first + p->val, r.second };
            }

            auto r1 = getMaximumPathSumHelper(p->left);
            auto r2 = getMaximumPathSumHelper(p->right);

            return{
                max(r1.first, r2.first) + p->val,
                max(r1.second, r2.second)
            };
        }

        int getMaximumPathSum(BinaryTree<int>& b)
        {
            auto r = getMaximumPathSumHelper(b.getRoot());
            return max(r.first, r.second);
        }

        void test()
        {
            auto b = getRandom(BinarySearchTree<int>(), getRandom(0, 10, 10));
            LOG(b.to_string(b.getRoot()));
            LOG(getMaximumPathSum(b));
        }
    };

    /* Valid Palindrome */
    // LeetCode#125
    // Given a string, determine if it is a palindrome, considering only alphanumeric characters and
    // ignoring cases. For example, "Red rum, sir, is murder" is a palindrome, while
    // "Programcreek is awesome" is not.
    // Note:
    // Have you consider that the string might be empty ? This is a good question to ask during an
    // interview. For the purpose of this problem, we define empty string as valid palindrome.
    struct p125
    {
        bool isPalindrome(const string& s)
        {
            int i = 0;
            int j = s.size() - 1;

            auto isValid = [](const char& c)
            {
                return (c >= 'a') && (c <= 'z');
            };

            while (i < j)
            {
                int ci = tolower(s[i]);
                int cj = tolower(s[j]);

                if (!isValid(ci))
                {
                    ++i;
                    continue;
                }

                if (!isValid(cj))
                {
                    --j;
                    continue;
                }

                if (ci != cj)
                    break;

                ++i;
                j--;
            }

            return i >= j;
        }

        void test()
        {
            VERIFY(isPalindrome("Red rum, sir, is murder"));
            VERIFY(isPalindrome("a man, a plan, a canal, panama"));
            VERIFY(!isPalindrome("not a palindrome"));
        }
    };

    /* Copy List with Random Pointer */
    // LeetCode#138
    // A linked list is given such that each node contains an additional random pointer which could 
    // point to any node in the list or null. Return a deep copy of the list.
    struct p138
    {
        template<typename T>
        struct ListNode2;

        template<typename T>
        using PListNode2 = shared_ptr<ListNode2<T>>;

        template<typename T>
        struct ListNode2
        {
            T data;
            PListNode2<T> next;
            PListNode2<T> link;
        };

        PListNode2<int> copyList(PListNode2<int> head)
        {
            // clone list with zig-zag links
            PListNode2<int> h = head;
            while (h)
            {
                PListNode2<int> n = make_shared<ListNode2<int>>();
                n->data = h->data;
                n->next = h->next;

                h->next = n;
                h = n->next;
            }

            PListNode2<int> nh = head->next;

            // copy link pointers
            h = head;
            while (h)
            {
                if (h->link)
                    h->next->link = h->link->next;

                h = h->next->next;
            }

            // update next pointers and fix original links
            h = head;
            while (h)
            {
                PListNode2<int> n = h->next->next;

                if (n)
                    h->next->next = n->next;

                h->next = n;
                h = n;
            }

            return nh;
        }

        void test()
        {
            vector<PListNode2<int>> pv(getRandom(0, 10));
            for (int i = 0; i < pv.size(); ++i)
                pv[i] = PListNode2<int>(new ListNode2<int>{ i, nullptr, nullptr });

            for (int i = 0; i < pv.size() - 1; ++i)
            {
                pv[i]->next = pv[i + 1];
                if (getRandom(0, 3) > 0)
                    pv[i]->link = pv[getRandom(0, pv.size() - 1)];
            }

            PListNode2<int> h = pv[0];
            auto p = copyList(h);

            bool verified = true;
            while (h)
            {
                verified = verified && (h->data == p->data);
                verified = verified && (!h->next || (h->next->data == p->next->data));
                verified = verified && (!h->link || (h->link->data == p->link->data));
                LOG(verified);

                LOG(h->data << "@" << h << ",n:" << h->next << ",l:" << h->link);
                LOG(p->data << "@" << p << ",n:" << p->next << ",l:" << p->link);

                h = h->next;
                p = p->next;
            }

            VERIFY(verified);
        }
    };

    /* Word Break */
    // LeetCode#139
    struct p139
    {
        bool isWordBreakableHelper(vector<string>& r, int i, const string& s, const unordered_set<string>& d)
        {
            LOG(i << ": " << s.substr(i));

            if (i >= s.size())
                return true;

            for (int j = i + 1; j < s.size(); ++j)
            {
                string p = s.substr(i, j - i + 1);
                if (d.find(p) == d.end())
                    continue;

                r.push_back(p);
                if (isWordBreakableHelper(r, j + 1, s, d))
                    return true;

                r.pop_back();
            }

            return false;
        }

        /* Recursion */
        bool isWordBreakable_0(const string& s, const unordered_set<string>& d)
        {
            vector<string> r;
            if (isWordBreakableHelper(r, 0, s, d))
            {
                printVector(r);
                return true;
            }

            return false;
        }

        /* DP */
        bool isWordBreakable(const string& s, const unordered_set<string>& d)
        {
            // b[i] contains the length of the dictionary word ending at index i
            vector<int> b(s.size());
            for (int i = 0; i < s.size(); ++i)
            {
                // check if the substring ending at i is a dictionary word
                if (d.find(s.substr(0, i + 1)) != d.end())
                {
                    b[i] = i + 1;
                    continue;
                }

                // if not, try the suffix ending at i
                for (int j = 0; j < i; ++j)
                {
                    if (d.find(s.substr(j, i - j + 1)) != d.end())
                        b[i] = i - j + 1;
                }
            }

            int i = s.size() - 1;
            while ((i > 0) && (b[i] > 0))
                i = i - b[i];

            return i < 0;
        }

        void test()
        {
            VERIFY(isWordBreakable("leetcode", { "leet", "code" }));
            VERIFY(isWordBreakable("goldmedalwinner", { "go", "gold", "old", "med", "medal", "win", "winner" }));
        }
    };

    /* Reorder List */
    // LeetCode#143
    // Given a singly linked list L:
    // L0 -> L1->  ... -> Ln-1 -> Ln
    // Reorder it to:
    // L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 -> ...
    // For example, given { 1, 2, 3, 4 }, reorder it to { 1, 4, 2, 3 }. You must do this in-place 
    // without altering the nodes' values.
    struct p143
    {
        // given a Linked list (head), return median (m) node, (N-1)/2 where N is total number of
        // nodes
        template<typename T>
        PListNode<T> getMedianNode(const PListNode<T> head)
        {
            PListNode<T> fp;
            PListNode<T> sp;

            fp = head;
            while (fp)
            {
                fp = fp->next;
                sp = sp ? sp->next : head;

                if (fp)
                    fp = fp->next;
            }

            return sp;
        }

        template<typename T>
        PListNode<T> reverse_list(const PListNode<T> head)
        {
            if (!head || !head->next)
                return head;

            PListNode<T> p;
            PListNode<T> c = head;
            while (c)
            {
                PListNode<T> n = c->next;
                c->next = p;

                p = c;
                c = n;
            }

            return p;
        }

        void makeZigZag(PListNode<int>& head)
        {
            if (!head || !head->next)
                return;

            // 1st sublist
            PListNode<int> h = head;

            // get the median node which points to m-1'th of 2m | m'th of 2m+1 nodes in the list
            PListNode<int> m = getMedianNode(head);

            // build 2nd sub-list
            PListNode<int> t = reverse_list(m->next);

            // "ground" the 1st sub-list
            m->next = nullptr;

            PListNode<int> p = h;
            h = h->next;
            while (h || t)
            {
                if (t)
                {
                    p->next = t;
                    p = p->next;
                    t = t->next;
                }

                if (h)
                {
                    p->next = h;
                    p = p->next;
                    h = h->next;
                }
            }
        }

        void test()
        {
            auto h1 = vector<int>{ 1, 2, 3, 4 };
            auto h2 = vector<int>{ 1, 2, 3, 4, 5 };

            auto c = convert_to(h1);
            makeZigZag(c);
            VERIFY(convert_from(c) == vector<int>({ 1, 4, 2, 3 }));

            c = convert_to(vector<int>{});
            makeZigZag(c);
            VERIFY(!c);

            c = convert_to(vector<int>{ 0 });
            makeZigZag(c);
            VERIFY(convert_from(c) == vector<int>({ 0 }));

            c = convert_to(h1);
            makeZigZag(c);
            VERIFY(convert_from(c) == vector<int>({ 1, 4, 2, 3 }));

            c = convert_to(h2);
            makeZigZag(c);
            VERIFY(convert_from(c) == vector<int>({ 1, 5, 2, 4, 3 }));
        }
    };

    /* LRU Cache */
    // LeetCode#146
    struct p146
    {
        template<typename K, typename V>
        class LRUCache
        {
        public:
            LRUCache(int capacity) :
                m_capacity(capacity)
            {
            }

            bool get(const K& key, shared_ptr<V>& pValue)
            {
                if (m_keys.find(key) == m_keys.end())
                    return false;

                pValue = m_keys[key]->second;
                m_values.erase(m_keys[key]);

                m_values.push_front(make_pair(key, pValue));
                m_keys[key] = m_values.begin();

                return true;
            }

            void set(const K& key, shared_ptr<V>& pValue)
            {
                if (m_keys.find(key) != m_keys.end())
                    m_values.erase(m_keys[key]);

                m_values.push_front(make_pair(key, pValue));
                m_keys[key] = m_values.begin();

                if (m_values.size() > m_capacity)
                {
                    m_keys.erase(m_values.back().first);
                    m_values.pop_back();
                }
            }

        private:
            int m_capacity;

            list<pair<K, shared_ptr<V>>> m_values;
            unordered_map<K, typename list<pair<K, shared_ptr<V>>>::iterator> m_keys;
        };

        void test()
        {
            unique_ptr<LRUCache<string, int>> pc = make_unique<LRUCache<string, int>>(3);

            pc->set("one", make_shared<int>(1));

            shared_ptr<int> pi;
            VERIFY(pc->get("one", pi));
            VERIFY(*pi == 1);

            pc->set("two", make_shared<int>(2));
            pc->set("three", make_shared<int>(3));
            pc->set("four", make_shared<int>(4));

            VERIFY(!pc->get("one", pi));

            pc->set("five", make_shared<int>(5));
            // "two" should have been pushed out
            VERIFY(!pc->get("two", pi));

            // refresh "three"
            pc->get("three", pi);
            pc->set("six", make_shared<int>(6));
            VERIFY(!pc->get("four", pi));

            //LOG("usageCount:" << lru1::Foo::usageCount << ", usageCountMax:" << lru1::Foo::usageCountMax);
        }
    };

    /* Evaluate Reverse Polish Notation */
    // LeetCode#150
    // Evaluate the value of an arithmetic expression in Reverse Polish Notation.Valid operators are
    // + , -, *, /. Each operand may be an integer or another expression.
    //  [ "2", "1", "+", "3", "*" ] -> ((2 + 1) * 3) -> 9
    //  [ "4", "13", "5", "/", "+" ] -> (4 + (13 / 5)) -> 6
    struct p150
    {
        vector<string> tokenize(const string& expr)
        {
            return vector<string>(
                istream_iterator<string>(istringstream(expr)),
                istream_iterator<string>());
        }

        int evalRPN(const string& expr)
        {
            const string ops = "+-*/";
            stack<int> st;
            for (auto& s : tokenize(expr))
            {
                if (ops.find_first_of(s) != string::npos)
                {
                    int op2 = st.top();
                    st.pop();
                    int op1 = st.top();
                    st.pop();

                    int val = 0;
                    if (s == "+")
                        val = op1 + op2;
                    else if (s == "-")
                        val = op1 - op2;
                    else if (s == "*")
                        val = op1 * op2;
                    else if (s == "/")
                        val = op1 / op2;

                    st.push(val);
                }
                else
                {
                    st.push(atoi(s.c_str()));
                }
            }

            int r = st.top();
            st.pop();

            // assert(st.empty());
            return r;
        }

        void test()
        {
            VERIFY(9 == evalRPN("2 1 + 3 *"));
            VERIFY(6 == evalRPN("4 13 5 / +"));
        }
    };

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
        int getMaxProfit(const vector<int>& v, int k)
        {
            vector<vector<int>> dp(k + 1, vector<int>(v.size()));

            for (int k = 1; k < dp.size(); ++k)
            {
                // Let
                //  T_i = Max { dp[k - 1][j] + v[i] - v[j] } where j = 0..i-1
                //      = Max { dp[k - 1][j] - v[j] } + v[i] where j = 0..i-1
                // Then (we have a recurring formula for computing T_i as,
                //  T_i+ = Max { T_i, dp[k - 1][i] - v[i] } + v[i+]

                int pv = dp[k - 1][0] - v[0];
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
                &lcd1::p189::rotateArray_1,
                &lcd1::p189::rotateArray_2,
                &lcd1::p189::rotateArray_3,
                &lcd1::p189::rotateArray_4,
                &lcd1::p189::rotateArray_5,
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

    /* Factor Combinations */
    // LeetCode#254
    // Print all unique combination of factors (except 1) of a given number.
    // For example :
    // Input : 12
    // Output : [[2, 2, 3], [2, 6], [3, 4]]
    // Input : 15
    // Output : [[3, 5]]
    // Input : 28 Output : [[2, 2, 7], [2, 14], [4, 7]]
    struct p254
    {
        vector<int> factorize(int n)
        {
            vector<int> f;
            int m = static_cast<int>(sqrt(n));
            for (int i = 2; i < m; ++i)
            {
                while ((n % i) == 0)
                {
                    n /= i;
                    f.push_back(i);
                }
            }

            if (n > 1)
                f.push_back(n);

            return f;
        }

        /*
        vector<pair<vector<int>, vector<int>>> splitCollection(vector<int>& v, int i)
        {
        if (i >= v.size())
        return{ make_pair(vector<int>(), vector<int>()) };

        vector<pair<vector<int>, vector<int>>> r;
        for (auto p : splitCollection(v, i + 1))
        {
        p.first.push_back(v[i]);
        r.push_back(p);
        p.first.pop_back();

        p.second.push_back(v[i]);
        r.push_back(p);
        p.second.pop_back();
        }

        return r;
        }

        vector<vector<int>> getFactors(int n)
        {
        auto v = factorize(n);

        vector<pair<int, int>> r;
        for (auto p : splitCollection(v, 0))
        {
        int i = 1;
        for (auto x : p.first)
        i *= x;

        int j = 1;
        for (auto x : p.second)
        j *= x;

        r.emplace_back(i, j);
        }

        return vector<vector<int>>();
        }
        */

        void getFactorsbt(vector<vector<int>>& r, vector<int>& c, int i, int n)
        {
            if ((n == 1) && (c.size() > 1))
            {
                r.push_back(c);
                return;
            }

            for (; i <= n; ++i)
            {
                if ((n % i) == 0)
                {
                    c.push_back(i);
                    getFactorsbt(r, c, i, n / i);
                    c.pop_back();
                }
            }
        }

        vector<vector<int>> getFactors(int n)
        {
            vector<vector<int>> r;
            vector<int> c;
            getFactorsbt(r, c, 2, n);

            return r;
        }

        void test()
        {
            auto r = vector<vector<int>>{ { 2, 2, 7 },{ 2, 14 },{ 4, 7 } };
            VERIFY(r == getFactors(28));
        }
    };

    /* Integer to English Words */
    // LeetCode#273
    // Given an integer between 0 and 999,999 print an English phrase that describes the integer. E.g.
    // 1234 => "One Thousand, Two Hundred and Thirty Four".
    struct p273
    {
        string getNumberString(int n)
        {
            static unordered_map<int, string> numbers = {
                { 0, "zero" },
                { 1, "one" },
                { 2, "two" },
                { 3, "three" },
                { 4, "four" },
                { 5, "five" },
                { 6, "six" },
                { 7, "seven" },
                { 8, "eight" },
                { 9, "nine" },
                { 10, "ten" },
                { 11, "eleven" },
                { 12, "twelve" },
                { 13, "thirteen" },
                { 14, "fourteen" },
                { 15, "fifteen" },
                { 16, "sixteen" },
                { 17, "seventeen" },
                { 18, "eighteen" },
                { 19, "nineteen" },
                { 20, "twenty" },
                { 30, "thirty" },
                { 40, "forty" },
                { 50, "fifty" },
                { 60, "sixty" },
                { 70, "seventy" },
                { 80, "eighty" },
                { 90, "ninety" },
            };

            if (n == 0)
                return numbers[0];

            string s;
            if (n >= 1000)
            {
                s += getNumberString(n / 1000);
                s += " thousand";
                n %= 1000;
            }

            if (n >= 100)
            {
                if (!s.empty())
                    s += ", ";

                s += numbers[n / 100];
                s += " hundred";
                n %= 100;
            }

            if (n > 0)
            {
                if (!s.empty())
                    s += " and ";

                if (n >= 20)
                {
                    s += numbers[(n / 10) * 10];
                    n %= 10;

                    if (n > 0)
                        s += " ";
                }

                if (n > 0)
                    s += numbers[n];
            }

            return s;
        }

        void test()
        {
            VERIFY("zero" == getNumberString(0));
            VERIFY("nine hundred and ninety nine thousand, nine hundred and ninety nine" == getNumberString(999999));
            VERIFY("one thousand, two hundred and thirty four" == getNumberString(1234));
            VERIFY("twelve" == getNumberString(12));
            VERIFY("ninety three thousand, two hundred and thirteen" == getNumberString(93213));
            VERIFY("six hundred and fifty two" == getNumberString(652));
        }
    };

    /* Move Zeroes */
    // LeetCode#283
    // Given an array nums, write a function to move all 0's to the end of it while maintaining the 
    // relative order of the non-zero elements. For example, given nums = [0, 1, 0, 3, 12], after 
    // calling your function, nums should be [1, 3, 12, 0, 0].
    // Note:
    // - You must do this in-place without making a copy of the array.
    // - Minimize the total number of operations.
    struct p283
    {
        int moveZeroes_0(vector<int>& v)
        {
            // j => write pointer, i => read pointer
            int j = 0;
            int i = 0;
            while (i < v.size())
            {
                if (v[i] != 0)
                    v[j++] = v[i];

                ++i;
            }

            int count = 0;
            while (j < v.size())
            {
                if (v[j] != 0)
                {
                    v[j] = 0;
                    ++count;
                }

                ++j;
            }

            return count;
        }

        int moveZeroes(vector<int>& v)
        {
            int i = 0;
            int j = v.size() - 1;
            int count = 0;
            while (i < j)
            {
                while ((i < j) && (v[i] != 0))
                    ++i;

                while ((i < j) && (v[j] == 0))
                    --j;

                if (i != j)
                {
                    swap(v[i++], v[j--]);
                    ++count;
                }
            }

            return count;
        }

        void test()
        {
            auto iv = vector<int>({ 0, 1, 0, 3, 12 });
            VERIFY(2 == moveZeroes(iv));
            VERIFY((iv[3] == 0) && (iv[4] == 0));

            VERIFY(0 == moveZeroes(iv));

            iv = vector<int>({ 0, 0, 0, 0, 0 });
            VERIFY(0 == moveZeroes(iv));
        }
    };

    /* Word Pattern II */
    // LeetCode#291
    // Given a pattern and a string str, find if str follows the same pattern. Here follow means a 
    // full match, such that there is a bijection between a letter in pattern and a non-empty substring 
    // in str.
    // Examples:
    // pattern = "abab", str = "redblueredblue" should return true.
    // pattern = "aaaa", str = "asdasdasdasd" should return true.
    // pattern = "aabb", str = "xyzabcxzyabc" should return false.
    struct p291
    {
        bool isWordPatternMatchHelper(
            const string& p,
            int i,
            const string& t,
            int j,
            unordered_map<char, string>& m,
            unordered_set<string>& s)
        {
            if ((i == p.size()) && (j == t.size()))
                return true;

            char c = p[i];
            for (int k = j; k < t.size(); ++k)
            {
                string v = t.substr(j, k - j + 1);
                if (m.find(c) == m.end())
                {
                    if (s.find(v) != s.end())
                        continue;

                    m[c] = v;
                    s.insert(m[c]);

                    if (isWordPatternMatchHelper(p, i + 1, t, k + 1, m, s))
                        return true;

                    s.erase(m[c]);
                    m.erase(c);
                }
                else
                {
                    if ((m[c] == v) && isWordPatternMatchHelper(p, i + 1, t, k + 1, m, s))
                        return true;
                }
            }

            return false;
        }

        bool isWordPatternMatch(const string& pattern, const string& text)
        {
            unordered_map<char, string> m;
            unordered_set<string> s;
            return isWordPatternMatchHelper(pattern, 0, text, 0, m, s);
        }

        void test()
        {
            VERIFY(isWordPatternMatch("abab", "redblueredblue"));
            VERIFY(isWordPatternMatch("aaaa", "asdasdasdasd"));
            VERIFY(!isWordPatternMatch("aabb", "xyzabcxzyabc"));
            VERIFY(isWordPatternMatch("abab", "xyzxyxyzxy"));
        }
    };

    /* Find Median from Data Stream */
    // LeetCode#295
    // Median is the middle value in an ordered integer list. If the size of the list is even, there 
    // is no middle value. So the median is the mean of the two middle value.
    //
    // Given a stream of unsorted integers, find the median element in sorted order at any given
    // time.So, we will be receiving a continuous stream of numbers in some random order and we
    // don’t know the stream length in advance.Write a function that finds the median of the already
    // received numbers efficiently at any time.We will be asked to find the median multiple times.
    // Just to recall, median is the middle element in an odd length sorted array, and in the even
    // case it’s the average of the middle elements.
    struct p295
    {
        int findMedian_0(vector<int> v)
        {
            priority_queue<int, vector<int>, greater<int>> upper;
            priority_queue<int> lower;

            auto heapSwap = [&upper, &lower](int x, bool isLower)
            {
                int y;
                if (isLower)
                {
                    y = lower.top();
                    lower.pop();
                    lower.push(x);
                }
                else
                {
                    y = upper.top();
                    upper.pop();
                    upper.push(x);
                }

                return y;
            };

            int m = 0;
            bool isMedianValid = false;
            for (int i = 0; i < v.size(); ++i)
            {
                int x = v[i];
                if (lower.empty())
                {
                    lower.push(x);
                }
                else if (upper.empty())
                {
                    if (x < lower.top())
                        x = heapSwap(x, true);
                    upper.push(x);
                }
                else if (!isMedianValid)
                {
                    if (x < lower.top())
                        x = heapSwap(x, true);

                    if (x > upper.top())
                        x = heapSwap(x, false);

                    m = x;
                    isMedianValid = true;
                }
                else
                {
                    if (x < m)
                    {
                        lower.push(x);
                        upper.push(m);
                    }
                    else
                    {
                        lower.push(m);
                        upper.push(x);
                    }

                    isMedianValid = false;
                }
            }

            if (isMedianValid)
                return m;

            if (lower.empty() || upper.empty())
                return lower.empty() ? upper.top() : lower.top();

            return (lower.top() + upper.top()) / 2;
        }

        int findMedian_1(vector<int> v)
        {
            priority_queue<int, vector<int>, greater<int>> upper;
            priority_queue<int> lower;

            unique_ptr<int> pi;
            for (int i = 0; i < v.size(); ++i)
            {
                int x = v[i];
                if (lower.empty())
                {
                    lower.push(x);
                }
                else if (upper.empty())
                {
                    if (x < lower.top())
                    {
                        int tmp = lower.top();
                        lower.pop();
                        lower.push(x);
                        x = tmp;
                    }

                    upper.push(x);
                }
                else if (pi == nullptr)
                {
                    if (x > upper.top())
                    {
                        int tmp = upper.top();
                        upper.pop();
                        upper.push(x);
                        x = tmp;
                    }

                    if (x < lower.top())
                    {
                        int tmp = lower.top();
                        lower.pop();
                        lower.push(x);
                        x = tmp;
                    }

                    pi.reset(new int(x));
                }
                else
                {
                    if (*pi < x)
                    {
                        lower.push(*pi);
                        upper.push(x);
                    }
                    else
                    {
                        lower.push(x);
                        upper.push(*pi);
                    }

                    pi.release();
                }
            }

            return pi != nullptr ? *pi : (lower.top() + upper.top()) / 2;
        }

        int findMedian(vector<int> v)
        {
            priority_queue<int, vector<int>, greater<int>> upper;
            priority_queue<int> lower;

            int N = 0;
            int m;
            for (int i = 0; i < v.size(); ++i)
            {
                if (N % 2)
                {
                    if (v[i] < m)
                    {
                        lower.push(v[i]);
                        upper.push(m);
                    }
                    else
                    {
                        lower.push(m);
                        upper.push(v[i]);
                    }
                }
                else
                {
                    if (lower.empty() || (v[i] < lower.top()))
                    {
                        lower.push(v[i]);
                        m = lower.top();
                        lower.pop();
                    }
                    else
                    {
                        upper.push(v[i]);
                        m = upper.top();
                        upper.pop();
                    }
                }

                N += 1;
            }

            return (N % 2) ? m : (lower.top() + upper.top()) / 2;
        }

        void test()
        {
            VERIFY(1 == findMedian({ 1 }));
            VERIFY(2 == findMedian({ 1, 3 }));
            VERIFY(3 == findMedian({ 1, 2, 3, 4, 5 }));
            VERIFY(4 == findMedian({ 1, 2, 3, 5, 6, 7 }));
        }
    };

    /* Serialize and Deserialize Binary Tree */
    // LeetCode#297
    // Serialization is the process of converting a data structure or object into a sequence of bits so 
    // that it can be stored in a file or memory buffer, or transmitted across a network connection link 
    // to be reconstructed later in the same or another computer environment.
    // Design an algorithm to serialize and deserialize a binary tree. There is no restriction on how 
    // your serialization/deserialization algorithm should work. You just need to ensure that a binary 
    // tree can be serialized to a string and this string can be deserialized to the original tree 
    // structure.
    struct p297
    {
        void serializeBinaryTreeHelper(PTreeNode<int>& p, string& s)
        {
            if (!p)
            {
                s += " #";
                return;
            }

            s += " " + to_string(p->val);
            serializeBinaryTreeHelper(p->left, s);
            serializeBinaryTreeHelper(p->right, s);
        }

        string serializeBinaryTree(BinaryTree<int>& b)
        {
            string s;
            serializeBinaryTreeHelper(b.getRoot(), s);

            return s;
        }

        class tokenizer
        {
        public:
            tokenizer(const string& s) :
                v(istream_iterator<string>(istringstream(s)), istream_iterator<string>()),
                i(0)
            {
            }

            bool haxNext()
            {
                return i < v.size();
            }

            string getNext()
            {
                return v[i++];
            }

        private:
            vector<string> v;
            int i;
        };

        PTreeNode<int> deserializeBinaryTreeHelper(PTreeNode<int>& p, tokenizer& tk)
        {
            if (!tk.haxNext())
                return nullptr;

            string t = tk.getNext();
            if (t == "#")
                return nullptr;

            int x = atoi(t.c_str());

            PTreeNode<int> n(new TreeNode<int>{ x });
            n->parent = p;
            n->left = deserializeBinaryTreeHelper(n, tk);
            n->right = deserializeBinaryTreeHelper(n, tk);

            return n;
        }

        BinaryTree<int> deserializeBinaryTree(const string& s)
        {
            tokenizer tk(s);
            return BinaryTree<int>(deserializeBinaryTreeHelper(PTreeNode<int>(), tk));
        }

        void test()
        {
            BinaryTree<int> b = getRandom(BinaryTree<int>(), getRandom(0, 100, 20));
            auto s1 = serializeBinaryTree(b);
            auto s2 = serializeBinaryTree(deserializeBinaryTree(s1));
            VERIFY(s1 == s2);
        }
    };

    /* Bulls and Cows */
    // LeetCode#299
    // You are playing the following Bulls and Cows game with your friend: You write down a number and
    // ask your friend to guess what the number is.Each time your friend makes a guess, you provide a
    // hint that indicates how many digits in said guess match your secret number exactly in both digit
    // and position(called "bulls") and how many digits match the secret number but locate in the wrong
    // position(called "cows").Your friend will use successive guesses and hints to eventually derive
    // the secret number.
    // For example:
    //  Secret number : "1807"
    //  Friend's guess: "7810"
    // Hint : 1 bull and 3 cows. (The bull is 8, the cows are 0, 1 and 7.)
    // Write a function to return a hint according to the secret number and friend's guess, use A to 
    // indicate the bulls and B to indicate the cows.In the above example, your function should return
    // "1A3B".
    struct p299
    {
        string getBullsAndCows(const string& secret, const string& guess)
        {
            unordered_map<char, int> m;
            for (auto& c : secret)
                m[c] += 1;

            int b = 0;
            for (int i = 0; i < guess.size(); ++i)
            {
                if (guess[i] == secret[i])
                {
                    m[guess[i]] -= 1;
                    ++b;
                }
            }

            int c = 0;
            for (int i = 0; i < guess.size(); ++i)
            {
                if ((m.find(guess[i]) != m.end()) && (m[guess[i]] > 0))
                {
                    ++c;
                    m[guess[i]] -= 1;
                }
            }

            return to_string(b) + "A" + to_string(c) + "B";
        }

        void test()
        {
            VERIFY("1A3B" == getBullsAndCows("1807", "7810"));
            VERIFY("1A2B" == getBullsAndCows("1807", "8810"));
            VERIFY("1A0B" == getBullsAndCows("1807", "8888"));
            VERIFY("0A0B" == getBullsAndCows("1807", "9236"));
            VERIFY("4A0B" == getBullsAndCows("1807", "1807"));
        }
    };

    /* Remove Invalid Parentheses */
    // LeetCode#301
    // Remove the minimum number of invalid parentheses in order to make the input string valid. 
    // Return all possible results. Note: The input string may contain letters other than the 
    // parentheses ( and ).
    // Examples:
    // "()())()" -> [ "()()()", "(())()" ]
    // "(a)())()" -> [ "(a)()()", "(a())()" ]
    // ")(" -> [ "" ]
    struct p301
    {
        vector<string> removeInvalidParens(const string& s)
        {
            auto isValid = [](string& s)
            {
                int pcount = 0;
                int i = 0;
                int j = -1;
                while ((j = s.find_first_of("()", i)) != string::npos)
                {
                    pcount += (s[j] == '(') ? 1 : -1;
                    if (pcount < 0)
                        return false;

                    i = j + 1;
                }

                return pcount == 0;
            };

            // result vector
            vector<string> r;

            // queue of substrings that removes a single '(' or ')' to for a new substring
            deque<string> q = { s };

            // since we are doing a BFS here and the outer loop is processing a single level, we stop
            // as soon as we find any valid substrings after a certain level since any further processing
            // will only generater smaller strings.
            while (!q.empty() && r.empty())
            {
                // BFS, process the level
                int count = q.size();

                // keep track of 'sibling' strings that are already added in the queue.
                unordered_set<string> b;
                while (count-- > 0)
                {
                    string& t = q.front();
                    if (!isValid(t))
                    {
                        int i = 0;
                        int j = -1;
                        while ((j = t.find_first_of("()", i)) != string::npos)
                        {
                            string ts = t.substr(0, j) + t.substr(j + 1);
                            if (b.find(ts) == b.end())
                            {
                                q.push_back(ts);
                                b.insert(ts);
                            }

                            i = j + 1;
                        }
                    }
                    else
                    {
                        r.push_back(t);
                    }

                    q.pop_front();
                }
            }

            return r;
        }

        void test()
        {
            VERIFY(removeInvalidParens("()())()") == vector<string>({ "(())()", "()()()" }));
            VERIFY(removeInvalidParens("(a)())()") == vector<string>({ "(a())()", "(a)()()" }));
            VERIFY(removeInvalidParens(")(") == vector<string>({ "" }));
        }
    };

    /* Number of Islands II */
    // LeetCode#305
    // A 2d grid map of m rows and n columns is initially filled with water. We may perform an 
    // addLand operation which turns the water at position (row, col) into a land.Given a list of
    // positions to operate, count the number of islands after each addLand operation. An island is
    // surrounded by water and is formed by connecting adjacent lands horizontally or vertically.
    // You may assume all four edges of the grid are all surrounded by water.
    struct p305
    {
        template<typename T, typename H = hash<T>>
        class DisjointSet
        {
        public:
            DisjointSet() :
                m_count(0)
            {
            }

            void add(const T& i)
            {
                if (m_root.find(i) == m_root.end())
                {
                    m_root[i] = i;
                    ++m_count;
                }
            }

            void join(const T& i, const T& j)
            {
                add(i);
                add(j);

                auto ri = i;
                while (m_root[ri] != ri)
                    ri = m_root[ri];

                auto rj = j;
                while (m_root[rj] != rj)
                    rj = m_root[rj];

                if (ri != rj)
                {
                    m_root[ri] = rj;
                    m_count--;
                }
            }

            int count() const
            {
                return m_count;
            }

        private:
            unordered_map<T, T, H> m_root;
            int m_count;
        };

        vector<int> getNumberOfIslands_0(const vector<vector<int>>& m, const vector<pair<int, int>>& addOps)
        {
            vector<vector<int>> g(m);

            const vector<pair<int, int>> offsets = {
                { 0, -1 }, // left
                { -1, -1 }, // top-left
                { -1, 0 }, // immediate-top
                { -1, 1 }, // top-right
                { 0, 1 }, // right
                { 1, 1 }, // bottom-right
                { 1, 0 }, // immediate-bottom
                { 1, -1 }, // bottom-left
            };

            struct pair_hasher
            {
                size_t operator()(const pair<int, int>& p) const
                {
                    return hash_value(p.first) ^ hash_value(p.second);
                }
            };

            vector<int> r;
            DisjointSet<pair<int, int>, pair_hasher> d;
            for (int i = 0; i < g.size(); ++i)
                for (int j = 0; j < g[0].size(); ++j)
                {
                    if (g[i][j] == 0)
                        continue;

                    pair<int, int> p = { i, j };
                    d.add(p);

                    for (auto o : offsets)
                    {
                        o.first += p.first;
                        o.second += p.second;

                        if ((o.first >= 0) &&
                            (o.first < g.size()) &&
                            (o.second >= 0) &&
                            (o.second < g[0].size()) &&
                            (g[o.first][o.second] == 1))
                            d.join(p, o);
                    }
                }

            r.push_back(d.count());

            for (auto& p : addOps)
            {
                g[p.first][p.second] = 1;
                d.add(p);

                for (auto o : offsets)
                {
                    o.first += p.first;
                    o.second += p.second;

                    if ((o.first >= 0) &&
                        (o.first < g.size()) &&
                        (o.second >= 0) &&
                        (o.second < g[0].size()) &&
                        (g[o.first][o.second] == 1))
                        d.join(p, o);
                }

                r.push_back(d.count());
            }

            return r;
        }

        vector<int> getNumberOfIslands(const vector<vector<int>>& m, const vector<pair<int, int>>& addOps)
        {
            vector<vector<int>> g(m);

            // union-find
            vector<int> roots(g.size() * g[0].size(), -1);
            int count = 0;

            auto add = [&g, &roots, &count](int i, int j)
            {
                const vector<pair<int, int>> offsets = {
                    { 0, -1 }, // left
                    { -1, -1 }, // top-left
                    { -1, 0 }, // immediate-top
                    { -1, 1 }, // top-right
                    { 0, 1 }, // right
                    { 1, 1 }, // bottom-right
                    { 1, 0 }, // immediate-bottom
                    { 1, -1 }, // bottom-left
                };

                int x = i * g.size() + j;
                roots[x] = x;
                ++count;

                for (auto& o : offsets)
                {
                    int oi = i + o.first;
                    int oj = j + o.second;
                    if ((oi < 0) || (oi >= g.size()) || (oj < 0) || (oj >= g[0].size()))
                        continue;

                    int y = oi * g.size() + oj;
                    if ((g[oi][oj] == 1) && (roots[y] != -1))
                    {
                        int rx = x;
                        while (rx != roots[rx])
                            rx = roots[rx];

                        int ry = y;
                        while (ry != roots[ry])
                            ry = roots[ry];

                        if (rx != ry)
                        {
                            roots[ry] = rx;
                            --count;
                        }
                    }
                }
            };

            for (int i = 0; i < g.size(); ++i)
                for (int j = 0; j < g[0].size(); ++j)
                {
                    if (g[i][j] == 1)
                        add(i, j);
                }

            vector<int> r;
            r.push_back(count);

            for (auto& p : addOps)
            {
                g[p.first][p.second] = 1;
                add(p.first, p.second);
                r.push_back(count);
            }

            return r;
        }

        void test()
        {
            VERIFY(vector<int>({ 2, 3, 3, 2, 1 }) == getNumberOfIslands({
                { 0, 0, 0, 0, 1 },
                { 0, 1, 1, 0, 0 },
                { 0, 0, 1, 0, 0 },
                { 0, 0, 0, 0, 0 },
            }, {
                { 3, 0 },
                { 1, 4 },
                { 1, 3 },
                { 2, 1 },
            }));
        }
    };

    /* Super Ugly Number */
    // LeetCode#313
    // Write a program to find the nth super ugly number. Super ugly numbers are positive numbers 
    // whose all prime factors are in the given prime list primes of size k.
    // For example, [1, 2, 4, 7, 8, 13, 14, 16, 19, 26, 28, 32] is the sequence of the first 12 super 
    // ugly numbers given primes = [2, 7, 13, 19] of size 4.
    // Note:
    //  (1) 1 is a super ugly number for any given primes.
    //  (2) The given numbers in primes are in ascending order.
    //  (3) 0 < k = 100, 0 < n = 106, 0 < primes[i] < 1000.
    struct p313
    {
        int getSuperUglyNumber(const vector<int>& primes, int n)
        {
            using pqitem = tuple<int, int, int>;

            priority_queue <pqitem, vector<pqitem>, greater<pqitem>> pq;
            for (int i = 0; i < primes.size(); ++i)
                pq.emplace(primes[i], primes[i], 0);

            vector<int> v;
            v.reserve(n);

            v.push_back(1);
            while (--n > 0)
            {
                while (get<0>(pq.top()) == v.back())
                {
                    int p = get<1>(pq.top());
                    int j = get<2>(pq.top());

                    pq.emplace(make_tuple(v[j + 1] * p, p, j + 1));
                    pq.pop();

                    continue;
                }

                v.push_back(get<0>(pq.top()));
            }

            return v.back();
        }

        void test()
        {
            VERIFY(12 == getSuperUglyNumber({ 2, 3, 5 }, 10));
            VERIFY(32 == getSuperUglyNumber({ 2, 7, 13, 19 }, 12));
        }
    };

    /* Remove Duplicate Letters */
    // LeetCode#316
    // Given a string which contains only lowercase letters, remove duplicate letters so that every 
    // letter appear once and only once. You must make sure your result is the smallest in 
    // lexicographical order among all possible results.
    // Example:
    // Given
    //  "bcabc"
    // Return
    //  "abc"
    // Given
    //  "cbacdcbc"
    // Return
    //  "acdb"
    struct p316
    {
        string removeDuplicates(const string& s)
        {
            // char frequency counter
            unordered_map<char, int> m;
            for (int i = 0; i < s.size(); ++i)
                m[s[i]] += 1;

            // candidate string array
            vector<char> r;
            // keeps track of all chars added to candidate string
            unordered_set<char> t;
            for (int i = 0; i < s.size(); ++i)
            {
                // once pass the current index, we are left with one less of it.
                m[s[i]] -= 1;

                // if we have already added it in the candidate string, ignore any subsequent copies 
                // since we know that all the chars added after it in the string would be higher in 
                // lexicographical order (otherwise we would have evicted it already in the next few
                // statements).
                if (t.find(s[i]) != t.end())
                    continue;

                // Evict any "larger" chars from string with copies still available later.
                while (!r.empty() && (r.back() > s[i]) && (m[r.back()] > 0))
                {
                    t.erase(r.back());
                    r.pop_back();
                }

                r.push_back(s[i]);
                t.insert(s[i]);
            }

            return string(r.begin(), r.end());
        }

        void test()
        {
            VERIFY("abc" == removeDuplicates("bcabc"));
            VERIFY("acdb" == removeDuplicates("cbacdcbc"));

            VERIFY("abc" == removeDuplicates("abc"));
            VERIFY("" == removeDuplicates(""));
            VERIFY("a" == removeDuplicates("a"));
            VERIFY("a" == removeDuplicates("aa"));
            VERIFY("ab" == removeDuplicates("ab"));
            VERIFY("ba" == removeDuplicates("ba"));
        }
    };

    /* Maximum Size Subarray Sum Equals k */
    // LeetCode#325
    struct p325
    {
        pair<int, int> findMaxSubArrayWithSum(const vector<int>& v, int k)
        {
            pair<int, int> r = { -1, -1 };

            // m contains the {cumulative sum, end index} pair
            unordered_map<int, int> m = { { 0, -1 } };

            int cs = 0;
            for (int i = 0; i < v.size(); ++i)
            {
                cs += v[i];

                // if we have a index with the target sum (k), we're done
                if (m.find(cs - k) != m.end())
                {
                    int j = m[cs - k] + 1;
                    if ((r.first < 0) || ((i - j) >(r.second - r.first)))
                        r = { j, i };
                }

                // add the sum with current index only if it's not there already i.e. keep the 
                // earliest possible index as is in order to help find the largest subarray.
                if (m.find(cs) == m.end())
                    m[cs] = i;
            }

            return r;
        }

        void test()
        {
            VERIFY(make_pair(2, 4) == findMaxSubArrayWithSum({ 25, 12, 14, 22, 19, 15, 10, 23 }, 55));
            VERIFY(make_pair(5, 6) == findMaxSubArrayWithSum({ 25, 12, 14, 22, 19, 15, 10, 23 }, 25));
            VERIFY(make_pair(7, 7) == findMaxSubArrayWithSum({ 25, 12, 14, 22, 19, 15, 10, 23 }, 23));
            VERIFY(make_pair(0, 0) == findMaxSubArrayWithSum({ 23, 12, 14, 22, 19, 15, 10, 25 }, 23));
        }
    };

    /* Odd Even Linked List */
    // LeetCode#328
    // Given a singly linked list, group all odd nodes together followed by the even nodes. Please 
    // note here we are talking about the node number and not the value in the nodes. The program 
    // should run in O(1) space complexity and O(nodes) time complexity. Example:
    // Given
    //  1->2->3->4->5->NULL,
    // Return
    //  1->3->5->2->4->NULL.
    struct p328
    {
        PListNode<int> makeOddEven(PListNode<int>& head)
        {
            PListNode<int> tail = head;
            while (tail->next)
                tail = tail->next;

            PListNode<int> p = head;
            PListNode<int> t = tail;
            while (p != tail)
            {
                // take the every 2nd node and append it to the end
                PListNode<int> tmp = p->next;
                p->next = p->next->next;

                t->next = tmp;

                t = t->next;
                t->next.reset();

                p = p->next;
            }

            return head;
        }

        void makeOddEven_1(PListNode<int>& head)
        {
            PListNode<int> op = head;
            PListNode<int> eh;
            PListNode<int> ep;

            while (op->next)
            {
                if (!eh)
                {
                    eh = op->next;
                    ep = eh;
                }
                else
                {
                    ep->next = op->next;
                    ep = ep->next;
                }

                op->next = op->next->next;
                if (op->next)
                    op = op->next;
            }

            op->next = eh;
            ep->next = nullptr;
        }

        void test()
        {
            VERIFY(vector<int>({ 1, 3, 5, 2, 4 }) == convert_from(
                makeOddEven(convert_to(vector<int>{ 1, 2, 3, 4, 5 }))));

            auto h1 = vector<int>{ 1, 2, 3, 4 };
            auto h2 = vector<int>{ 1, 2, 3, 4, 5 };

            auto c = convert_to(h1);
            makeOddEven(c);
            VERIFY(convert_from(c) == vector<int>({ 1, 3, 2, 4 }));

            c = convert_to(h2);
            makeOddEven(c);
            VERIFY(convert_from(c) == vector<int>({ 1, 3, 5, 2, 4 }));
        }
    };

    /* Longest Increasing Path in a Matrix */
    // LeetCode#329
    // Given an integer matrix, find the length of the longest increasing path.
    // From each cell, you can either move to four directions : left, right, up or down. You may NOT
    // move diagonally or move outside of the boundary.
    struct p329
    {
        int getMaxIncreasingPath(const vector<vector<int>>& m)
        {
            // TODO
            return 0;
        }

        void test()
        {
            VERIFY(0 == getMaxIncreasingPath({
                { 0, 0, 0 },
                { 0, 0, 0 },
                { 0, 0, 0 },
            }));
        }
    };

    /* Palindrome Pairs */
    // LeetCode#336
    // Given a list of unique words. Find all pairs of distinct indices(i, j) in the given list, so
    // that the concatenation of the two words, i.e.words[i] + words[j] is a palindrome.
    // Example 1:
    // Given words = ["bat", "tab", "cat"], Return[[0, 1], [1, 0]]
    struct p336
    {
        vector<pair<int, int>> getPalindromePairs(const vector<string>& v)
        {
            auto isPalindrome = [](string& s)
            {
                if (s.size() < 2)
                    return true;

                int i = 0;
                int j = s.size() - 1;
                while ((i < j) && (s[i] == s[j]))
                    ++i, --j;

                return i == j;
            };

            vector<pair<int, int>> r;
            vector<string> rv(v);
            for (int i = 0; i < v.size(); ++i)
                reverse(rv[i].begin(), rv[i].end());

            for (int i = 0; i < v.size() - 1; ++i)
                for (int j = i + 1; j < v.size(); ++j)
                {
                    int x = i;
                    int y = j;
                    if (v[i].size() > v[j].size())
                        swap(x, y);

                    if ((v[x] == rv[y].substr(0, v[x].size())) &&
                        isPalindrome(rv[y].substr(v[x].size())))
                    {
                        r.emplace_back(i, j);
                        r.emplace_back(j, i);
                    }
                }

            return r;
        }

        void test()
        {
            VERIFY(getPalindromePairs({ "bat", "tab", "cat" }) ==
                (vector<pair<int, int>>({ { 0, 1 },{ 1, 0 } })));
        }
    };

    /* Reverse Vowels of a String */
    // LeetCode#345
    // Write a function that takes a string as input and reverse only the vowels of a string.
    struct p345
    {
        string reverseVowels(string s)
        {
            auto isVowel = [](char c)
            {
                //const string vowels = "aeiouAEIOU";
                //return vowels.find_first_of(c) != string::npos;

                //const unordered_set<char> vowels = {
                //    'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'
                //};
                //return vowels.find(c) != vowels.end();

                switch (c)
                {
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                case 'A':
                case 'E':
                case 'I':
                case 'O':
                case 'U':
                    return true;
                default:
                    return false;
                }
            };

            int i = 0;
            int j = s.size() - 1;
            while (i < j)
            {
                if (!isVowel(s[i]))
                {
                    ++i;
                    continue;
                }

                if (!isVowel(s[j]))
                {
                    --j;
                    continue;
                }

                swap(s[i++], s[j--]);
            }

            return s;
        }

        void test()
        {
            VERIFY("This es a sampli string" == reverseVowels("This is a sample string"));
            VERIFY("Trei of leberty" == reverseVowels("Tree of liberty"));
        }
    };

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
        p022().test();
    }

    //REGISTER_RUNNABLE(lcd1)
}
