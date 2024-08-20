#ifndef BST_H
#define BST_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <queue>

/**
 *@brief 非递归写法的 BST
 */
class BST {
private:
    struct TreeNode {
        TreeNode* left;
        TreeNode* right;
        int count;
        int value;
        TreeNode() :left(nullptr), right(nullptr), count(0), value(0) {}
        TreeNode(int v) :left(nullptr), right(nullptr), count(1), value(v) {}
    };

    TreeNode* root;

    int nodeCount;  // 检查内存泄漏
public:
    BST();

    inline int getCount() { return nodeCount; }

    TreeNode* findNode(int value);

    void insertNode(int value);

    void removeNode(int value);

    bool checkSortTree();

    void clear();

    ~BST();

private:
    void deleteTree(TreeNode* root);

    // 从子树中摘下最小的节点(不论其计数多少), 当子树根节点为最小节点时会改变传入的子树指针
    // 如果子树只有一个元素, 返回其自身
    // 如果子树为空, 返回 nullptr
    TreeNode* cutMinimum(TreeNode** subtree);
};

#endif