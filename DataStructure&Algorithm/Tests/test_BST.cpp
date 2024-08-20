#include "BST/BST.h"

int main()
{
    srand(time(NULL));
    int insertCount = 100;
    int valueMax = 100;

    std::vector<int> v;
    for (int i = 0;i < insertCount;i++) v.push_back(rand() % valueMax);

    BST bst;

    for (int i = 0;i < insertCount;i++) bst.insertNode(v[i]);

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
        bst.removeNode(v[i]);
    }

    if (nullptr != bst.findNode(-1)) std::cout << "error: find " << -1 << std::endl;
    if (nullptr != bst.findNode(valueMax)) std::cout << "error: find " << valueMax << std::endl;
}