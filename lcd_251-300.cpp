#include "stdafx.h"

namespace lcd6
{
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

    /* Find the Duplicate Number */
    // LeetCode#287
    // Given an array nums containing n + 1 integers where each integer is between 1 and n 
    // (inclusive), prove that at least one duplicate number must exist. Assume that there is only
    // one duplicate number, find the duplicate one.
    struct p287
    {
        int findDuplicate(const vector<int>& n)
        {
            int sp = 0;
            int fp = 0;
            
            // Get into the loop. Since the array doesn't contain 0, any swap loop that contains
            // the number sitting at 0'th position MUST contain the deplicate.
            do
            {
                sp = n[sp];
                fp = n[n[fp]];
            } while (sp != fp);

            // Let's reset and rerun the pointers but single step until they are both pointing
            // at the entry point of the cycle, hence a pair of duplicates.
            sp = 0;
            do
            {
                sp = n[sp];
                fp = n[fp];
            } while (sp != fp);

            return sp;
        }

        void test()
        {
            VERIFY(4 == findDuplicate({ 4, 1, 3, 2, 4 }));
            VERIFY(3 == findDuplicate({ 4, 1, 3, 2, 3 }));
            VERIFY(1 == findDuplicate({ 1, 1, 3, 2, 4 }));
            VERIFY(2 == findDuplicate({ 1, 3, 4, 2, 2 }));
            VERIFY(1 == findDuplicate({ 1, 3, 4, 2, 1 }));
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

    static void run()
    {
        p287().test();
    }

    //REGISTER_RUNNABLE(lcd6)
}
