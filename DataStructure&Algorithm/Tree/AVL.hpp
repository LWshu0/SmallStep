#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <assert.h>

#include "TreeNode.hpp"

template <typename T>
class AVL {
private:
    using NodeType = AVLNode<T>;
    NodeType* m_root;

    int nodeCount;  // 检查内存泄漏
public:
    AVL() :m_root(nullptr), nodeCount(0) {}

    inline int getCount() { return nodeCount; }
    inline NodeType* root() { return m_root; }

    NodeType* findNode(T value)
    {
        NodeType* search_ptr = m_root;
        while (search_ptr)
        {
            if (value < search_ptr->value()) search_ptr = search_ptr->left();
            else if (value > search_ptr->value()) search_ptr = search_ptr->right();
            else return search_ptr;
        }
        return nullptr;
    }

    void insertNode(T value)
    {
        if (nullptr == m_root)
        {
            m_root = new NodeType(value);
            nodeCount += 1;
            return;
        }

        std::stack<NodeType**> path;
        path.push(&m_root);
        NodeType* search_ptr = m_root;
        while (search_ptr)
        {
            if (search_ptr->value() < value)
            {
                if (nullptr == search_ptr->right())
                {
                    search_ptr->right() = new AVLNode(value);
                    nodeCount += 1;
                    break;
                }
                else
                {
                    path.push(&search_ptr->right());
                    search_ptr = search_ptr->right();
                }
            }
            else if (search_ptr->value() > value)
            {
                if (nullptr == search_ptr->left())
                {
                    search_ptr->left() = new AVLNode(value);
                    nodeCount += 1;
                    break;
                }
                else
                {
                    path.push(&search_ptr->left());
                    search_ptr = search_ptr->left();
                }
            }
            else
            {
                // 没有新建节点, 树结构不变
                // 不需要做旋转操作, 直接返回
                search_ptr->count() += 1;
                return;
            }
        }

        while (!path.empty())
        {
            NodeType** pre_this_ptr = path.top();   // 树中指向 this_node 的指针的指针
            NodeType* this_node = *pre_this_ptr;
            int this_balance = getBalance(this_node);
            if (-2 == this_balance)
            {
                NodeType* right_child = this_node->right();
                int right_balance = getBalance(right_child);
                if (1 == right_balance) this_node->right() = rotateRight(this_node->right());
                *pre_this_ptr = rotateLeft(this_node);
            }
            else if (2 == this_balance)
            {
                NodeType* left_child = this_node->left();
                int left_balance = getBalance(left_child);
                if (-1 == left_balance) this_node->left() = rotateLeft(this_node->left());
                *pre_this_ptr = rotateRight(this_node);
            }
            updateHeight(this_node);
            path.pop();
        }
    }

    void removeNode(T value)
    {
        std::stack<NodeType**> path;
        path.push(&m_root);
        NodeType* search_ptr = m_root;
        while (search_ptr)
        {
            if (search_ptr->value() < value)
            {
                path.push(&search_ptr->right());
                search_ptr = search_ptr->right();
            }
            else if (search_ptr->value() > value)
            {
                path.push(&search_ptr->left());
                search_ptr = search_ptr->left();
            }
            else
            {
                search_ptr->count() -= 1;
                break;
            }
        }

        if (nullptr == search_ptr) return;  // 树中不存在待删除的元素

        // 节点计数大于 0 时, 不改变树结构
        if (search_ptr->count() > 0) return;

        // delete
        if (nullptr == search_ptr->left())    // 待删除节点左子树为空(右子树可能为空, 也可能不空)
        {
            *path.top() = search_ptr->right();
            delete search_ptr;
            nodeCount--;
        }
        else if (nullptr == search_ptr->right())  // 待删除节点右子树为空(左子树不空)
        {
            *path.top() = search_ptr->left();
            delete search_ptr;
            nodeCount--;
        }
        else  // 待删除节点左右子树都不空
        {
            // 得到右子树的最小节点
            NodeType* min_node = cutMinimum(&search_ptr->right());
            min_node->left() = search_ptr->left();
            min_node->right() = search_ptr->right();
            *path.top() = min_node;
            delete search_ptr;
            nodeCount--;
        }

        // 调整树结构
        while (!path.empty())
        {
            NodeType** pre_this_ptr = path.top();   // 树中指向 this_node 的指针的指针
            NodeType* this_node = *pre_this_ptr;
            if (nullptr == this_node)
            {
                path.pop();
                continue;
            }
            int this_balance = getBalance(this_node);
            if (-2 == this_balance)
            {
                NodeType* right_child = this_node->right();
                int right_balance = getBalance(right_child);
                if (1 == right_balance) this_node->right() = rotateRight(this_node->right());
                *pre_this_ptr = rotateLeft(this_node);
            }
            else if (2 == this_balance)
            {
                NodeType* left_child = this_node->left();
                int left_balance = getBalance(left_child);
                if (-1 == left_balance) this_node->left() = rotateLeft(this_node->left());
                *pre_this_ptr = rotateRight(this_node);
            }
            updateHeight(this_node);
            path.pop();
        }
    }

    bool checkSortTree()
    {
        std::queue<NodeType*> bfs;
        bfs.push(m_root);
        while (!bfs.empty())
        {
            NodeType* this_root = bfs.front();
            if (nullptr != this_root->left())
            {
                if (this_root->left()->value() > this_root->value()) return false;
                bfs.push(this_root->left());
            }
            if (nullptr != this_root->right())
            {
                if (this_root->right()->value() < this_root->value()) return false;
                bfs.push(this_root->right());
            }
            bfs.pop();
        }
        return true;
    }

    void clear()
    {
        deleteTree(m_root);
        m_root = nullptr;
    }

    ~AVL()
    {
        std::cout << "before delete, total node count: " << nodeCount << std::endl;
        deleteTree(m_root);
        std::cout << "after delete, undeleted node count: " << nodeCount << std::endl;
    }

private:

    void deleteTree(NodeType* root)
    {
        if (nullptr == root) return;
        deleteTree(root->left());
        deleteTree(root->right());
        delete root;
        nodeCount -= 1;
    }

    NodeType* cutMinimum(NodeType** subtree)
    {
        // 传入子树为空, 返回 nullptr
        if (nullptr == *subtree) return nullptr;
        NodeType** pre_search_ptr = subtree;
        NodeType* search_ptr = *subtree;
        // 找到最小元素节点
        while (search_ptr->left())
        {
            pre_search_ptr = &(search_ptr->left());
            search_ptr = search_ptr->left();
        }
        // 将最小节点的右子树接到其父节点的 left 处
        *pre_search_ptr = search_ptr->right();
        search_ptr->right() = nullptr;
        return search_ptr;
    }

    NodeType* rotateLeft(NodeType* subtree)
    {
        assert(nullptr != subtree);
        NodeType* new_root = subtree->right();
        assert(nullptr != new_root);
        subtree->right() = new_root->left();
        new_root->left() = subtree;
        updateHeight(subtree);
        updateHeight(new_root);
        return new_root;
    }

    NodeType* rotateRight(NodeType* subtree)
    {
        assert(nullptr != subtree);
        NodeType* new_root = subtree->left();
        assert(nullptr != new_root);
        subtree->left() = new_root->right();
        new_root->right() = subtree;
        updateHeight(subtree);
        updateHeight(new_root);
        return new_root;
    }

    int getBalance(NodeType* node)
    {
        int left_depth = (nullptr == node->left()) ? 0 : node->left()->height();
        int right_depth = (nullptr == node->right()) ? 0 : node->right()->height();
        return left_depth - right_depth;
    }

    bool updateHeight(NodeType* node)
    {
        int left_depth = (nullptr == node->left()) ? 0 : node->left()->height();
        int right_depth = (nullptr == node->right()) ? 0 : node->right()->height();
        int new_depth = 1 + (left_depth > right_depth ? left_depth : right_depth);
        if (new_depth == node->height()) return false;
        else
        {
            node->height() = new_depth;
            return true;
        }
    }
};

#endif