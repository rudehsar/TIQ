#include "stdafx.h"

namespace lcd7
{
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

            vector<int> v;
            v.reserve(n);
            v.push_back(1);

            for (int i = 0; i < primes.size(); ++i)
                pq.emplace(v[0] * primes[i], primes[i], 0);

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

    /* Count of Smaller Numbers After Self */
    // LeetCode#315
    // You are given an integer array nums and you have to return a new counts array. The counts 
    // array has the property where counts[i] is the number of smaller elements to the right of nums[i].
    // Example:
    // Given nums = [5, 2, 6, 1]
    // To the right of 5 there are 2 smaller elements (2 and 1).
    // To the right of 2 there is only 1 smaller element (1).
    // To the right of 6 there is 1 smaller element (1).
    // To the right of 1 there is 0 smaller element.
    // Return the array [2, 1, 1, 0].
    struct p315
    {
        vector<int> countSmaller(const vector<int>& nums)
        {
            vector<int> r(nums.size());

            vector<int> sq;
            for (int i = nums.size() - 1; i >= 0; --i)
            {
                auto it = lower_bound(sq.begin(), sq.end(), nums[i]);
                it = sq.insert(it, nums[i]);
                r[i] = it - sq.begin();
            }

            return r;
        }

        void test()
        {
            VERIFY(vector<int>({ 2, 1, 1, 0 }) == countSmaller({ 5, 2, 6, 1 }));
            VERIFY(vector<int>({ 2, 0, 0 }) == countSmaller({ 2, 0, 1 }));
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
                    if ((r.first < 0) || ((i - j) > (r.second - r.first)))
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
        // Backtracking
        int getMaxIncreasingPathHelper(
            const vector<vector<int>>& m,
            int i,
            int j,
            vector<vector<bool>>& b,
            vector<vector<int>>& c)
        {
            if (c[i][j] > INT_MIN)
                return c[i][j];

            const vector<pair<int, int>> d = {
                { 1,  0 },
                { 0,  1 },
                { -1,  0 },
                { 0, -1 },
            };

            auto isValidIndex = [&m](int x, int y)
            {
                return (x >= 0)
                    && (x < m.size())
                    && (y >= 0)
                    && (y < m[0].size());
            };

            int p = 0;
            b[i][j] = true;
            for (auto& dx : d)
            {
                int x = i + dx.first;
                int y = j + dx.second;
                if (isValidIndex(x, y) && !b[x][y] && (m[x][y] > m[i][j]))
                    p = max(p, getMaxIncreasingPathHelper(m, x, y, b, c));
            }

            b[i][j] = false;
            c[i][j] = p + 1;

            return c[i][j];
        }

        int getMaxIncreasingPath(const vector<vector<int>>& m)
        {
            int l = 0;
            vector<vector<bool>> b(m.size(), vector<bool>(m[0].size()));
            vector<vector<int>> c(m.size(), vector<int>(m[0].size(), INT_MIN));
            for (int i = 0; i < m.size(); ++i)
                for (int j = 0; j < m[0].size(); ++j)
                {
                    l = max(l, getMaxIncreasingPathHelper(m, i, j, b, c));
                }

            return l;
        }

        void test()
        {
            VERIFY(4 == getMaxIncreasingPath({
                { 3, 4, 5 },
                { 3, 2, 6 },
                { 2, 2, 1 },
            }));

            VERIFY(4 == getMaxIncreasingPath({
                { 9, 9, 4 },
                { 6, 6, 8 },
                { 2, 1, 1 },
            }));

            VERIFY(6 == getMaxIncreasingPath({
                { 7, 8, 9 },
                { 9, 7, 6 },
                { 7, 2, 3 },
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

    static void run()
    {
        p315().test();
    }

    //REGISTER_RUNNABLE(lcd7)
}
