#include "BST.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <queue>

BST::BST()
{
    root = nullptr;
    nodeCount = 0;
}

BST::TreeNode* BST::findNode(int value)
{
    TreeNode* search_ptr = root;
    while (search_ptr)
    {
        if (value < search_ptr->value) search_ptr = search_ptr->left;
        else if (value > search_ptr->value) search_ptr = search_ptr->right;
        else return search_ptr;
    }
    return nullptr;
}

void BST::insertNode(int value)
{
    if (nullptr == root)
    {
        root = new TreeNode(value);
        nodeCount += 1;
    }
    else
    {
        TreeNode* search_ptr = root;
        while (search_ptr)
        {
            if (search_ptr->value < value)
            {
                if (nullptr == search_ptr->right)
                {
                    search_ptr->right = new TreeNode(value);
                    nodeCount += 1;
                    break;
                }
                else search_ptr = search_ptr->right;
            }
            else if (search_ptr->value > value)
            {
                if (nullptr == search_ptr->left)
                {
                    search_ptr->left = new TreeNode(value);
                    nodeCount += 1;
                    break;
                }
                else search_ptr = search_ptr->left;
            }
            else
            {
                search_ptr->count += 1;
                break;
            }
        }
    }
}

void BST::removeNode(int value)
{
    if (nullptr == root) return;
    TreeNode** pre_search_ptr = &root;
    TreeNode* search_ptr = root;
    // 找到待删除元素
    while (search_ptr)
    {
        if (value < search_ptr->value)
        {
            pre_search_ptr = &(search_ptr->left);
            search_ptr = search_ptr->left;
        }
        else if (value > search_ptr->value)
        {
            pre_search_ptr = &(search_ptr->right);
            search_ptr = search_ptr->right;
        }
        else break;
    }

    if (nullptr == search_ptr) return;  // 树中不存在待删除的元素

    if (search_ptr->count > 1)
    {
        search_ptr->count--;
        return;
    }

    if (nullptr == search_ptr->left)    // 待删除节点左子树为空(右子树可能为空, 也可能不空)
    {
        *pre_search_ptr = search_ptr->right;
        delete search_ptr;
        nodeCount--;
    }
    else if (nullptr == search_ptr->right)  // 待删除节点右子树为空(左子树不空)
    {
        *pre_search_ptr = search_ptr->left;
        delete search_ptr;
        nodeCount--;
    }
    else  // 待删除节点左右子树都不空
    {
        // 得到右子树的最小节点
        TreeNode* min_node = cutMinimum(&search_ptr->right);
        min_node->left = search_ptr->left;
        min_node->right = search_ptr->right;
        *pre_search_ptr = min_node;
        delete search_ptr;
        nodeCount--;
    }
}

bool BST::checkSortTree()
{
    std::queue<TreeNode*> bfs;
    bfs.push(root);
    while (!bfs.empty())
    {
        TreeNode* this_root = bfs.front();
        if (nullptr != this_root->left)
        {
            if (this_root->left->value > this_root->value) return false;
            bfs.push(this_root->left);
        }
        if (nullptr != this_root->right)
        {
            if (this_root->right->value < this_root->value) return false;
            bfs.push(this_root->right);
        }
        bfs.pop();
    }
    return true;
}

void BST::clear()
{
    deleteTree(root);
    root = nullptr;
}

BST::~BST()
{
    // deleteTree(root);
    std::cout << "undeleted node count: " << nodeCount << std::endl;
}


void BST::deleteTree(TreeNode* root)
{
    if (nullptr == root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
    nodeCount -= 1;
}

// 从子树中摘下最小的节点(不论其计数多少), 当子树根节点为最小节点时会改变传入的子树指针
// 如果子树只有一个元素, 返回其自身
// 如果子树为空, 返回 nullptr
BST::TreeNode* BST::cutMinimum(BST::TreeNode** subtree)
{
    // 传入子树为空, 返回 nullptr
    if (nullptr == *subtree) return nullptr;
    TreeNode** pre_search_ptr = subtree;
    TreeNode* search_ptr = *subtree;
    // 找到最小元素节点
    while (search_ptr->left)
    {
        pre_search_ptr = &(search_ptr->left);
        search_ptr = search_ptr->left;
    }
    // 将最小节点的右子树接到其父节点的 left 处
    *pre_search_ptr = search_ptr->right;
    search_ptr->right = nullptr;
    return search_ptr;
}
