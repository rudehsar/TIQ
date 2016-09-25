#include "stdafx.h"
#include "utility.h"
#include <windows.h>

template<>
int utl::getElementPrintWidth<int>(const int& x)
{
    auto ax = abs(x);
    if (ax <= 1)
        return 1 + static_cast<int>(x < 0);

    return static_cast<int>(ceil(log10(ax + 1))) + static_cast<int>(x < 0);
}

template<>
int utl::getElementPrintWidth<string>(const string& x)
{
    return x.size();
}

template<>
int utl::getPrintWidth(const vector<bool>& v)
{
    return 2;
}

template<>
void utl::printVector(vector<bool> v, int width)
{
    cout << "[";

    for (int i = 0; i < v.size(); ++i)
        cout << setw(width + 1) << right << (v[i] ? "T" : "F");

    cout << setw(width + 1) << right << "]" << endl;
}

void utl::printVector(const string& s, int width)
{
    vector<char> sv(s.begin(), s.end());

    vector<int> iv(s.size());
    iota(iv.begin(), iv.end(), 0);

    width = max(width, getPrintWidth(iv));
    width = max(width, getPrintWidth(sv));

    printVector(iv, width);
    printVector(sv, width);
}

int utl::getRandom(int min, int max)
{
    static default_random_engine eng(random_device{}());
    auto d = uniform_int_distribution<int>(min, max);

    return d(eng);
}

vector<int> utl::getRandom(int min, int max, int n)
{
    vector<int> v;
    while (n-- > 0)
        v.push_back(utl::getRandom(min, max));

    return v;
}

vector<vector<int>> utl::getRandom(int min, int max, int r, int c)
{
    vector<vector<int>> m;
    while (r-- > 0)
        m.push_back(utl::getRandom(min, max, c));

    return m;
}

vector<int> utl::getSeqVector(int start, int end, int incr)
{
    vector<int> v((end - start) / incr + 1);

    if (incr == 1)
        iota(v.begin(), v.end(), start);
    else
    {
        v[0] = start;
        for (int i = 1; i < v.size(); ++i)
            v[i] = v[i - 1] + incr;
    }

    return v;
}

void printBinaryTreeHelper(PTreeNode<int>& p, int r, int px, int of, vector<vector<int>>& m)
{
    if (!p)
        return;

    int x = px;
    if (p->parent)
        x = px + of * (p->parent->left == p ? -1 : 1);

    m[r][x] = p->val;

    printBinaryTreeHelper(p->left, r + 1, x, of / 2, m);
    printBinaryTreeHelper(p->right, r + 1, x, of / 2, m);
}

void utl::printBinaryTree(BinaryTree<int>& b)
{
    LOG(b.to_string(b.getRoot()));

    //int d = b.getMaxDepth(b.getRoot());

    //vector<vector<int>> m(d, vector<int>(static_cast<int>(pow(2, d)) - 1, INT_MIN));
    //printBinaryTreeHelper(b.getRoot(), 0, m[0].size() / 2, (m[0].size() + 1) / 2, m);

    //const int width = 4;
    //for (int i = 0; i < m.size(); ++i)
    //{
    //    for (int j = 0; j < m[0].size(); ++j)
    //    {
    //        if (m[i][j] != INT_MIN)
    //            cout << setw(width) << m[i][j];
    //        else
    //            cout << setw(width) << "";
    //    }

    //    cout << endl;
    //}
}

string utl::getTimestamp()
{
    using namespace std::chrono;

    // get now
    auto td = system_clock::now().time_since_epoch();

    // extract time parts
    int ts[] = { 0, 0, 0, 0 };
    auto h = duration_cast<hours>(td);
    ts[0] = h.count() % 24;
    td -= h;
    auto m = duration_cast<minutes>(td);
    ts[1] = m.count();
    td -= m;
    auto s = duration_cast<seconds>(td);
    ts[2] = static_cast<int>(s.count());
    td -= s;
    auto l = duration_cast<milliseconds>(td);
    ts[3] = static_cast<int>(l.count());
    td -= l;

    int ti = static_cast<int>(GetCurrentThreadId());

    // get the required buffer size (+ terminating null)
    int len = snprintf(nullptr, 0, "%02d:%02d:%02d.%03d [%05d]", ts[0], ts[1], ts[2], ts[3], ti) + 1;

    // format string
    unique_ptr<char[]> pb(new char[len]);
    snprintf(pb.get(), len, "%02d:%02d:%02d.%03d [%05d]", ts[0], ts[1], ts[2], ts[3], ti);

    return string(pb.get());
}
