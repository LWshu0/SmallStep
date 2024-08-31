#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <exception>
#include <iostream>
#include <vector>
#include <queue>
#include "TreeNode.hpp"

template<typename T>
class rbTree {
public:
    class rbTreeException : protected std::exception {
    private:
        const char* message;

    public:
        explicit rbTreeException(const char* msg) : message(msg) {}

        const char* what() const noexcept override { return message; }
    };

private:
    using NodeType = rbTreeNode<T>;
    NodeType* m_root;

    int nodeCount;  // 检查内存泄漏
public:
    rbTree() :m_root(nullptr), nodeCount(0) {}

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
        /*
            case 红黑树为空
            root 创建新节点并置为黑色
        */
        if (nullptr == m_root)
        {
            m_root = new NodeType(value);
            m_root->setBlack();
            nodeCount += 1;
            return;
        }
        /*
            查找插入位置
        */
        NodeType* search_ptr = m_root;
        NodeType* new_node = nullptr;
        while (search_ptr)
        {
            if (search_ptr->value() < value)
            {
                if (nullptr == search_ptr->right())
                {
                    new_node = new NodeType(value);
                    search_ptr->right() = new_node;
                    new_node->parent() = search_ptr;
                    nodeCount += 1;
                    break;
                }
                else
                {
                    search_ptr = search_ptr->right();
                }
            }
            else if (search_ptr->value() > value)
            {
                if (nullptr == search_ptr->left())
                {
                    new_node = new NodeType(value);
                    search_ptr->left() = new_node;
                    new_node->parent() = search_ptr;
                    nodeCount += 1;
                    break;
                }
                else
                {
                    search_ptr = search_ptr->left();
                }
            }
            else
            {
                // 没有新建节点, 树结构不变
                // 不需要做调整, 直接返回
                search_ptr->count() += 1;
                return;
            }
        }

        assert(nullptr != new_node);
        assert(nullptr != search_ptr);

        /*
            case 新插入节点的父节点为红色
            此时必定有祖父节点(且为祖父节点黑色)
        */
        while (nullptr != new_node)
        {
            // 根节点置为黑色
            if (new_node->isRoot())
            {
                new_node->setBlack();
                break;
            }

            // 非根节点 检查父节点颜色
            NodeType* parent_ptr = new_node->parent();
            if (parent_ptr->isBlack())
            {
                /*
                    case 新插入节点的父节点为黑色
                    do nothing
                */
                break;
            }

            // 非根节点且父节点为红色, 必定有祖父节点(黑色)与叔节点(未知颜色)
            NodeType* grandparent_ptr = parent_ptr->parent();
            NodeType* uncle_ptr = new_node->uncle();

            // 叔节点为黑色
            if (nullptr == uncle_ptr || uncle_ptr->isBlack())
            {
                switch (parent_ptr->direction())
                {
                case NodeType::Direction::LEFT: // 父节点是祖父节点的左子树
                    if (NodeType::Direction::RIGHT == new_node->direction()) rotateLeft(parent_ptr); // 如果当前节点是父节点的右子树, 左旋以父节点为根的子树, 即 parent_ptr
                    swapColor(grandparent_ptr, grandparent_ptr->left()); // 交换前 grandparent_ptr 必为黑色, grandparent_ptr->left() 必为红色
                    rotateRight(grandparent_ptr);
                    break;
                case NodeType::Direction::RIGHT: // 父节点是祖父节点的右子树
                    if (NodeType::Direction::LEFT == new_node->direction()) rotateRight(parent_ptr); // 如果当前节点是父节点的左子树, 右旋以父节点为根的子树, 即 parent_ptr
                    swapColor(grandparent_ptr, grandparent_ptr->right()); // 交换前 grandparent_ptr 必为黑色, grandparent_ptr->left() 必为红色
                    rotateLeft(grandparent_ptr);
                    break;
                default:
                    // 父节点为根节点且其为红色
                    // 违反红黑树定义
                    throw rbTreeException("Error in rbTree::insertNode: parent_dire can't be root (001)");
                    break;
                }

                // 调整过后, 子树根节点为黑色
                // 跳出循环
                break;
            }
            // 叔节点为红色
            else
            {
                uncle_ptr->setBlack();
                parent_ptr->setBlack();
                grandparent_ptr->setRed();
                // 循环检查祖父节点在变色后是否违反红黑树定义
                // e.g. 当祖父节点的父节点为红色时, 变色后违反红黑树定义(因为相邻两个父子节点都为红色)
                new_node = grandparent_ptr;
            }
        }
    }

    void removeNode(T value)
    {
        NodeType* delete_ptr = m_root;
        while (delete_ptr)
        {
            if (delete_ptr->value() < value)
            {
                delete_ptr = delete_ptr->right();
            }
            else if (delete_ptr->value() > value)
            {
                delete_ptr = delete_ptr->left();
            }
            else
            {
                delete_ptr->count() -= 1;
                // 删除一个后节点计数大于 0, 不改变树结构
                if (delete_ptr->count() > 0) return;
                break;
            }
        }

        if (nullptr == delete_ptr) return;  // 树中不存在待删除的元素

        // 待删除节点左右子树都不空
        // 找中序后继交换位置(仅交换值)
        // 将删除节点置为中序后继
        if (nullptr != delete_ptr->left() && nullptr != delete_ptr->right())
        {
            // 中序后继
            NodeType* min_ptr = findMinimum(delete_ptr->right());
            swapNode_exceptColor(delete_ptr, min_ptr);
            delete_ptr = min_ptr;
        }

        // 叶子节点
        if (nullptr == delete_ptr->left() && nullptr == delete_ptr->right())
        {
            if (delete_ptr->isRed())
            {
                onRemoveRedLeaf(delete_ptr);
            }
            else
            {
                onRemoveBlackLeaf(delete_ptr);
            }
        }
        // 只有左子树(左子树必定为一个红节点)
        else if (nullptr != delete_ptr->left())
        {
            swapNode_exceptColor(delete_ptr, delete_ptr->left());
            delete delete_ptr->left();
            delete_ptr->left() = nullptr;
            nodeCount--;
        }
        // 只有右子树(右子树必定为一个红节点)
        else
        {
            swapNode_exceptColor(delete_ptr, delete_ptr->right());
            delete delete_ptr->right();
            delete_ptr->right() = nullptr;
            nodeCount--;
        }
    }

    bool checkSortTree()
    {
        std::queue<NodeType*> bfs;
        if (nullptr != m_root) bfs.push(m_root);
        while (!bfs.empty())
        {
            NodeType* this_root = bfs.front();
            if (nullptr != this_root->left())
            {
                if (this_root->left()->value() > this_root->value()) return false;
                if (this_root->isRed() && this_root->left()->isRed())
                {
                    return false;
                }
                bfs.push(this_root->left());
            }
            if (nullptr != this_root->right())
            {
                if (this_root->right()->value() < this_root->value()) return false;
                if (this_root->isRed() && this_root->right()->isRed()) return false;
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

    ~rbTree()
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

    // 找到子树最小节点
    NodeType* findMinimum(NodeType* subtree)
    {
        assert(nullptr != subtree);
        // 找到最小元素节点
        while (subtree->left())
        {
            subtree = subtree->left();
        }
        return subtree;
    }

    // 仅删除节点时调用
    // 交换两节点的值, 计数
    void swapNode_exceptColor(NodeType* node1, NodeType* node2)
    {
        assert(nullptr != node1);
        assert(nullptr != node2);
        assert(node1 != node2);

        std::swap(node2->value(), node1->value());
        std::swap(node2->count(), node1->count());
    }

    void swapColor(NodeType* node1, NodeType* node2)
    {
        assert(nullptr != node1);
        assert(nullptr != node2);
        assert(node1 != node2);

        std::swap(node1->color(), node2->color());
    }

    void onRemoveRedLeaf(NodeType* node)
    {
        assert(nullptr == node->left() && nullptr == node->right());
        switch (node->direction())
        {
        case NodeType::Direction::LEFT:
            node->parent()->left() = nullptr;
            break;
        case NodeType::Direction::RIGHT:
            node->parent()->right() = nullptr;
            break;
        default:
            m_root = nullptr;
            break;
        }

        delete node;
        nodeCount--;
    }

    // 删除黑色叶子节点
    void onRemoveBlackLeaf(NodeType* node)
    {
        assert(nullptr == node->left() && nullptr == node->right());

        typename NodeType::Direction node_dire = node->direction();
        switch (node_dire)
        {
        case NodeType::Direction::LEFT: {
            node = node->parent();
            delete node->left();
            nodeCount--;
            node->left() = nullptr;
            break;
        }
        case NodeType::Direction::RIGHT: {
            node = node->parent();
            delete node->right();
            nodeCount--;
            node->right() = nullptr;
            break;
        }
        default: {
            delete m_root;
            nodeCount--;
            m_root = nullptr;
            return;
        }
        }

        // 调整
        bool modify_flag = true;
        do
        {
            switch (node_dire)
            {
            case NodeType::Direction::LEFT: {
                modify_flag = onLeftChildDualBlack(node);
                break;
            }
            case NodeType::Direction::RIGHT: {
                modify_flag = onRightChildDualBlack(node);
                break;
            }
            default: {
                // 根节点已删除不需要调整
                node = m_root;
                return;
            }
            }
            node_dire = node->direction(); // 双黑节点的方向
            node = node->parent();  // 新双黑节点的父节点
        } while (modify_flag);
    }

    // 返回 parent_ptr 指向的节点是否为双重黑
    bool onLeftChildDualBlack(NodeType* parent_ptr)
    {
        NodeType* sibling_ptr = parent_ptr->right();
        if (sibling_ptr->isRed())
        {
            rotateLeft(parent_ptr);
            swapColor(sibling_ptr, parent_ptr);
            sibling_ptr = parent_ptr->right();
        }

        assert(sibling_ptr->isBlack());

        // sibling 右红 左黑/红
        if (nullptr != sibling_ptr->right() && sibling_ptr->right()->isRed())
        {
            sibling_ptr->right()->setBlack();
            swapColor(parent_ptr, sibling_ptr);
            rotateLeft(parent_ptr);
        }
        // sibling 右黑 左红
        else if (nullptr != sibling_ptr->left() && sibling_ptr->left()->isRed())
        {
            swapColor(sibling_ptr, sibling_ptr->left());
            rotateRight(sibling_ptr);
            swapColor(parent_ptr, parent_ptr->right());
            sibling_ptr->setBlack();
            rotateLeft(parent_ptr);
        }
        // sibling 右黑 左黑
        else
        {
            sibling_ptr->setRed();
            if (parent_ptr->isRed()) parent_ptr->setBlack();
            else return true;
        }
        return false;
    }

    // 返回 parent_ptr 指向的节点是否为双重黑
    bool onRightChildDualBlack(NodeType* parent_ptr)
    {
        NodeType* sibling_ptr = parent_ptr->left();
        if (sibling_ptr->isRed())
        {
            rotateRight(parent_ptr);
            swapColor(sibling_ptr, parent_ptr);
            sibling_ptr = parent_ptr->left();
        }

        assert(sibling_ptr->isBlack());

        // sibling left-red  right-black/red
        if (nullptr != sibling_ptr->left() && sibling_ptr->left()->isRed())
        {
            sibling_ptr->left()->setBlack();
            swapColor(parent_ptr, sibling_ptr);
            rotateRight(parent_ptr);
        }
        // sibling left-black  right-red
        else if (nullptr != sibling_ptr->right() && sibling_ptr->right()->isRed())
        {
            swapColor(sibling_ptr, sibling_ptr->right());
            rotateLeft(sibling_ptr);
            sibling_ptr->setBlack();
            swapColor(parent_ptr, parent_ptr->left());
            rotateRight(parent_ptr);
        }
        // sibling left-black  right-black
        else
        {
            sibling_ptr->setRed();
            if (parent_ptr->isRed()) parent_ptr->setBlack();
            else return true;
        }
        return false;
    }

    void rotateLeft(NodeType* subtree)
    {
        /*
            R = subtree
            S = new_root
                R            S
               / \          / \
              rl  S   =>   R   M
                 / \      / \
                sl  M    rl  sl
        */
        assert(nullptr != subtree);
        NodeType* new_root = subtree->right();
        assert(nullptr != new_root);

        switch (subtree->direction())
        {
        case NodeType::Direction::LEFT:
            subtree->parent()->left() = new_root;
            break;
        case NodeType::Direction::RIGHT:
            subtree->parent()->right() = new_root;
            break;
        default:
            m_root = new_root;
            break;
        }
        new_root->parent() = subtree->parent();

        subtree->right() = new_root->left();
        if (nullptr != subtree->right()) subtree->right()->parent() = subtree;

        new_root->left() = subtree;
        subtree->parent() = new_root;
    }

    void rotateRight(NodeType* subtree)
    {
        /*
            R = subtree
            S = new_root
                R          S
               / \        / \
              S  rr  =>  M   R
             / \            / \
            M  sr          sr rr
        */
        assert(nullptr != subtree);
        NodeType* new_root = subtree->left();
        assert(nullptr != new_root);

        switch (subtree->direction())
        {
        case NodeType::Direction::LEFT:
            subtree->parent()->left() = new_root;
            break;
        case NodeType::Direction::RIGHT:
            subtree->parent()->right() = new_root;
            break;
        default:
            m_root = new_root;
            break;
        }
        new_root->parent() = subtree->parent();

        subtree->left() = new_root->right();
        if (nullptr != subtree->left()) subtree->left()->parent() = subtree;

        new_root->right() = subtree;
        subtree->parent() = new_root;
    }
};

#endif