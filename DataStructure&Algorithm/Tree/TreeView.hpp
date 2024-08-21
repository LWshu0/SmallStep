#ifndef TREEVIEW_HPP
#define TREEVIEW_HPP

#include <vector>
#include <string>
#include <iostream>

// 要求 left(), right(), value()
// value() 可以使用 to_string() 方法转为字符串
class TreeView {
public:
    TreeView() = default;

    template <typename T>
    void operator()(T* root, int unit_width)
    {
        if (nullptr == root) return;
        const int bottom_unit_count = 1 << (root->getHeight() - 1);    // 2^(h-1)
        int node_unit_count = bottom_unit_count;
        int node_char_count = node_unit_count * unit_width + node_unit_count - 1;
        int node_count = 1;

        std::vector<T*> layer;
        layer.push_back(root);
        // print root
        centerPrint(std::to_string(root->value()), node_char_count);
        std::cout << std::endl;

        // print other (if exist)
        while (node_unit_count > 1)
        {
            // print marker
            std::vector<T*> next_layer;
            for (auto& node : layer)
            {
                if (node)
                {
                    next_layer.push_back(node->left());
                    next_layer.push_back(node->right());
                    markerPrint(node_char_count, node->left(), node->right());
                    std::cout << ' ';
                }
                else
                {
                    next_layer.push_back(nullptr);
                    next_layer.push_back(nullptr);
                    markerPrint(node_char_count, false, false);
                    std::cout << ' ';
                }
            }
            std::cout << std::endl;

            // modify unit to next layer
            node_unit_count /= 2;
            node_char_count = node_unit_count * unit_width + node_unit_count - 1;
            node_count *= 2;
            // print next layer value
            for (auto& node : next_layer)
            {
                if (node) centerPrint(std::to_string(node->value()), node_char_count);
                else std::cout << std::string(node_char_count, ' ');
                std::cout << ' ';
            }
            std::cout << std::endl;

            layer = std::move(next_layer);
        }
    }

    ~TreeView() = default;

private:
    void centerPrint(const std::string& str, int total_width)
    {
        if (str.length() > total_width)
        {
            std::cout << std::string(total_width, '#');
            return;
        }

        int pre_space_length = (total_width - str.length()) / 2;
        int post_space_length = total_width - str.length() - pre_space_length;

        std::cout << std::string(pre_space_length, ' ')
            << str
            << std::string(post_space_length, ' ');
    }

    std::string repeatString(const std::string& str, size_t n)
    {
        std::string sstr;
        sstr.reserve(str.size() * n);
        while (n--) sstr += str;
        return sstr;
    }

    void markerPrint(int total_width, bool enable_left, bool enable_right)
    {
        int space_length = (total_width - 3) / 4;

        std::cout << std::string(space_length, ' ');

        if (enable_left) std::cout << "┌" << repeatString("─", space_length);
        else std::cout << std::string(1 + space_length, ' ');

        if (enable_left && enable_right) std::cout << "┴";
        else if (!enable_left && enable_right) std::cout << "└";
        else if (enable_left && !enable_right) std::cout << "┘";
        else std::cout << " ";

        if (enable_right) std::cout << repeatString("─", space_length) << "┐";
        else std::cout << std::string(space_length + 1, ' ');

        std::cout << std::string(space_length, ' ');
    }
};

// template<typename T>
// class TreeViewHorizontal {
//     // 
//     void print()
//     {

//         std::vector<std::string> prefix;
//         _print(this, prefix, 0);
//     }

//     void _print(T* node, std::vector<std::string>& prefix, int depth)
//     {
//         if (node->right())
//         {
//             prefix.push_back("┌──");
//             _print(node->right(), prefix, depth + 1);
//             prefix.pop_back();
//         }

//         prefix.back() = "";
//         for (auto& str : prefix)
//         {
//             std::cout << str;
//         }
//         std::cout << node->value() << std::endl;
//         if (node->left())
//         {
//             prefix.push_back("└──");
//             _print(node->left(), prefix, depth + 1);
//             prefix.pop_back();
//         }
//     }
// };

#endif