#include "stdafx.h"

namespace lcd3
{
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

    /* Distinct Subsequences */
    // LeetCode#115
    // Given a string S and a string T, count the number of distinct subsequences of T in S. A 
    // subsequence of a string is a new string which is formed from the original string by deleting some 
    // (can be none) of the characters without disturbing the relative positions of the remaining 
    // characters. (ie, "ACE" is a subsequence of "ABCDE" while "AEC" is not).
    // Here is an example:
    //  S = "rabbbit", T = "rabbit"
    // Return 3.
    struct p115
    {
        int numDistinct(const string& s, const string& t)
        {
            if (s.size() < t.size())
                return 0;

            if (t.empty())
                return 1;

            // dp[i][j], i and j offset from end-of-string of t and s resp.
            vector<vector<int>> dp(t.size() + 1, vector<int>(s.size() + 1));

            // empty string is contained in every offset at s
            for (int j = 0; j < dp[0].size(); ++j)
                dp[0][j] = 1;

            for (int i = 1; i < dp.size(); ++i)
                for (int j = 1; j < dp[0].size(); ++j)
                {
                    dp[i][j] += dp[i][j - 1];

                    if (t[t.size() - i] == s[s.size() - j])
                        dp[i][j] += dp[i - 1][j - 1];
                }

            //printVector(dp);
            return dp.back().back();
        }

        void test()
        {
            VERIFY(3 == numDistinct("rabbbit", "rabbit"));
            VERIFY(1 == numDistinct("ABCDE", "ACE"));
            VERIFY(0 == numDistinct("ABCDE", "AEC"));
            VERIFY(1 == numDistinct("", ""));
            VERIFY(3 == numDistinct("ccc", "c"));
        }
    };

    /* Populating Next Right Pointers in Each Node II */
    // LeetCode#117
    // Given a binary tree (of any type), populate each next pointer to point to its next right node. 
    // If there is no next right node, the next pointer should be set to NULL. Initially, all next 
    // pointers are set to NULL.
    // Note:
    //  You may only use constant extra space.
    struct p117
    {
        struct TreeLinkNode;
        using PTreeLinkNode = shared_ptr<TreeLinkNode>;
        struct TreeLinkNode
        {
            int data;

            PTreeLinkNode left;
            PTreeLinkNode right;
            PTreeLinkNode next;
        };

        PTreeLinkNode connect(PTreeLinkNode root)
        {
            if (!root)
                return root;

            PTreeLinkNode p;
            for (PTreeLinkNode n = root->next; n && !p; n = n->next)
                p = n->left ? n->left : n->right;

            if (root->left)
                root->left->next = root->right ? root->right : p;

            if (root->right)
                root->right->next = p;

            // Note, right subtree needs to be processed first to have the parent's next link
            // properly filled beforehand.
            connect(root->right);
            connect(root->left);

            return root;
        }

        string convert(PTreeLinkNode r, bool includeNext = false)
        {
            queue<PTreeLinkNode> q;
            q.push(r);

            string s;
            while (!q.empty())
            {
                PTreeLinkNode p = q.front();
                q.pop();

                if (!s.empty())
                    s += " ";

                if (p)
                {
                    s += to_string(p->data);

                    if (includeNext)
                    {
                        s += "->";
                        s += p->next ? to_string(p->next->data) : "#";
                    }

                    q.push(p->left);
                    q.push(p->right);
                }
                else
                {
                    s += " #";
                }
            }

            return s;
        }

        PTreeLinkNode convertHelper(const vector<string>& d, int i)
        {
            PTreeLinkNode p;

            if (i >= d.size())
                return p;

            string s = d[i];
            if (s != "#")
            {
                p = make_shared<TreeLinkNode>();
                p->data = atoi(s.c_str());

                p->left = convertHelper(d, 2 * i + 1);
                p->right = convertHelper(d, 2 * i + 2);
            }

            return p;
        }

        PTreeLinkNode convert(const string& s)
        {
            vector<string> d = vector<string>(
                istream_iterator<string>(istringstream(s)),
                istream_iterator<string>());

            return convertHelper(d, 0);
        }

        void test()
        {
            //string i = "1 2 4 # # # 3 # 5";
            //auto p = convert(i);
            //auto o = convert(p, false);
            //VERIFY(i == o);

            //connect(p);
            //auto co = convert(p, true);
            //LOG(co);

            //auto p = convert("1 2 3 4 5 # 7");
            //LOG(convert(p));
            //LOG(convert(connect(p), true));

            auto p = convert("2 1 3 0 7 9 1 2 # 1 0 # # 8 8 # # # # # 7");
            //LOG(convert(p));
            LOG(convert(connect(p), true));
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
            // r.first  path sum ending at root
            // r.second path sum not necessarily ending at root
            pair<int, int> r = { p->val, p->val };

            if (!p->left && !p->right)
                return r;

            if (!p->left || !p->right)
            {
                auto r1 = getMaximumPathSumHelper(p->left ? p->left : p->right);

                r.first = max(p->val, p->val + r1.first);
                r.second = max(p->val + r1.first, r1.second);
            }
            else
            {
                auto r1 = getMaximumPathSumHelper(p->left);
                auto r2 = getMaximumPathSumHelper(p->right);

                r.first = max(p->val, p->val + max(r1.first, r2.first));
                r.second = max(p->val + r1.first + r2.first, max(r1.second, r2.second));
            }

            r.second = max(r.first, r.second);
            return r;
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

    /* Palindrome Partitioning II */
    // LeetCode#132
    // Given a string s, partition s such that every substring of the partition is a palindrome. 
    // Return the minimum cuts needed for a palindrome partitioning of s.
    // For example, given s = "aab",
    // Return 1 since the palindrome partitioning ["aa","b"] could be produced using 1 cut.
    struct p132
    {
        int minCut(const string& s)
        {
            // dp[i][j] indicates s[i..j] is a palindrome 
            vector<vector<bool>> dp(s.size(), vector<bool>(s.size()));

            // m[i] min-cuts needed
            vector<int> m(s.size(), INT_MAX);

            for (int i = 0; i < dp.size(); ++i)
                for (int j = i; j >= 0; --j)
                {
                    if (i == j)
                        dp[i][j] = true;
                    else if ((i - j) == 1)
                        dp[i][j] = s[i] == s[j];
                    else
                        dp[i][j] = (s[i] == s[j]) && dp[i - 1][j + 1];

                    // determine min-cut
                    if (dp[i][j])
                        m[i] = (j == 0) ? 0 : min(m[i], m[j - 1] + 1);
                }

            return m.back();
        }

        void test()
        {
            VERIFY(1 == minCut("aab"));
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

    /* Word Break II */
    struct p140
    {
        vector<string> wordBreak(string s, unordered_set<string>& wordDict)
        {
            // dp[i][j] whether s[i..j] is a dictionary word
            vector<vector<bool>> dp(s.size(), vector<bool>(s.size()));

            // p[i] list of indexes with valid dictionary words ending at i
            vector<vector<int>> p(s.size());

            for (int j = 0; j < dp[0].size(); ++j)
                for (int i = j; i >= 0; --i)
                {
                    dp[i][j] = wordDict.find(s.substr(i, j - i + 1)) != wordDict.end();
                    if (dp[i][j])
                        p[j].push_back(i);
                }

            vector<string> r;
            queue<pair<int, string>> q;
            q.push(make_pair((int)s.size() - 1, ""));
            while (!q.empty())
            {
                auto x = q.front();
                q.pop();

                for (auto& i : p[x.first])
                {
                    string suffix = x.second;
                    if (!suffix.empty())
                        suffix = " " + suffix;

                    if (i == 0)
                        r.push_back(s.substr(i, x.first + 1) + suffix);
                    else
                        q.push(make_pair(i - 1, s.substr(i, x.first - i + 1) + suffix));
                }
            }

            printVector(r);
            return r;
        }

        void test()
        {
            VERIFY(vector<string>({ "cats and dog", "cat sand dog" }) ==
                wordBreak("catsanddog", unordered_set<string>({ "cat", "cats", "and", "sand", "dog" })));
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

    static void run()
    {
        p140().test();
    }

    //REGISTER_RUNNABLE(lcd3)
}
