#include "stdafx.h"

#define FREQ_IDX(b, i) ((b) << 1) | (i)

mutex s_loggerLock;
int s_maxLogLevel = 0;

#define TLOG(x) \
    do { \
        lock_guard<mutex> lock(s_loggerLock); \
        LOG( \
            getTimestamp() \
            << " " \
            << x \
            << " (" << __FUNCTION__ << "@" <<__LINE__ << ")"); \
    } while (0) 

#define TLOG1(x) \
    do { \
        if (s_maxLogLevel >= 1) \
            TLOG(x); \
    } while (0)

#define ASSERT(x) do { \
        if (!x) \
            TLOG("!!! assert FAILED (" ## #x ## ")"); \
    } while (0)

namespace oth1
{
    // Get binary representation string for a given double
    struct p01
    {
        string getBinString(double d)
        {
            if (d < numeric_limits<double>::epsilon())
                return "0";

            const int max_digits = 32;

            string s;
            int n = (int)d;
            d -= n;
            while ((n > 0) && (s.size() < max_digits))
            {
                s = ((n % 2) > 0 ? "1" : "0") + s;
                n /= 2;
            }

            if (d > numeric_limits<double>::epsilon())
                s += ".";

            while ((d > numeric_limits<double>::epsilon()) && (s.size() < max_digits))
            {
                d *= 2;
                s += (d >= 1) ? "1" : "0";
                d -= (int)d;
            }

            if (d > numeric_limits<double>::epsilon())
                s = "ERROR";

            return s;
        }

        void test()
        {
            VERIFY("0" == getBinString(0));
            VERIFY("1" == getBinString(1));
            VERIFY("1101" == getBinString(13));
            VERIFY("11.01" == getBinString(3.25));
        }
    };

    // Implement Huffman endcoding.
    struct p02
    {
        struct symnode;
        using psymnode = shared_ptr<symnode>;

        struct symnode
        {
            string sym;
            int freq;
            psymnode left;
            psymnode right;
        };

        void buildhenc(unordered_map<string, string>& result, const psymnode& root, string encoding)
        {
            if (!root->sym.empty())
            {
                result[root->sym] = encoding;
                return;
            }

            buildhenc(result, root->left, encoding + "0");
            buildhenc(result, root->right, encoding + "1");
        }

        unordered_map<string, string> gethenc(unordered_map<string, int> syms)
        {
            // comparator for psymnode to build a min-heap
            struct psymnode_comparator
            {
                bool operator()(psymnode& s1, psymnode& s2)
                {
                    return s2->freq < s1->freq;
                }
            };

            // symbol frequency table
            priority_queue<psymnode, vector<psymnode>, psymnode_comparator> pq;
            for (auto& p : syms)
                pq.emplace(new symnode{ p.first, p.second, nullptr, nullptr });

            // build the encoding tree
            while (pq.size() > 1)
            {
                auto n1 = pq.top();
                pq.pop();

                auto n2 = pq.top();
                pq.pop();

                psymnode p(new symnode{ "", n1->freq + n2->freq });

                // assign left-node based on symbol/frequency
                if (n1->sym.empty() || n2->sym.empty())
                    p->left = !n1->sym.empty() ? n1 : n2;
                else
                    p->left = n1->freq > n2->freq ? n1 : n2;

                p->right = p->left == n1 ? n2 : n1;
                pq.push(p);
            }

            // assign symbols
            unordered_map<string, string> result;
            buildhenc(result, pq.top(), "");

            return result;
        }

        void test()
        {
            unordered_map<string, int> input = {
                { "a1", 8 },
                { "a2", 7 },
                { "a3", 4 },
                { "a4", 1 },
            };

            auto r = gethenc(input);
            VERIFY(r.size() == input.size());
            VERIFY("0" == r["a1"]);
            VERIFY("10" == r["a2"]);
            VERIFY("110" == r["a3"]);
            VERIFY("111" == r["a4"]);
        }
    };

    // Implement RLE
    struct p03
    {
        class RLEEncoder
        {
        public:
            RLEEncoder() :
                m_lastChar('\0'),
                m_lastCharCount(0),
                m_lastCharPosition(-1)
            {
            }

            const string& get() const
            {
                return m_encodedValue;
            }

            void append(char c)
            {
                if (!c)
                    return;

                if ((m_lastCharCount == 0) || (m_lastChar != c))
                {
                    // move the "cursor" to the end to make the current encoded value permanent, otherwise
                    // we already rollback and update the latest count.
                    m_lastCharPosition = m_encodedValue.size() - 1;

                    m_lastChar = c;
                    m_lastCharCount = 0;
                }

                m_lastCharCount += 1;

                // trim and update encoded value
                m_encodedValue.resize(m_lastCharPosition + 1);
                m_encodedValue += to_string(m_lastCharCount);
                m_encodedValue.push_back(m_lastChar);
            }

        //protected:
            void clear()
            {
                m_lastCharCount = 0;
                m_lastCharPosition = -1;
                m_encodedValue.clear();
            }

        private:
            char m_lastChar;
            int m_lastCharCount;
            int m_lastCharPosition;

            string m_encodedValue;
        };

        void test()
        {
            RLEEncoder e;
            for (auto& c : "WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW")
                e.append(c);

            VERIFY(e.get() == "12W1B12W3B24W1B14W");

            e.append('W');
            VERIFY(e.get() == "12W1B12W3B24W1B15W");

            e.append('B');
            VERIFY(e.get() == "12W1B12W3B24W1B15W1B");

            e.clear();
            e.append('W');
            VERIFY(e.get() == "1W");
        }
    };

    // Implement Blocking Queue
    struct p04
    {
        template<typename T>
        class BlockingQueue
        {
        public:
            BlockingQueue(int capacity = 3) :
                m_capacity(capacity),
                m_abort(false)
            {
            }

            void enqueue(T item)
            {
                unique_lock<mutex> lock(m_lock);
                while (m_queue.size() == m_capacity)
                {
                    if (m_abort)
                        break;

                    LOG("q full, blocking.");
                    m_cv.wait(lock);
                }

                m_queue.push(item);
                m_cv.notify_all();
            }

            T deque()
            {
                unique_lock<mutex> lock(m_lock);
                while (m_queue.empty())
                {
                    if (m_abort)
                        break;

                    LOG("q empty, blocking.");
                    m_cv.wait(lock);
                }

                T item = m_queue.front();
                m_queue.pop();

                m_cv.notify_all();
                return item;
            }

            void abort()
            {
                m_abort = true;
                m_cv.notify_all();
            }

        private:
            mutex m_lock;
            condition_variable m_cv;

            int m_capacity;
            queue<T> m_queue;

            bool m_abort;
        };

        void test()
        {
            BlockingQueue<int> bq;

            bool abort = false;
            thread r([&]() {
                while (!abort)
                {
                    LOG("r:: dq");
                    int item = bq.deque();
                    LOG("r:: dq << " << item);

                    int sleepDuration = rand() % 10;
                    this_thread::sleep_for(sleepDuration * 100ms);
                }

                bq.abort();
            });

            thread w([&]() {
                while (!abort)
                {
                    int item = rand() % 100;
                    LOG("w:: eq >> " << item);
                    bq.enqueue(item);
                    LOG("w:: eq complete");

                    int sleepDuration = rand() % 10;
                    this_thread::sleep_for(sleepDuration * 10ms);
                }

                bq.abort();
            });

            this_thread::sleep_for(1000 * 10ms);
            abort = true;

            r.join();
            w.join();
        }
    };

    // Implement Threading
    struct p05
    {
        struct Data
        {
            bool created = true;
            int count = 0;
        };

        struct ThreadStat
        {
            unsigned long loopcount = 0;
            unordered_map<int, int> freq;
        };

        static void worker(Data* pd, ThreadStat* pt)
        {
            while (true)
            {
                if (pt->loopcount > 1e3)
                    break;

                pt->loopcount += 1;
                pt->freq[FREQ_IDX(pd->created, pd->count)] += 1;

                this_thread::sleep_for(1ms);

                if (!pd->created)
                {
                    pd->count = 1;
                    pd->created = true;
                }
                else
                {
                    pd->count = 0;
                    pd->created = false;
                }
            }
        }

        void test()
        {
            Data d;
            ThreadStat ts1;
            std::thread t1(worker, &d, &ts1);

            ThreadStat ts2;
            std::thread t2(worker, &d, &ts2);

            t1.join();
            t2.join();
        }
    };

    // 0-1 Knapsack
    struct p06
    {
        int solveKnapsack01(vector<int> W, vector<int> C, int w)
        {
            vector<vector<int>> m = vector<vector<int>>(W.size() + 1, vector<int>(w + 1));

            for (int i = 0; i < m.size(); ++i)
                m[i][0] = 0;

            for (int j = 0; j < m[0].size(); ++j)
                m[0][j] = 0;

            for (int i = 1; i < m.size(); ++i)
                for (int j = 1; j < m[0].size(); ++j)
                {
                    m[i][j] = m[i - 1][j];

                    int wp = j - W[i - 1];
                    if (wp >= 0)
                        m[i][j] = max(m[i][j], m[i - 1][wp] + W[i - 1] * C[i - 1]);
                }

            printVector(m);
            return m.back().back();
        }

        int solveKnapsack01_2(vector<int> W, vector<int> C, int w)
        {
            vector<vector<int>> m = vector<vector<int>>(W.size(), vector<int>(w + 1));

            for (int i = 0; i < m.size(); ++i)
                m[i][0] = 0;

            for (int j = 0; j < m[0].size(); ++j)
                m[0][j] = W[0] == j ? W[0] * C[0] : 0;

            for (int i = 1; i < m.size(); ++i)
                for (int j = 1; j < m[0].size(); ++j)
                    m[i][j] = max(
                        m[i - 1][j],
                        j - W[i] >= 0 ? m[i - 1][j - W[i]] + W[i] * C[i] : 0);

            printVector(m);
            return m.back().back();
        }

        int solveKnapsack01_3(vector<int> W, vector<int> C, int w)
        {
            vector<vector<int>> m = vector<vector<int>>(W.size() + 1, vector<int>(w + 1));

            for (int i = 0; i < m.size(); ++i)
                for (int j = 0; j < m[0].size(); ++j)
                    if ((i == 0) || (j == 0))
                        m[i][j] = 0;
                    else if ((j - W[i - 1]) >= 0)
                        m[i][j] = max(m[i - 1][j], m[i - 1][j - W[i - 1]] + W[i - 1] * C[i - 1]);
                    else
                        m[i][j] = m[i - 1][j];

                    printVector(m);
                    return m.back().back();
        }

        int solveKnapsack01_4(vector<int> W, vector<int> C, int w)
        {
            vector<int> m = vector<int>(w + 1, 0);
            vector<int> p = vector<int>(w + 1, 0);

            m[0] = 0;
            for (int j = 1; j < m.size(); ++j)
            {
                int q = 0;
                for (int i = 0; i < W.size(); ++i)
                {
                    if (j >= W[i])
                    {
                        int v = m[j - W[i]] + W[i] * C[i];
                        if (v > q)
                        {
                            q = v;
                            p[j] = i;
                        }
                    }
                }

                m[j] = q;

                printVector(p, 3);
                printVector(m, 3);
            }

            return m.back();
        }

        void test()
        {
            //int r1 = solveKnapsack01(W, C, w);
            //cout << r1 << endl;

            //int r2 = solveKnapsack01_2(W, C, w);
            //cout << r2 << endl;

            //int r3 = solveKnapsack01_3(W, C, w);
            //cout << r3 << endl;

            //int r4 = solveKnapsack01_4(W, C, w);
            //cout << r4 << endl;
        }
    };

    // Implement Circular Queue
    struct p07
    {
        class CircularQueue
        {
        public:
            CircularQueue(int capacity = 10) :
                m_rp(0),
                m_wp(0),
                m_hasItem(false)
            {
                if (capacity < 1)
                    capacity = 10;

                m_store.resize(capacity);
                m_capacity = capacity;
            }

            void push(int data)
            {
                _throwIfFull();

                m_store[m_wp] = data;
                m_wp = (m_wp + 1) % m_capacity;
                m_hasItem = true;
            }

            void pop()
            {
                _throwIfEmpty();

                m_rp = (m_rp + 1) % m_capacity;
                m_hasItem = m_rp != m_wp;
            }

            int top()
            {
                _throwIfEmpty();

                return m_store[m_rp];
            }

            size_t size()
            {
                return m_store.size();
            }

        private:
            void _throwIfEmpty()
            {
                if ((m_rp == m_wp) && !m_hasItem)
                    throw underflow_error("Underflow");
            }

            void _throwIfFull()
            {
                if ((m_rp == m_wp) && m_hasItem)
                    throw overflow_error("Overflow");
            }

            vector<int> m_store;
            int m_capacity;
            int m_rp;
            int m_wp;
            bool m_hasItem;
        };

        void test()
        {
            CircularQueue cq(5);

            cq.push(1);
            VERIFY(cq.top() == 1);

            cq.pop();

            try
            {
                cq.pop();
            }
            catch (underflow_error)
            {
                // underflow
            }

            for (int i = 0; i < 5; i++)
                cq.push(i);

            try
            {
                cq.push(5);
            }
            catch (overflow_error)
            {
                // overflow
            }

            cq.top();
            VERIFY(cq.top() == 0);

            cq.pop();
            VERIFY(cq.top() == 1);
        }
    };

    // Implement Rabin-Karp
    struct p10
    {
        template<typename T = int, typename V = unsigned char>
        class RollingHasher
        {
        public:
            const T c_radix = 256;
            const T c_modValue = 1021;

            RollingHasher() :
                m_hash(0),
                m_maxFactor(1)
            {
            }

            int push(V val)
            {
                if (m_hash > 0)
                {
                    m_maxFactor *= c_radix;
                    m_maxFactor %= c_modValue;
                }

                _add(val);
                return m_hash;
            }

            T roll(V val, V first)
            {
                T x = (first % c_modValue) * m_maxFactor;
                x %= c_modValue;

                m_hash -= x;
                m_hash = m_hash < 0 ? m_hash + c_modValue : m_hash;

                _add(val);
                return m_hash;
            }

            T getHash()
            {
                return m_hash;
            }

            void reset()
            {
                m_hash = 0;
                m_maxFactor = 1;
            }

            bool operator==(const RollingHasher& other)
            {
                return m_hash == other.m_hash;
            }

        private:
            void _add(V val)
            {
                m_hash *= c_radix;
                m_hash %= c_modValue;

                m_hash += val % c_modValue;
                m_hash %= c_modValue;
            }

            T m_hash;
            T m_maxFactor;
        };

        class RollingStringHasher : public RollingHasher<>
        {
        public:
            RollingStringHasher(int windowSize) :
                m_windowSize(windowSize)
            {
            }

            int push(unsigned char c)
            {
                if (m_value.size() < m_windowSize)
                {
                    __super::push(c);
                }
                else
                {
                    int fc = m_value.front();
                    m_value = m_value.substr(1);

                    __super::roll(c, fc);
                }

                m_value.push_back(c);
                return getHash();
            }

            int push(const string& s)
            {
                for (int i = 0; i < s.size(); ++i)
                    push(s[i]);

                return getHash();
            }

        private:
            int m_windowSize;
            string m_value;
        };

        // Rabin-Karp
        int findString(const string& s, const string& p)
        {
            int ls = s.size();
            int lp = p.size();

            if (ls < lp)
                return -1;

            RollingStringHasher rhp(lp);
            rhp.push(p);

            RollingStringHasher rhs(lp);
            int spuriousHits = 0;
            for (int i = 0; i < ls; ++i)
            {
                rhs.push(s[i]);

                if (rhp == rhs)
                {
                    ++spuriousHits;

                    int j = i - lp + 1;
                    if (j < 0)
                        continue;

                    int k = j;
                    while ((k <= i) && (s[k] == p[k - j]))
                        ++k;

                    if (k <= i)
                        continue;

                    return j;
                }
            }

            return -1;
        }

        void test()
        {
            //VERIFY(-1 == findString("try find the match", "findnot"));
            //VERIFY(-1 == findString("try", "findnot"));
            //VERIFY(0 == findString("try find the match", "tr"));
            //VERIFY(4 == findString("try find the match", "find"));
            //VERIFY(4 == findString("try find the match", "find the mat"));
            //VERIFY(4 == findString("try find the match", "find the match"));

            string s = "\
                It was the best of times, it was the worst of times, it was the age of wisdom, it was the age of \
                foolishness, it was the epoch of belief, it was the epoch of incredulity, it was the season of \
                Light, it was the season of Darkness, it was the spring of hope, it was the winter of despair, we \
                had everything before us, we had nothing before us, we were all going direct to Heaven, we were \
                all going direct the other way — in short, the period was so far like the present period, that \
                some of its noisiest authorities insisted on its being received, for good or for evil, in the \
                superlative degree of comparison only.";
            string p = "superlative degree";
            VERIFY(s.find(p) == findString(s, p));

            //string p = "find the";
            //RollingStringHasher rhp(p.size());
            //rhp.push(p);

            //LOG(p);
            //LOG(rhp.getHash());

            //string s = "try find the match";
            //RollingStringHasher rhs(p.size());
            //for (int i = 0; i < s.size(); ++i)
            //{
            //    rhs.push(s[i]);

            //    LOG(s.substr(i));
            //    LOG(rhs.getHash());
            //}
        }
    };

    // Partitioning (Lomuto vs. Hoare)
    struct p11
    {
        // Lomuto
        int partition1(vector<int>& v, int lo, int hi)
        {
            int wp = lo;
            while (lo < hi)
            {
                if (v[lo] <= v[hi])
                    swap(v[wp++], v[lo]);

                ++lo;
            }

            swap(v[wp], v[hi]);
            return wp;
        }

        // ~Hoare
        int partition2(vector<int>& v, int lo, int hi)
        {
            int p = hi;
            while (lo < hi)
            {
                while ((v[lo] < v[p]) && (lo < hi))
                    ++lo;

                while ((v[hi] >= v[p]) && (hi > lo))
                    --hi;

                if (lo == hi)
                    break;

                swap(v[lo], v[hi]);
            }

            swap(v[lo], v[p]);
            return lo;
        }

        template<typename T>
        bool validatePartitioning(vector<int> v, const T& f)
        {
            int p = (this->*f)(v, 0, v.size() - 1);

            if ((p < 0) || (p >= v.size()))
                return false;

            for (int i = 0; i < v.size(); ++i)
            {
                if ((i < p) && (v[i] > v[p]))
                    return false;

                if ((i > p) && (v[i] < v[p]))
                    return false;
            }

            return true;
        }

        void test()
        {
            vector<int> v(10);
            iota(v.begin(), v.end(), 0);
            random_shuffle(v.begin(), v.end());

            for (auto f : vector<decltype(&p11::partition1)>{ &p11::partition1, &p11::partition2 })
            {
                VERIFY(validatePartitioning(v, f));
                VERIFY(validatePartitioning({ 0 }, f));
                VERIFY(validatePartitioning({ 0, 1 }, f));
                VERIFY(validatePartitioning({ 1, 0 }, f));
                VERIFY(validatePartitioning({ 1, 1 }, f));
                VERIFY(validatePartitioning({ 0, 1, 1 }, f));

                for (int i = 0; i < 10; ++i)
                    VERIFY(validatePartitioning(getRandom(0, 100, getRandom(1, 10)), f));
            }
        }
    };

    // RPN
    struct p12
    {
        vector<char> tokenize(const string& exp)
        {
            vector<char> r;
            for (auto c : exp)
            {
                if (c != ' ')
                    r.push_back(c);
            }

            return r;
        }

        string convertToRPN(const string& infixExpression)
        {
            static string op = "(*/+-";

            stack<char> st;
            string r;
            for (auto t : tokenize(infixExpression))
            {
                if (t == ')')
                {
                    while (st.top() != '(')
                    {
                        r.push_back(st.top());
                        r.push_back(' ');
                        st.pop();
                    }

                    st.pop();
                }
                else if (op.find_first_of(t) != string::npos)
                {
                    if (!st.empty() && (st.top() != '(') && (op.find_first_of(st.top()) < op.find_first_of(t)))
                    {
                        r.push_back(st.top());
                        r.push_back(' ');
                        st.pop();
                    }

                    st.push(t);
                }
                else
                {
                    r.push_back(t);
                    r.push_back(' ');
                }
            }

            while (!st.empty())
            {
                r.push_back(st.top());
                r.push_back(' ');
                st.pop();
            }

            return r;
        }

        int eval(const string& exp)
        {
            static string op = "*/+-";

            string s = convertToRPN(exp);

            stack<int> st;
            for (int i = 0; i < s.size(); ++i)
            {
                if (s[i] == ' ')
                    continue;

                if (op.find_first_of(s[i]) == string::npos)
                {
                    st.push(s[i] - '0');
                    continue;
                }

                int op2 = st.top();
                st.pop();
                int op1 = st.top();
                st.pop();

                switch (s[i])
                {
                case '+':
                    st.push(op1 + op2);
                    break;

                case '-':
                    st.push(op1 - op2);
                    break;

                case '*':
                    st.push(op1 * op2);
                    break;

                case '/':
                    st.push(op1 / op2);
                    break;
                }
            }

            assert(st.size() == 1);
            return st.top();
        }

        void test()
        {
            VERIFY(19 == eval("4 * ( 3 + 2 ) / (6 - 4) + 9"));
        }
    };

    // Implement Reader-Writer Lock
    namespace p13
    {
        //
        // First readers-writers problem (readers-preference)
        //

        // Using two mutexes and a condition variable, weak priority to readers
        class RWLock1
        {
        public:
            RWLock1() :
                nreaders(0)
            {
            }

            void EnterRead()
            {
                TLOG1("enter read");

                lock_guard<mutex> lock(m_rlock);
                ++nreaders;
                TLOG1("acquired rlock, nreaders=" << nreaders);

                if (nreaders == 1)
                {
                    lock_guard<mutex> lock(m_wlock);
                    TLOG1("first reader, acquired wlock once");
                }
            }

            void LeaveRead()
            {
                TLOG1("leave read");

                lock_guard<mutex> lock(m_rlock);
                --nreaders;
                TLOG1("acquired rlock, nreaders=" << nreaders);

                if (nreaders == 0)
                {
                    TLOG1("last reader, signal waiting writer (if any)");
                    m_cv.notify_one();
                }
            }

            void EnterWrite()
            {
                TLOG1("enter write");

                unique_lock<mutex> lock(m_rlock);
                while (nreaders > 0)
                {
                    TLOG1("waiting on readers");
                    m_cv.wait(lock);
                }

                m_wlock.lock();
                TLOG1("acquired wlock");

                ASSERT(nreaders == 0);
            }

            void LeaveWrite()
            {
                TLOG1("leave write");

                m_wlock.unlock();
                TLOG1("released wlock");
            }

        private:
            mutex m_rlock;
            mutex m_wlock;
            condition_variable m_cv;

            int nreaders;
        };

        //
        // Second reader-writers probelm (writers-preference)
        //

        // Using two mutexes, strong priority to writers
        class RWLock2a
        {
        public:
            RWLock2a() :
                nreaders(0)
            {
            }

            void EnterRead()
            {
                TLOG1("entering read");

                TLOG1("queue behind writers (if any)");
                lock_guard<mutex> wlock(m_wlock);

                lock_guard<mutex> rlock(m_rlock);
                ++nreaders;
                TLOG1("acquired rlock, nreaders=" << nreaders);

                TLOG1("entered read");
            }

            void LeaveRead()
            {
                TLOG1("leaving read");

                lock_guard<mutex> lock(m_rlock);
                --nreaders;
                TLOG1("acquired rlock, nreaders=" << nreaders);

                if (nreaders == 0)
                {
                    TLOG1("last reader, signal waiting writer (if any)");
                    m_cv.notify_one();
                }

                TLOG1("left read");
            }

            void EnterWrite()
            {
                TLOG1("entering write");
                m_wlock.lock();

                unique_lock<mutex> lock(m_rlock);
                while (nreaders > 0)
                {
                    TLOG1("waiting on readers");
                    m_cv.wait(lock);
                }

                TLOG1("entered write");
            }

            void LeaveWrite()
            {
                TLOG1("leaving write");
                m_wlock.unlock();
                TLOG1("released wlock");

                TLOG1("left write");
            }

        private:
            mutex m_rlock;
            mutex m_wlock;
            condition_variable m_cv;

            int nreaders;
        };

        // Using a single mutex and condition variable
        class RWLock2b
        {
        public:
            RWLock2b() :
                rp(0),
                rw(0),
                ww(0),
                wp(false)
            {
            }

            void EnterRead()
            {
                TLOG1("entering read");

                unique_lock<mutex> lock(m_lock);
                ++rw;
                TLOG1("acquired lock, " << _getState());

                while (_isWriteInProgress() || _isReaderFull() || _isWriterWaiting())
                {
                    TLOG1("waiting, " << _getState());
                    m_cv.wait(lock);
                }

                --rw;
                ++rp;
                TLOG1("entered read, " << _getState());
            }

            void LeaveRead()
            {
                TLOG1("leaving read");

                unique_lock<mutex> lock(m_lock);
                --rp;
                TLOG1("acquired lock, " << _getState());

                if (!_isReadInProgress() && (_isWriterWaiting() || _isReaderWaiting()))
                {
                    TLOG1("last reader, signal waiting writer (if any)");
                    m_cv.notify_all();
                }

                TLOG1("left read");
            }

            void EnterWrite()
            {
                TLOG1("entering write");
                unique_lock<mutex> lock(m_lock);
                ++ww;

                while (_isWriteInProgress() || _isReadInProgress())
                {
                    TLOG1("waiting, " << _getState());
                      m_cv.wait(lock);
                }

                --ww;
                wp = true;

                TLOG1("entered write, " << _getState());
            }

            void LeaveWrite()
            {
                TLOG1("leaving write");

                unique_lock<mutex> lock(m_lock);
                wp = false;

                if (_isWriterWaiting() || _isReaderWaiting())
                {
                    TLOG1("waiting, " << _getState());
                    m_cv.notify_all();
                }

                TLOG1("left write, " << _getState());
            }

        private:
            bool _isWriteInProgress() { return wp; }
            bool _isReadInProgress() { return rp > 0; }
            bool _isWriterWaiting() { return ww > 0; }
            bool _isReaderWaiting() { return rw > 0; }
            bool _isReaderFull() { return rp == rmax; }

            string _getState()
            {
                return "state [rw=" +
                    to_string(rw) +
                    ",rp=" +
                    to_string(rp) +
                    ",ww=" +
                    to_string(ww) +
                    ",wp=" +
                    to_string(wp) +
                    "]";
            }

            mutex m_lock;
            condition_variable m_cv;

            // read-in-progress
            int rp;
            // reader waiting
            int rw;

            // max readers
            int rmax = 3;

            // write-in-progress
            bool wp;
            // writer waiting
            int ww;
        };

        //
        // Third readers-writers problem (fair)
        //

        // Fair reader-writer lock
        class RWLock3
        {
        public:
            RWLock3() :
                rp(0),
                rw(0),
                ww(0),
                wp(false),
                lw(false)
            {
            }

            void EnterRead()
            {
                unique_lock<mutex> lock(m_lock);
                ++rw;

                while (_isWriteInProgress() || _isReaderFull())
                    m_cv.wait(lock);

                while (_isWriterWaiting() && !_isLastOwnerWriter())
                    m_cv.wait(lock);

                --rw;
                ++rp;
            }

            void LeaveRead()
            {
                unique_lock<mutex> lock(m_lock);
                --rp;

                lw = false;

                if (!_isReadInProgress() && (_isWriterWaiting() || _isReaderWaiting()))
                    m_cv.notify_all();
            }

            void EnterWrite()
            {
                unique_lock<mutex> lock(m_lock);
                ++ww;

                while (_isWriteInProgress() || _isReadInProgress())
                    m_cv.wait(lock);

                while (_isReaderWaiting() && _isLastOwnerWriter())
                    m_cv.wait(lock);

                --ww;
                wp = true;
            }

            void LeaveWrite()
            {
                unique_lock<mutex> lock(m_lock);
                wp = false;

                lw = true;

                if (_isWriterWaiting() || _isReaderWaiting())
                    m_cv.notify_all();
            }

        private:
            bool _isWriteInProgress() { return wp; }
            bool _isReadInProgress() { return rp > 0; }
            bool _isWriterWaiting() { return ww > 0; }
            bool _isReaderWaiting() { return rw > 0; }
            bool _isReaderFull() { return rp == rmax; }
            bool _isLastOwnerWriter() { return lw; }

            mutex m_lock;
            condition_variable m_cv;

            // read-in-progress
            int rp;
            // reader waiting
            int rw;

            // max readers
            int rmax = 3;

            // write-in-progress
            bool wp;
            // writer waiting
            int ww;

            // write resource user was a writer
            bool lw;
        };

        void demo()
        {
            auto simsec = 25ms;
            int Nsimsec = 50;
            int Nreaders = 5;
            int Nwriters = 2;
            string message = "His throne extends over the heavens and the earth";

            RWLock1 rwl;

            bool abort = false;

            vector<char> wb;
            int wp = 0;

            auto r = [&]() {
                vector<char> b;
                int i = 0;

                while (!abort)
                {
                    rwl.EnterRead();

                    int n = getRandom(1, 3);
                    while ((n-- > 0) && (i < wb.size()))
                    {
                        TLOG("reading " << wb[i]);

                        b.push_back(wb[i++]);
                        this_thread::sleep_for(simsec);
                    }

                    rwl.LeaveRead();

                    this_thread::sleep_for(simsec);
                }

                TLOG("read buffer: " << string(b.begin(), b.end()).c_str());
            };

            auto w = [&]() {
                while (!abort)
                {
                    rwl.EnterWrite();

                    int n = getRandom(1, 5);
                    while ((n-- > 0) && (wp < message.size()))
                    {
                        TLOG("writing " << message[wp]);

                        wb.push_back(message[wp++]);
                        this_thread::sleep_for(simsec);
                    }

                    rwl.LeaveWrite();

                    this_thread::sleep_for(simsec);
                }

                TLOG("write buffer: " << string(wb.begin(), wb.end()).c_str());
            };

            vector<future<void>> f;
            for (int i = 0; i < Nreaders; ++i)
                f.push_back(async(r));

            for (int i = 0; i < Nwriters; ++i)
                f.push_back(async(w));

            this_thread::sleep_for(simsec * Nsimsec);
            abort = true;

            for (auto& x : f)
                x.get();
        }
    }

    // Implement File-IO
    struct p14
    {
        void simpleFileWrite(const string& filename)
        {
            ofstream ofs(filename);

            ofs << "one" << ", " << "1" << endl;
            ofs << "two" << ", " << "2" << endl;
        }

        void simpleFileRead(const string& filename)
        {
            ifstream ifs(filename);

            vector<string> v;
            while (!ifs.eof())
            {
                string s;
                getline(ifs, s);

                v.push_back(s);
            }

            printVector(v);
        }

        void test()
        {
            string fn = "testfio1.txt";
            simpleFileWrite(fn);
            simpleFileRead(fn);
        }
    };

    // Implement LRU Cache
    namespace p15
    {
        class Foo
        {
        public:
            static int usageCount;
            static int usageCountMax;

            Foo(int i) :
                m_i(i),
                m_id(++usageCount)
            {
                LOG(" cFoo#" << m_id << "(" << m_i << ")");
                usageCountMax = max(usageCountMax, usageCount);
            }

            Foo(const Foo& other) :
                m_i(other.m_i),
                m_id(++usageCount)
            {
                LOG("ccFoo#" << m_id << "(" << m_i << ")");
            }

            Foo& operator=(const Foo& other)
            {
                if (this != &other)
                    m_i = other.m_i;

                LOG("caFoo#" << m_id << "(" << m_i << ")");
                return *this;
            }

            ~Foo()
            {
                LOG(" ~Foo#" << m_id << "(" << m_i << ")");
                usageCount--;
            }

            bool operator==(int i)
            {
                return m_i == i;
            }

        private:
            int m_i;
            int m_id;
        };

        int Foo::usageCount = 0;
        int Foo::usageCountMax = 0;

        template<typename K, typename V>
        class ILruCache
        {
        public:
            virtual void initialize(int capacity) = 0;
            virtual bool getItem(const K& key, V& item) = 0;
            virtual void setItem(const K& key, V item) = 0;
        };

        template<typename K, typename V>
        class LruCache1 : ILruCache<K, V>
        {
            typedef pair<K, V> CacheItem;
            typedef typename list<CacheItem>::const_iterator PCacheItem;

        public:
            void initialize(int capacity)
            {
                m_maxItems = capacity;
            }

            bool getItem(K key, V& item)
            {
                auto it = m_lookup.find(key);
                if (it != m_lookup.end())
                    item = it->second->second;

                return it != m_lookup.end();
            }

            void setItem(K key, V item)
            {
                auto it = m_lookup.find(key);
                if (it != m_lookup.end())
                {
                    m_store.erase(m_lookup[key]);
                    m_lookup.erase(key);
                }

                m_store.emplace_back(key, item);
                m_lookup[key] = --m_store.end();

                if (m_store.size() > m_maxItems)
                {
                    auto it = m_store.begin();
                    m_lookup.erase(it->first);

                    m_store.pop_front();
                }
            }

            //int size();
            //int getHitRate();
        private:
            list<CacheItem> m_store;
            unordered_map<K, PCacheItem> m_lookup;
            int m_maxItems;
        };

        template<typename K, typename V>
        class LruCache2 : public ILruCache<K, V>
        {
            typedef typename pair<K, V> CacheItem;
            typedef typename list<pair<K, V>>::iterator PCacheItem;

        public:
            void initialize(int maxCount)
            {
                m_maxCount = maxCount;
            }

            bool getItem(K key, V& value)
            {
                if (m_lookup.find(key) == m_lookup.end())
                    return false;

                value = m_lookup[key]->second;
                refreshItem(key);

                return true;
            }

            void setItem(K key, V value)
            {
                if (m_lookup.find(key) == m_lookup.end())
                {
                    m_store.emplace_front(key, value);
                    m_lookup[key] = m_store.begin();
                }
                else
                {
                    m_lookup[key]->second = value;
                    refreshItem(key);
                }

                if (m_store.size() > m_maxCount)
                {
                    m_lookup.erase(m_store.back().first);
                    m_store.pop_back();
                }
            }

        private:
            void refreshItem(K& key)
            {
                // assert(m_lookup.find(key) != m_lookup.end());
                auto it = m_lookup[key];
                if (it != m_store.begin())
                {
                    m_store.push_front(*it);
                    m_store.erase(it);

                    m_lookup[key] = m_store.begin();
                }
            }

            int m_maxCount;
            list<CacheItem> m_store;
            unordered_map<K, PCacheItem> m_lookup;
        };

        template<typename K, typename V>
        class LruCache : public ILruCache<K, V>
        {
        public:
            void initialize(int capacity) override
            {
                m_capacity = capacity;
            }

            bool getItem(const K& key, V& item) override
            {
                auto it = m_cache.find(key);
                if (it == m_cache.end())
                    return false;

                _updateLruList(key);

                item = it->second.first;
                return true;
            }

            void setItem(const K& key, V item) override
            {
                auto it = m_cache.find(key);
                if (it == m_cache.end())
                {
                    m_keys.push_front(key);
                    m_cache.emplace(key, make_pair(item, m_keys.begin()));
                }
                else
                {
                    it->second.first = item;
                    _updateLruList(key);
                }

                if (m_cache.size() > m_capacity)
                {
                    m_cache.erase(m_keys.back());
                    m_keys.pop_back();
                }
            }

        private:
            void _updateLruList(const K& key)
            {
                auto it = m_cache.find(key);

                m_keys.erase(it->second.second);
                m_keys.push_front(key);
                it->second.second = m_keys.begin();
            }

            unordered_map<K, pair<V, typename list<K>::iterator>> m_cache;
            list<K> m_keys;
            int m_capacity;
        };

        void testLruCache()
        {
            int i = 0;
            LruCache<string, int> c;
            ILruCache<string, int>* pc = &c;

            pc->initialize(3);

            pc->setItem("one", 1);

            bool found = pc->getItem("one", i);
            VERIFY(found);
            VERIFY(i == 1);

            pc->setItem("two", 2);
            pc->setItem("three", 3);
            pc->setItem("four", 4);

            found = pc->getItem("one", i);
            VERIFY(!found);

            pc->setItem("five", 5);
            // "two" should have been pushed out
            VERIFY(!pc->getItem("two", i));

            // refresh "three"
            pc->getItem("three", i);
            pc->setItem("six", 6);
            VERIFY(!pc->getItem("four", i));

            LOG("usageCount:" << Foo::usageCount << ", usageCountMax:" << Foo::usageCountMax);
        }

        void main()
        {
            testLruCache();
        }
    }

    // Implement Stack/Queue
    namespace p16
    {
        template<typename T>
        class IGenericQueue
        {
        public:
            virtual void push(T item) = 0;
            virtual T pop() = 0;
            virtual T& front() = 0;
            virtual T& back() = 0;
            virtual bool empty() const = 0;
        };

        template<typename T>
        class ICustomQueue : public IGenericQueue<T>
        {
        };

        template<typename T, int BucketSize = 16>
        class CustomQueue : public ICustomQueue<T>
        {
        public:
            void push(T item) override
            {
                if (m_items.empty() || m_items.back().size() == BucketSize)
                {
                    m_items.push_back(vector<T>());
                    m_items.back().reserve(BucketSize);
                }

                m_items.back().push_back(item);
            }

            T pop() override
            {
                assert(!m_items.empty());

                auto& v = m_items.front();

                T result = v.front();
                v.erase(v.begin());

                if (v.empty())
                    m_items.pop_front();

                return result;
            }

            T& front() override
            {
                vector<int> v;
                assert(!m_items.empty() && !m_items.front().empty());
                return m_items.front().front();
            }

            T& back() override
            {
                assert(!m_items.empty() && !m_items.back().empty());
                return m_items.back().back();
            }

            bool empty() const override
            {
                return m_items.empty();
            }

        protected:
            list<vector<T>>& getItems()
            {
                return m_items;
            }

        private:
            list<vector<T>> m_items;
        };

        template<typename T, int BucketSize = 16>
        class TestableCustomQueue : public CustomQueue<T, BucketSize>
        {
        public:
            const list<vector<T>>& getItemsView() const
            {
                return const_cast<TestableCustomQueue&>(*this).getItems();
            }
        };

        template<typename T>
        class IGenericStack2
        {
            virtual bool empty(int stackId) const = 0;
            virtual void push(int stackId, T item) = 0;
            virtual T pop(int stackId) = 0;
            virtual T top(int stackId) = 0;
        };

        template<typename T>
        class ICustomStack : public IGenericStack2<T>
        {
        public:
            virtual bool full(int stackId) const = 0;
        };

        template<typename T, int StackSize = 8>
        class CustomStack : public ICustomStack<T>
        {
        public:
            CustomStack(int numStacks)
            {
                m_items.resize(numStacks * StackSize);
                for (int i = 0; i < numStacks; ++i)
                    m_stackTops.push_back(_getMinIndex(i) - 1);
            }

            // IGenericStack

            bool empty(int stackId) const override
            {
                assert(_isExistingStackId(stackId));
                return _getStackTop(stackId) < _getMinIndex(stackId);
            }

            void push(int stackId, T item) override
            {
                assert(_getStackTop(stackId) <= _getMaxIndex(stackId));

                ++_getStackTop(stackId);
                _getItems()[_getStackTop(stackId)] = item;
            }

            T pop(int stackId) override
            {
                assert(_getStackTop(stackId) >= _getMinIndex(stackId));

                --_getStackTop(stackId);
                return _getItems()[_getStackTop(stackId) + 1];
            }

            T top(int stackId) override
            {
                assert(_getStackTop(stackId) >= _getMinIndex(stackId));

                return _getItems()[_getStackTop(stackId)];
            }

            // ICustomStack
            bool full(int stackId) const override
            {
                return _getStackTop(stackId) == _getMaxIndex(stackId);
            }

        private:
            inline vector<T>& _getItems()
            {
                return m_items;
            }

            inline int& _getStackTop(int stackId)
            {
                return const_cast<int&>(
                    const_cast<const CustomStack&>(*this)._getStackTop(stackId));
            }

            inline const int& _getStackTop(int stackId) const
            {
                assert(_isExistingStackId(stackId));
                return m_stackTops[stackId];
            }

            inline bool _isExistingStackId(int stackId) const
            {
                return (stackId >= 0) && (stackId < m_stackTops.size());
            }

            inline int _getMinIndex(int stackId) const
            {
                return stackId * StackSize;
            }

            inline int _getMaxIndex(int stackId) const
            {
                return (stackId + 1) * StackSize - 1;
            }

            vector<T> m_items;
            vector<int> m_stackTops;
        };

        template<typename T>
        class IGenericStack
        {
            virtual bool empty() const = 0;
            virtual void push(T item) = 0;
            virtual T pop() = 0;
            virtual T top() = 0;
        };

        class SetOfStacks : public IGenericStack<int>
        {
        public:
            SetOfStacks(int maxStackSize) :
                m_maxStackSize(maxStackSize)
            {
            }

            bool empty() const override
            {
                return m_stacks.empty();
            }

            void push(int item) override
            {
                if (m_stacks.empty() || (m_stacks.back().size() == m_maxStackSize))
                    m_stacks.push_back(vector<int>());

                m_stacks.back().push_back(item);
            }

            int pop() override
            {
                int value = -1;
                if (!m_stacks.empty())
                {
                    value = m_stacks.back().back();
                    m_stacks.back().pop_back();
                    if (m_stacks.back().empty())
                        m_stacks.pop_back();
                }

                return value;
            }

            int top() override
            {
                int value = -1;
                if (!m_stacks.empty())
                    value = m_stacks.back().back();

                return value;
            }

        private:
            int m_maxStackSize;
            vector<vector<int>> m_stacks;
        };

        class TowerOfHanoiSim
        {
        public:
            TowerOfHanoiSim(int nDisks) :
                m_towers(vector<vector<int>>(3, vector<int>()))
            {
                for (int i = nDisks; i > 0; --i)
                    m_towers[0].push_back(i);
            }

            int Simulate()
            {
                m_stepCount = 0;
                _moveDisks(m_towers[0].size(), 0, 2, 1);

                return m_stepCount;
            }

        private:
            void _moveDisks(int n, int st, int dt, int it)
            {
                if (n < 1)
                    return;

                _moveDisks(n - 1, st, it, dt);

                int d = m_towers[st].back();
                m_towers[st].pop_back();
                m_towers[dt].push_back(d);

                ++m_stepCount;
                //LOG("#" << m_stepCount << ". moved disk#" << d << " from tower#" << (st + 1) << " to tower#" << (dt + 1));

                _moveDisks(n - 1, it, dt, st);
            }

            int m_stepCount;
            vector<vector<int>> m_towers;
        };


        void test()
        {
            TestableCustomQueue<int> tq;
            assert(tq.empty());

            tq.push(1);
            assert(!tq.empty());

            tq.push(2);
            int i = tq.pop();
            assert(i = 1);
            i = tq.pop();
            assert(i = 2);
            assert(tq.empty());

            tq.push(3);
            assert(tq.front() == 3);
            assert(tq.back() == 3);

            tq.push(4);
            assert(tq.front() == 3);
            assert(tq.back() == 4);

            while (!tq.empty())
                tq.pop();

            assert(tq.getItemsView().empty());

            enum STACKID : int
            {
                STACK1,
                STACK2,
                STACK3,
                MAX
            };

            CustomStack<char> s(STACKID::MAX);

            s.push(STACKID::STACK1, 'a');
            assert(!s.empty(STACKID::STACK1));

            s.push(STACKID::STACK2, 'A');
            assert(s.top(STACKID::STACK2) == 'A');

            s.push(STACKID::STACK3, '1');
            s.push(STACKID::STACK3, '2');
            s.push(STACKID::STACK3, '3');
            assert(s.top(STACKID::STACK3) == '3');

            for (int i = 4; i <= 8; ++i)
                s.push(STACKID::STACK3, i);
            assert(s.full(STACKID::STACK3));

            SetOfStacks st(4);

            st.push(1);
            assert(!st.empty());
            int value = st.pop();
            assert(value == 1);
            assert(st.empty());

            for (int i = 0; i < 10; ++i)
                st.push(i);
            assert(st.top() == 9);

            int popCount = 0;
            while (!st.empty())
            {
                ++popCount;
                st.pop();
            }
            assert(popCount == 10);

            auto tic = chrono::system_clock::now();
            TowerOfHanoiSim th(16);
            LOG("#steps " << th.Simulate());
            auto toc = chrono::system_clock::now();

            LOG("took " << (chrono::duration_cast<chrono::milliseconds>(toc - tic).count()) << "ms");
        }

        void main()
        {
            test();
        }
    }

    // Implement Trie
    namespace p17
    {
        struct TrieNode
        {
            char value;
            unordered_map<char, shared_ptr<TrieNode>> children;
            bool isTerminal;
        };

        class Trie
        {
        public:
            Trie()
            {
                m_root = make_shared<TrieNode>();
                m_root->value = '$';
            }

            virtual void put(const string& s)
            {
                add(s, 0);
            }

            virtual bool contains(const string& s)
            {
                shared_ptr<TrieNode> n = m_root;
                int i = 0;
                while ((i < s.size()) && (n->children.find(s[i]) != n->children.end()))
                {
                    n = n->children[s[i++]];
                }

                return (i == s.size()) && n->isTerminal;
            }

            void reset()
            {
                m_root->children.clear();
            }

            void display()
            {
                vector<string> v;
                string s;
                displaybt(m_root, v, s);
                for (auto x : v)
                    LOG(x);
            }

        protected:
            void add(const string& s, int i)
            {
                shared_ptr<TrieNode> n = m_root;
                while (i < s.size())
                {
                    if (n->children.find(s[i]) == n->children.end())
                    {
                        n->children[s[i]] = make_shared<TrieNode>();
                        n->children[s[i]]->value = s[i];
                    }

                    n = n->children[s[i]];
                    ++i;
                }

                n->isTerminal = true;
            }

            void displaybt(shared_ptr<TrieNode> n, vector<string>& v, string& s, int level = 0)
            {
                s.push_back(n->value);

                if (n->isTerminal)
                    v.push_back(string(level, ' ') + s);

                for (auto p : n->children)
                    displaybt(p.second, v, s, level + 1);

                s.pop_back();
            }

            shared_ptr<TrieNode> m_root;
        };

        class SuffixTrie : public Trie
        {
        public:
            void put(const string& s) override
            {
                for (int i = 0; i < s.size(); ++i)
                    add(s, i);
            }

            bool contains(const string& s) override
            {
                shared_ptr<TrieNode> n = m_root;
                int i = 0;
                while ((i < s.size()) && (n->children.find(s[i]) != n->children.end()))
                {
                    n = n->children[s[i++]];
                }

                return i == s.size();
            }
        };

        static void demo()
        {
            Trie t;
            t.put("violet");
            t.put("indigo");
            t.put("blue");
            t.put("green");
            t.put("orange");
            t.put("yellow");
            t.put("red");

            VERIFY(!t.contains("re"));
            VERIFY(t.contains("red"));
            VERIFY(!t.contains("viol"));
            VERIFY(!t.contains("purple"));

            t.display();

            t.reset();
            t.put("us");
            t.put("use");
            t.put("uses");
            t.put("used");
            t.put("user");
            t.put("users");
            t.put("using");
            t.put("useful");
            t.put("username");
            t.put("utah");
            t.display();

            SuffixTrie st;
            st.put("banana");

            VERIFY(st.contains("ban"));
            VERIFY(st.contains("ana"));
            VERIFY(st.contains("na"));
            VERIFY(!st.contains("aba"));

            st.display();
        }
    }

    // Implement Dijkstra (Shortest Path)/Prim (Minimum Spanning Tree)
    struct p18
    {
        // Dijkstra vs. Prim
        //
        //Dijkstra(G, w, r)
        //  for each key u ∈ G.V
        //      u.key = ∞
        //      u.parent = NIL
        //  r.key = 0
        //  Q = G.V
        //  while (Q ≠ ø)
        //      u = Extract-Min(Q)
        //      for each v ∈ G.Adj[u]
        //          if (v ∈ Q) and w(u, v) + u.key < v.key
        //              v.parent = u
        //              v.key = w(u, v) + u.key <= = relax function, Pay attention here
        //
        //MST-PRIM(G, w, r)
        //  for each u ∈ G.V
        //      u.key = ∞
        //      u.parent = NIL
        //  r.key = 0
        //  Q = G.V
        //  while (Q ≠ ø)
        //      u = Extract-Min(Q)
        //      for each v ∈ G.Adj[u]
        //          if (v ∈ Q) and w(u, v) < v.key
        //              v.parent = u
        //              v.key = w(u, v) <= = relax function, Pay attention here

        vector<vector<int>> dijkstraSPT(const Graph<int>& g, int i)
        {
            // distance to source
            vector<int> d(g.size(), 999);
            // path to sourre
            vector<int> p(g.size(), -1);
            // state
            vector<bool> s(g.size(), false);

            auto pqc = [&d](auto& i, auto& j)
            {
                return d[i] > d[j];
            };
            priority_queue<int, vector<int>, decltype(pqc)> pq(pqc);

            d[i] = 0;
            pq.push(i);
            s[i] = true;

            while (!pq.empty())
            {
                int v = pq.top();
                pq.pop();

                LOG(v);

                for (auto e : g.getEdges(v))
                {
                    int n = e.first->id;
                    int w = e.second;

                    if ((d[v] + w) < d[n])
                    {
                        d[n] = d[v] + w;
                        p[n] = v;
                    }

                    if (!s[n])
                    {
                        s[n] = true;
                        pq.push(n);
                    }
                }

                printVector(d);
            }

            return{ d, p };
        }

        vector<int> primMST(Graph<int> g, int i)
        {
            // path
            vector<int> p(g.size(), -1);
            // state (in-tree)
            vector<bool> s(g.size(), false);
            // edge-weigth/vertex pair
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

            pq.emplace(0, i);
            s[i] = true;

            while (!pq.empty())
            {
                int v = pq.top().second;
                pq.pop();

                for (auto& e : g.getNode(v)->adjList)
                {
                    int n = e.first->id;
                    int w = e.second;
                    if (!s[n])
                    {
                        s[n] = true;
                        pq.emplace(w, n);
                        p[n] = v;
                    }
                }
            }

            printVector(p);
            return p;
        }

        void test()
        {
            Graph<int> g;

            // gph1a.jpg
            g.addEdge(0, 1, 7);
            g.addEdge(0, 2, 9);
            g.addEdge(0, 5, 14);
            g.addEdge(1, 2, 10);
            g.addEdge(1, 3, 15);
            g.addEdge(2, 3, 11);
            g.addEdge(2, 5, 2);
            g.addEdge(3, 4, 6);
            g.addEdge(4, 5, 9);

            LOG("DFS");
            g.dfs([](auto& p) { LOG(p->id); }, 0);

            LOG("BFS");
            g.bfs([](auto& p) { LOG(p->id); }, 0);

            auto v = dijkstraSPT(g, 0);
            auto p = primMST(g, 0);
        }
    };

    // Implement Topological Sorting
    struct p19
    {
        template<typename TCallback>
        void doDFSh(Graph<int>& g, vector<bool>& state, int start, TCallback callback)
        {
            state[start] = true;
            for (auto e : g.getNode(start)->adjList)
                if (!state[e.first->id])
                    doDFSh(g, state, e.first->id, callback);

            callback(start);
        }

        template<typename TCallback>
        void doDFS(Graph<int> g, TCallback callback)
        {
            vector<bool> state(g.maxNode() + 1, false);
            for (int i = g.minNode(); i <= g.maxNode(); ++i)
            {
                if (!state[i])
                    doDFSh(g, state, i, callback);
            }
        }

        vector<int> topoSort(Graph<int>& g)
        {
            vector<int> v;

            vector<bool> state(g.size(), false);
            for (int i = 0; i < g.size(); ++i)
            {
                if (!state[i])
                    doDFSh(g, state, i, [&v](int n) { v.push_back(n); });
            }

            reverse(v.begin(), v.end());
            return v;
        }

        void test()
        {
            // Introduction to Algorthms (3rd ed.) p.613
            vector<string> nd = {
                "undershorts",  //0
                "socks",        //1
                "watch",        //2
                "pants",        //3
                "shoes",        //4
                "shirt",        //5
                "belt",         //6
                "tie",          //7
                "jacket",       //8
            };

            Graph<int> g(true);

            g.addEdge(0, 3);
            g.addEdge(0, 4);
            g.addEdge(1, 4);
            g.addEdge(2, 2);
            g.addEdge(3, 4);
            g.addEdge(3, 6);
            g.addEdge(5, 6);
            g.addEdge(5, 7);
            g.addEdge(6, 8);
            g.addEdge(7, 8);

            auto t = topoSort(g);
        }
    };

    static void run()
    {
        p13::demo();
    }

    REGISTER_RUNNABLE(oth1)
}
