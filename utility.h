#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>
using namespace std;

#include "common.h"
using namespace cmn;

namespace utl
{
    template<typename T>
    double findDuration(T callback, long loopCount = 0, int maxWaitInSeconds = 1);

    string getTimestamp();

    template<typename T>
    int getElementPrintWidth(const T& x);
    template<>
    int getElementPrintWidth<int>(const int& x);
    template<>
    int getElementPrintWidth<string>(const string& x);

    template<typename T>
    int getPrintWidth(const vector<T>& v);
    template<>
    int getPrintWidth(const vector<bool>& v);

    template<typename T>
    void printVector(vector<T> v, int width = 0);
    template<>
    void printVector(vector<bool> v, int width);
    template<typename T>
    void printVector(vector<vector<T>> m, int width = 0);
    void printVector(const string& s, int width = 0);

    // print DP matrix given horizontal header vector, veritical header vector
    template<typename T, typename U, typename V>
    void printDPMatrix(vector<vector<T>>& dp, vector<U> horz, vector<V> vert, int width = 0);

    /// <summary>
    /// Get a random number within range [min, max).
    /// </summary>
    int getRandom(int min = 0, int max = INT_MAX);

    /// <summary>
    /// Get a random vector of length n.
    /// </summary>
    vector<int> getRandom(int min, int max, int n);

    /// <summary>
    /// Get a random matrix of size r x c.
    /// </summary>
    vector<vector<int>> getRandom(int min, int max, int r, int c);

    template<typename T>
    BinaryTree<T> getRandom(BinaryTree<T>& b, const vector<T>& v);

    template<typename T>
    BinarySearchTree<T> getBinaryTree(BinarySearchTree<T>& b, const vector<T>& values, bool balanced = false);

    vector<int> getSeqVector(int start, int end, int incr = 1);

    void printBinaryTree(BinaryTree<int>& b);

    /*
    template<typename T>
    double findDuration(T callback, long loopCount = 0, unsigned int maxDurationInSeconds = 1)
    {
    using namespace std::chrono;

    auto tic = high_resolution_clock::now();
    if (loopCount <= 0)
    {
    callback();
    loopCount = seconds(maxDurationInSeconds) / (high_resolution_clock::now() - tic);
    }

    tic = high_resolution_clock::now();
    int l = loopCount;
    while (l-- > 0)
    {
    callback();
    }

    return duration_cast<microseconds>(high_resolution_clock::now() - tic).count() / (double)loopCount;
    }
    */

    // Return elapsed duration in us (MicroSeconds).
    template<typename T>
    double findDuration(T callback, long loopCount, int maxWaitInSeconds)
    {
        if (loopCount == 0)
        {
            // prime and get raw duration to determine loop count
            double durationInUs = findDuration(callback, 1);
            loopCount = (int)(maxWaitInSeconds * 1e6 / durationInUs);
        }

        auto tic = chrono::high_resolution_clock::now();

        int l = loopCount;
        while (l-- > 0)
        {
            callback();
        }

        auto toc = chrono::high_resolution_clock::now();

        return (double)chrono::duration_cast<chrono::microseconds>((toc - tic) / loopCount).count();
    }

    template<typename T>
    int getElementPrintWidth(const T& x)
    {
        return 0;
    }

    template<typename T>
    int getPrintWidth(const vector<T>& v)
    {
        int width = 0;
        for (auto& x : v)
            width = max(width, getElementPrintWidth(x));

        return width;
    }

    template<typename T>
    void printVector(vector<T> v, int width)
    {
        width = (width > 0) ? width : getPrintWidth(v);

        cout << "[";

        for (int i = 0; i < v.size(); ++i)
            cout << setw(width + 1) << right << v[i];

        cout << setw(width + 1) << right << "]" << endl;
    }

    template<typename T>
    void printVector(vector<vector<T>> m, int width)
    {
        if (!(width > 0))
        {
            for (auto& v : m)
                width = max(width, getPrintWidth(v));
        }

        cout << "{ " << m.size() << " x " << m[0].size() << " }" << endl;

        for (int i = 0; i < m.size(); ++i)
            printVector(m[i], width);

        cout << endl;
    }

    // print DP matrix given horizontal header vector, veritical header vector
    template<typename T, typename U, typename V>
    void printDPMatrix(vector<vector<T>>& dp, vector<U> horz, vector<V> vert, int width)
    {
        // assert(horz.size() + 1 >= dp.size());
        // assert(vert.size() + 1 >= dp[0].size());

        vector<vector<string>> m(dp.size() + 1, vector<string>(dp[0].size() + 1));

        int k = m[0].size() - 1;
        for (auto it = horz.rbegin(); it != horz.rend(); ++it)
            m[0][k--] = to_string(*it);

        k = m.size() - 1;
        for (auto it = vert.rbegin(); it != vert.rend(); ++it)
            m[k--][0] = to_string(*it);

        for (int i = 0; i < dp.size(); ++i)
            for (int j = 0; j < dp[0].size(); ++j)
                m[i + 1][j + 1] = to_string(dp[i][j]);

        printVector(m, width);
    }

    template<typename T>
    BinaryTree<T> getRandom(BinaryTree<T>& b, const vector<T>& v)
    {
        for (auto& x : v)
        {
            PTreeNode<T> n = b.getRoot();
            while (true)
            {
                bool insertLeft = getRandom(0, 10) > 5;

                if (!n || (insertLeft && !n->left) || (!insertLeft && !n->right))
                {
                    if (insertLeft)
                        b.insertLeft(n, x);
                    else
                        b.insertRight(n, x);

                    break;
                }

                n = insertLeft ? n->left : n->right;
            }
        }

        return b;
    }

    template<typename T>
    BinarySearchTree<T> getBinaryTree(BinarySearchTree<T>& b, const vector<T>& values, bool balanced)
    {
        if (balanced && values.size() > 2)
        {
            vector<T> v(values);
            sort(v.begin(), v.end());

            int mid = v.size() / 2;
            auto n = b.insert(v[mid]);

            auto l = getBinaryTree(BinarySearchTree<T>(), vector<T>(v.begin(), v.begin() + mid), true);
            auto r = getBinaryTree(BinarySearchTree<T>(), vector<T>(v.begin() + mid + 1, v.end()), true);

            b.insertLeft(n, l.getRoot());
            b.insertRight(n, r.getRoot());
        }
        else
        {
            for (auto& x : values)
                b.insert(x);
        }

        return b;
    }
}
