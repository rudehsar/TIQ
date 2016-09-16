#pragma once

typedef void(*PExtMain)();
extern unordered_map<PExtMain, bool> g_extMains;

#define REGISTER_RUNNABLE(x) \
    static void run(); \
    void x ## () { run(); } \
    class __register_ ## x \
    { \
    public: \
        __register_ ## x() { g_extMains[ ## x ## ] = true; } \
    }; \
    static __register_ ## x __register_ ## x ## _; \

#define LOG(x) std::cout << x << std::endl
#define VERIFY(x) do { \
    if (x) \
        LOG("verification passed (" ## #x ## ")"); \
    else \
        LOG("verification FAILED (" ## #x ## ")"); } while (0)

namespace cmn
{
    template<typename T>
    struct TreeNode
    {
        T val;

        shared_ptr<TreeNode<T>> parent;
        shared_ptr<TreeNode<T>> left;
        shared_ptr<TreeNode<T>> right;
    };

    template<typename T>
    using PTreeNode = shared_ptr<TreeNode<T>>;

    template<typename T>
    class BinaryTree
    {
    public:
        BinaryTree(PTreeNode<T> root = nullptr) :
            m_root(root)
        {
        }

        PTreeNode<T> getRoot()
        {
            return m_root;
        }

        PTreeNode<T> insertLeft(PTreeNode<T> p, T data)
        {
            return insertLeft(p, _makeNode(data));
        }

        PTreeNode<T> insertLeft(PTreeNode<T> p, PTreeNode<T> n)
        {
            if (!n)
                return nullptr;

            if (!p)
            {
                n->left = m_root;
                if (n->left)
                    n->left->parent = n;

                m_root = n;

            }
            else
            {
                p->left = n;
                p->left->parent = p;
            }

            return n;
        }

        PTreeNode<T> insertRight(PTreeNode<T> p, T data)
        {
            return insertRight(p, _makeNode(data));
        }

        PTreeNode<T> insertRight(PTreeNode<T> p, PTreeNode<T> n)
        {
            if (!n)
                return nullptr;

            if (!p)
            {
                n->right = m_root;
                if (n->right)
                    n->right->parent = n;

                m_root = n;

            }
            else
            {
                p->right = n;
                p->right->parent = p;
            }

            return n;
        }

        void remove(PTreeNode<T>& p)
        {
            if (!p)
                return;

            if (!p->left || !p->right)
                _transplant(p, p->left ? p->left : p->right);
            else
            {
                // find min from right subtree
                PTreeNode<T> n = p->right;
                while (n->left)
                    n = n->left;

                if (n != p->right)
                {
                    _transplant(n, n->right);

                    n->right = p->right;
                    n->right->parent = n;
                }

                n->left = p->left;
                n->left->parent = n;

                _transplant(p, n);
            }
        }

        void deleteTree(PTreeNode<T>& p)
        {
            if (!p)
                return;

            deleteTree(p->left);
            deleteTree(p->right);

            remove(p);
        }

        PTreeNode<T> find(T value)
        {
            PTreeNode<T> p = m_root;
            while (p && (p->val != value))
                p = p->val < value ? p->right : p->left;

            return p;
        }

        int getMaxDepth(PTreeNode<T>& p)
        {
            if (!p)
                return 0;

            return 1 + max(getMaxDepth(p->left), getMaxDepth(p->right));
        }

        string to_string(PTreeNode<int>& o)
        {
            if (!o)
                return "null";

            if (!o->left && !o->right)
                return ::to_string(o->val);

            return ::to_string(o->val) + " { " + to_string(o->left) + ", " + to_string(o->right) + " }";
        }

    protected:
        PTreeNode<T> m_root;

        PTreeNode<T> _makeNode(T val)
        {
            return PTreeNode<T>(new TreeNode<T>{ val, nullptr, nullptr, nullptr });
        }

    private:
        void _transplant(PTreeNode<T>& u, PTreeNode<T>& v)
        {
            if (!u->parent)
                m_root = v;
            else
            {
                if (u->parent->left == u)
                    u->parent->left = v;
                else
                    u->parent->right = v;
            }

            if (v)
                v->parent = u->parent;
        }
    };

    template<typename T>
    class BinarySearchTree : public BinaryTree<T>
    {
    public:
        PTreeNode<T> insert(T data)
        {
            return insert(_makeNode(data));
        }

        PTreeNode<T> insert(PTreeNode<T>& n)
        {
            PTreeNode<T> p = nullptr;
            PTreeNode<T> c = m_root;
            while (c)
            {
                p = c;
                if (n->val <= c->val)
                    c = c->left;
                else
                    c = c->right;
            }

            if (!p)
                m_root = n;
            else
            {
                if (n->val <= p->val)
                    p->left = n;
                else
                    p->right = n;

                n->parent = p;
            }

            return n;
        }

        PTreeNode<T> inOrderPredecessor(PTreeNode<T> n)
        {
            // if it contains a left branch, return the right-most node of that branch
            if (n->left)
            {
                n = n->left;
                while (n->right)
                    n = n->right;

                return n;
            }

            while (n->parent && (n->parent->right != n))
                n = n->parent;
                
            return n->parent;
        }

        PTreeNode<T> inOrderSuccessor(PTreeNode<T> n)
        {
            // if contains a right branch, return the left-most node of that branch
            if (n->right)
            {
                n = n->right;
                while (n->left)
                    n = n->left;

                return n;
            }

            while (n->parent && (n->parent->left != n))
                n = n->parent;

            return n->parent;
        }

        enum class TraversalType
        {
            PreOrder,
            InOrder,
            PostOrder,
        };

        template<typename TCallback>
        void traverse(TCallback&& callback, TraversalType traversalType = TraversalType::InOrder)
        {
            traverse(getRoot(), callback, traversalType);
        }

        template<typename TCallback>
        void traverse(const PTreeNode<T>& p, TCallback&& callback, TraversalType traversalType)
        {
            if (!p)
                return;

            switch (traversalType)
            {
            case TraversalType::PreOrder:
                callback(p);
                traverse(p->left, callback, traversalType);
                traverse(p->right, callback, traversalType);
                break;

            case TraversalType::InOrder:
                traverse(p->left, callback, traversalType);
                callback(p);
                traverse(p->right, callback, traversalType);
                break;

            case TraversalType::PostOrder:
                traverse(p->left, callback, traversalType);
                traverse(p->right, callback, traversalType);
                callback(p);
                break;

            default:
                assert(false);
            }
        }

        void traverse(const PTreeNode<T>& p = getRoot(), TraversalType traversalType = TraversalType::InOrder)
        {
            traverse(p, [](T& value) { LOG(value); }, traversalType);
        }
    };

    template<typename T>
    struct ListNode
    {
        T data;
        shared_ptr<ListNode<T>> next;
    };

    template<typename T>
    using PListNode = shared_ptr<ListNode<T>>;

    template<typename T>
    PListNode<T> convert_to(const vector<T>& v)
    {
        if (v.empty())
            return nullptr;

        shared_ptr<ListNode<int>> head(new ListNode<T>{ v[0], nullptr });

        shared_ptr<ListNode<int>> curr = head;
        for (int i = 1; i < v.size(); ++i)
        {
            shared_ptr<ListNode<int>> node(new ListNode<int>{ v[i], nullptr });
            curr->next = node;
            curr = node;
        }

        return head;
    }

    template<typename T>
    vector<T> convert_from(shared_ptr<ListNode<T>> head)
    {
        vector<T> v;
        while (head)
        {
            v.push_back(head->data);
            head = head->next;
        }

        return v;
    }

    template<typename T>
    struct GraphNode;

    template<typename T>
    using PGraphNode = shared_ptr<GraphNode<T>>;

    template<typename T>
    using PGraphNodes = vector<PGraphNode<T>>;

    template<typename T>
    using PGraphEdge = pair<PGraphNode<T>, int>;

    template<typename T>
    using PGraphEdges = vector<PGraphEdge<T>>;

    template<typename T>
    struct GraphNode
    {
        int id;
        PGraphEdges<T> adjList;
    };

    enum class TraversalState
    {
        UNDISCOVERED,
        DISCOVERED,
        PROCESSED
    };

    template<typename T>
    class Graph
    {
    public:
        using iterator = typename PGraphNodes<T>::iterator;

        Graph(bool isDirected = false) :
            m_isDirectedGraph(isDirected)
        {
        }

        bool isDirected() const
        {
            return m_isDirectedGraph;
        }

        PGraphNode<T> addNode()
        {
            m_vertices.emplace_back(make_shared<GraphNode<T>>());
            m_vertices.back()->id = m_vertices.size() - 1;

            return m_vertices.back();
        }

        PGraphNode<T> getNode(int id) const
        {
            assert(id < m_vertices.size());
            return m_vertices[id];
        }

        PGraphEdges<T> getEdges(int id) const
        {
            return m_vertices[id]->adjList;
        }

        void addEdge(int sourceId, int destinationId, int weight = 0)
        {
            //assert(sourceId < m_vertices.size());
            //assert(destinationId < m_vertices.size());

            // ensure nodes
            int maxId = max(sourceId, destinationId);
            while (!(maxId < m_vertices.size()))
                addNode();

            m_vertices[sourceId]->adjList.emplace_back(m_vertices[destinationId], weight);
            if (!m_isDirectedGraph)
            {
                m_vertices[destinationId]->adjList.emplace_back(m_vertices[sourceId], weight);
            }
        }

        iterator begin()
        {
            return m_vertices.begin();
        }

        iterator end()
        {
            return m_vertices.end();
        }

        size_t size() const
        {
            return m_vertices.size();
        }

        template<typename TCallback>
        void dfs(TCallback callback, int sourceId = 0)
        {
            vector<TraversalState> state(size(), TraversalState::UNDISCOVERED);
            _dfs(sourceId, state, callback);
        }

        template<typename TCallback>
        void bfs(TCallback callback, int sourceId = 0)
        {
            vector<TraversalState> state(size(), TraversalState::UNDISCOVERED);

            state[sourceId] = TraversalState::DISCOVERED;

            queue<int> q;
            q.push(sourceId);
            while (!q.empty())
            {
                int id = q.front();
                q.pop();

                for (auto p : getNode(id)->adjList)
                {
                    if (state[p.first->id] == TraversalState::UNDISCOVERED)
                    {
                        q.push(p.first->id);
                        state[p.first->id] = TraversalState::DISCOVERED;
                    }
                }

                callback(getNode(id));
                state[id] = TraversalState::PROCESSED;
            }
        }

    private:
        template<typename TCallback>
        void _dfs(int id, vector<TraversalState>& state, TCallback callback)
        {
            PGraphNode<T> n = getNode(id);
            state[id] = TraversalState::DISCOVERED;

            for (auto it = n->adjList.begin(); it != n->adjList.end(); ++it)
            {
                int neighborId = it->first->id;
                if (state[neighborId] == TraversalState::UNDISCOVERED)
                    _dfs(neighborId, state, callback);
            }

            callback(n);
            state[id] = TraversalState::PROCESSED;
        }

        PGraphNodes<T> m_vertices;
        bool m_isDirectedGraph;
    };
}
