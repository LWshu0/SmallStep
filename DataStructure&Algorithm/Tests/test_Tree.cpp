#include "Tree/BST.hpp"
#include "Tree/AVL.hpp"
#include "Tree/rbTree.hpp"

#include "Tree/TreeView.hpp"

int main()
{
    srand(time(NULL));
    int insertCount = 10;
    int valueMax = 100;

    std::vector<int> v;
    for (int i = 0;i < insertCount;i++) v.push_back(rand() % valueMax);

    rbTree bst;
    rbTreeView treeview;
    
    for (int i = 0;i < insertCount;i++)
    {
        std::cout << "insert " << v[i] << std::endl;
        bst.insertNode(v[i]);
        treeview(bst.root(), 3);
    }
    
    std::cout << "new node count: " << bst.getCount() << std::endl;

    if (bst.checkSortTree()) std::cout << "check sort correct" << std::endl;
    else std::cout << "error: check sort wrong" << std::endl;

    for (int i = 0;i < insertCount;i++)
    {
        if (nullptr == bst.findNode(v[i]))
        {
            std::cout << "error: not find " << v[i] << std::endl;
        }
    }

    for (int i = 0;i < insertCount;i++)
    {
        std::cout << "remove " << v[i] << std::endl;
        bst.removeNode(v[i]);
        treeview(bst.root(), 3);
    }

    if (nullptr != bst.findNode(-1)) std::cout << "error: find " << -1 << std::endl;
    if (nullptr != bst.findNode(valueMax)) std::cout << "error: find " << valueMax << std::endl;

    // std::cout << "height: " << bst.root()->getHeight() << std::endl;
    
}