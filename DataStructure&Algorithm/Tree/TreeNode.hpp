#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include <string>
#include <cassert>

// 最小树节点结构
template<typename T>
class TreeNode {
private:
    TreeNode* m_left;
    TreeNode* m_right;
    T m_value;
    int m_count;

public:
    TreeNode(T v) :m_left(nullptr), m_right(nullptr), m_value(v), m_count(1) {}
    inline TreeNode*& left() { return m_left; }
    inline TreeNode*& right() { return m_right; }
    inline T& value() { return m_value; }
    inline int& count() { return m_count; }

    int getHeight()
    {
        return _getHeight(this);
    }

    ~TreeNode() {}

private:

    int _getHeight(TreeNode* node)
    {
        if (nullptr == node) return 0;
        int left_height = _getHeight(node->left());
        int right_height = _getHeight(node->right());
        return (left_height > right_height ? left_height : right_height) + 1;
    }
};

template<typename T>
class AVLNode {
private:
    AVLNode* m_left;
    AVLNode* m_right;
    T m_value;
    int m_count;
    int m_height;

public:
    AVLNode(T v) : m_left(nullptr), m_right(nullptr), m_value(v), m_count(1), m_height(1) {}
    inline AVLNode*& left() { return m_left; }
    inline AVLNode*& right() { return m_right; }
    inline T& value() { return m_value; }
    inline int& count() { return m_count; }
    inline int& height() { return m_height; }

    int getHeight()
    {
        return m_height;
    }

    ~AVLNode() {}
};

template<typename T>
class rbTreeNode {
public:
    enum Direction { LEFT = -1, ROOT = 0, RIGHT = 1 };
    enum rbTreeNodeColor { BLACK, RED };
private:
    rbTreeNode* m_parent;
    rbTreeNode* m_left;
    rbTreeNode* m_right;
    T m_value;
    int m_count;
    rbTreeNodeColor m_color;
    
public:
    rbTreeNode(T v) :
        m_parent(nullptr),
        m_left(nullptr),
        m_right(nullptr),
        m_value(v),
        m_count(1),
        m_color(RED)
    {}

    inline rbTreeNode*& parent() { return m_parent; }
    inline rbTreeNode*& left() { return m_left; }
    inline rbTreeNode*& right() { return m_right; }
    inline T& value() { return m_value; }
    inline int& count() { return m_count; }
    inline rbTreeNodeColor& color() { return m_color; }

    inline Direction direction()
    {
        if (this->m_parent != nullptr)
        {
            return (this == this->m_parent->m_left) ? Direction::LEFT : Direction::RIGHT;
        }
        else
        {
            return Direction::ROOT;
        }
    }
    inline rbTreeNode* uncle()
    {
        assert(hasGrandparent());
        switch (parent()->direction())
        {
        case Direction::LEFT:
            return parent()->parent()->right();
        case Direction::RIGHT:
            return parent()->parent()->left();
        default:
            return nullptr;
        }
    }

    inline void setRed() { m_color = RED; }
    inline void setBlack() { m_color = BLACK; }
    inline bool isRed() { return RED == m_color; }
    inline bool isBlack() { return BLACK == m_color; }
    inline bool isRoot() { return nullptr == m_parent; }
    inline bool isLeftSon() { return Direction::LEFT == direction(); }
    inline bool isRightSon() { return Direction::RIGHT == direction(); }
    inline bool hasParent() { return nullptr != m_parent; }
    inline bool hasGrandparent() { return hasParent() && m_parent->hasParent(); }

    int getHeight()
    {
        return _getHeight(this);
    }

    ~rbTreeNode() {}
private:
    int _getHeight(rbTreeNode* node)
    {
        if (nullptr == node) return 0;
        int left_height = _getHeight(node->left());
        int right_height = _getHeight(node->right());
        return (left_height > right_height ? left_height : right_height) + 1;
    }
};

#endif
