#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>
#include <queue>
#include "TreeNode.hpp"

/**
 *@brief 非递归写法的 BST
 */
template <typename T>
class BST {
private:
    using NodeType = TreeNode<T>;
    NodeType* m_root;

    int nodeCount;  // 检查内存泄漏
public:
    BST() :m_root(nullptr), nodeCount(0) {}

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
            m_root = new TreeNode(value);
            nodeCount += 1;
            return;
        }
        NodeType* search_ptr = m_root;
        while (search_ptr)
        {
            if (search_ptr->value() < value)
            {
                if (nullptr == search_ptr->right())
                {
                    search_ptr->right() = new TreeNode(value);
                    nodeCount += 1;
                    break;
                }
                else search_ptr = search_ptr->right();
            }
            else if (search_ptr->value() > value)
            {
                if (nullptr == search_ptr->left())
                {
                    search_ptr->left() = new TreeNode(value);
                    nodeCount += 1;
                    break;
                }
                else search_ptr = search_ptr->left();
            }
            else
            {
                search_ptr->count() += 1;
                break;
            }
        }
    }

    void removeNode(T value)
    {
        if (nullptr == m_root) return;
        NodeType** pre_search_ptr = &m_root;
        NodeType* search_ptr = m_root;
        // 找到待删除元素
        while (search_ptr)
        {
            if (value < search_ptr->value())
            {
                pre_search_ptr = &(search_ptr->left());
                search_ptr = search_ptr->left();
            }
            else if (value > search_ptr->value())
            {
                pre_search_ptr = &(search_ptr->right());
                search_ptr = search_ptr->right();
            }
            else break;
        }

        if (nullptr == search_ptr) return;  // 树中不存在待删除的元素

        if (search_ptr->count() > 1)
        {
            search_ptr->count()--;
            return;
        }

        if (nullptr == search_ptr->left())    // 待删除节点左子树为空(右子树可能为空, 也可能不空)
        {
            *pre_search_ptr = search_ptr->right();
            delete search_ptr;
            nodeCount--;
        }
        else if (nullptr == search_ptr->right())  // 待删除节点右子树为空(左子树不空)
        {
            *pre_search_ptr = search_ptr->left();
            delete search_ptr;
            nodeCount--;
        }
        else  // 待删除节点左右子树都不空
        {
            // 得到右子树的最小节点
            NodeType* min_node = cutMinimum(&search_ptr->right());
            min_node->left() = search_ptr->left();
            min_node->right() = search_ptr->right();
            *pre_search_ptr = min_node;
            delete search_ptr;
            nodeCount--;
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

    ~BST()
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

    // 从子树中摘下最小的节点(不论其计数多少), 当子树根节点为最小节点时会改变传入的子树指针
    // 如果子树只有一个元素, 返回其自身
    // 如果子树为空, 返回 nullptr
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
};

#endif