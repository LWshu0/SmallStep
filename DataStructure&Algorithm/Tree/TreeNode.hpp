#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include <string>

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

#endif
