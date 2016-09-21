#include "stdafx.h"

// Get-Content .\LCD_01.cpp | findstr "struct p*" | ?{ $_ -match "struct p(\d+)" } | Measure-Object

namespace lcd2
{
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
            {
                r[lo].first = min(r[lo].first, x.first);
                r[lo].second = max(r[lo].second, x.second);
            }
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
        vector<interval> insertInterval_2(const vector<interval>& v, const interval& x)
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

        vector<interval> insertInterval(vector<interval> v, const interval& x)
        {
            // find the (inclusive) lower bound and insert right before that.
            auto p = lower_bound(v.begin(), v.end(), x);
            p = v.insert(p, x);

            // step back to the immediate previous interval if it overlaps to set the point from
            // where to start merging down
            if ((p != v.begin()) && ((p - 1)->second >= p->first))
                --p;

            while (((p + 1) != v.end()) && ((p + 1)->first <= p->second))
            {
                p->second = max(p->second, (p + 1)->second);
                v.erase(p + 1);
            }

            return v;
        }

        void test()
        {
            auto f = &p057::insertInterval;

            VERIFY(vector<interval>({
                { 1, 2 },
            }) == (this->*f)(vector<interval>({
            }),
            { 1, 2 }));

            VERIFY(vector<interval>({
                { 1, 5 },
                { 6, 9 },
            }) == (this->*f)(vector<interval>({
                { 1, 3 },
                { 6, 9 },
            }),
            { 2, 5 }));

            VERIFY(vector<interval>({
                { 1, 2 },
                { 3, 10 },
                { 12, 16 },
            }) == (this->*f)(vector<interval>({
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
            }) == (this->*f)(vector<interval>({
                { 1, 2 },
                { 6, 9 },
            }),
            { 3, 5 }));

            VERIFY(vector<interval>({
                { 0, 6 },
            }) == (this->*f)(vector<interval>({
                { 1, 5 },
            }),
            { 0, 6 }));

            VERIFY(vector<interval>({
                { 0, 5 },
                { 7, 16 },
            }) == (this->*f)(vector<interval>({
                { 0, 5 },
                { 9, 12 },
            }),
            { 7, 16 }));
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

        vector<string> columnFit_1(const string& s, int columnWidth)
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

        vector<string> columnFit(const string& s, const int columnWidth)
        {
            vector<string> words = vector<string>(
                istream_iterator<string>(istringstream(s)),
                istream_iterator<string>());

            vector<string> lines;
            for (int i = 0, k; i < words.size(); i += k)
            {
                // pick k words that fits the line
                k = 0;

                int len = 0;
                while ((i + k < words.size()) && (len + words[i + k].size() < columnWidth - k))
                {
                    len += words[i + k].size();
                    ++k;
                }

                string line;
                if (k > 1)
                {
                    // number of spaces to distribute across the words (note, the last word doesn't need any
                    // trailng spaces, hence k - 1).
                    string ws((columnWidth - len) / (k - 1), ' ');
                    // extra spaces that needs to be meted out.
                    int ns = (columnWidth - len) % (k - 1);

                    // append (k - 1) words with trailing spaces
                    for (int j = i; j < i + k - 1; ++j)
                    {
                        line += words[j];
                        line += ws;
                        if ((j - i) < ns)
                            line += " ";
                    }
                }

                // assume all words fit within the column-size
                // assert(k > 0);

                // append the final word
                line += words[i + k - 1];

                // add the finished line
                lines.push_back(line);
            }

            return lines;
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

    static void run()
    {
        p057().test();
    }

    REGISTER_RUNNABLE(lcd2)
}
