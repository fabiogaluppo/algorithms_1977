//Sample by Fabio Galuppo - http://member.acm.org/~fabiogaluppo - fabiogaluppo@acm.org
//July 2022
//Inspired by Don Knuth's article on Algorithms for Scientific American in 1977
//This file is licensed to you under the MIT license

#include <cstddef>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>

struct word_count
{
    std::string word;
    std::size_t count;
};

static inline std::vector<word_count> load_word_count_istream(std::istream& is)
{
    std::vector<word_count> words;
    std::string word; std::size_t count, hint; //hint ignored
    while (is >> word >> count >> hint >> hint)
        words.emplace_back(std::move(word), count);
    return words;
}

namespace std
{
    std::string to_string(const word_count& w)
    {
        return "[" + w.word + ", " + std::to_string(w.count) + ']';
    }
}

const std::size_t NOT_FOUND = -1;

std::size_t sequencial_search(const std::vector<word_count>& words, const std::string& target);
std::size_t binary_search(const std::vector<word_count>& words, const std::string& target);
struct bst_node
{
    std::vector<word_count>::const_iterator data;
    std::unique_ptr<bst_node> left = nullptr;
    std::unique_ptr<bst_node> right = nullptr;
};
std::unique_ptr<bst_node> bst_build(const std::vector<word_count>& words);
const bst_node* bst_search_recursive(const bst_node* node, const std::string& target);
struct ht_t
{
    std::vector<std::vector<word_count>::const_iterator> buckets;
    std::vector<word_count>::const_iterator end;
};
ht_t ht_build(const std::vector<word_count>& words);
std::size_t ht_search(const ht_t& ht, const std::string& target);
ht_t ht_build_2(const std::vector<word_count>& words);
std::size_t ht_search_2(const ht_t& ht, const std::string& target);

void display(const std::vector<word_count>& words)
{
    int i = 0;
    for (const auto& w : words)
        std::cout << i++ << ":[" << w.word << ", " << w.count << "] ";
    std::cout << "\n\n";
}

#include <sstream>

std::string to_level_order_string(const bst_node* node)
{
    std::stringstream ss;
    std::queue<const bst_node*> q;
    q.push(node);
    int i = 0;
    while (!q.empty())
    {
        const bst_node* node = q.front();
        q.pop();
        ss << i++ << ":[";
        if (node)
        {
            ss << node->data->word << ", " << node->data->count;
            if (node->left) q.push(node->left.get());
            if (node->right) q.push(node->right.get());
        }
        ss <<  "] ";
    }
   return ss.str();
}

void display(const bst_node* node)
{
    std::cout << to_level_order_string(node) << "\n\n";
}

std::string to_graphviz_string(const bst_node* node)
{
    std::stringstream ss;
    ss << '\n' << "digraph BINARY_SEARCH_TREE {\n";
    ss << "\t//";
    ss << to_level_order_string(node) << '\n';
    std::queue<const bst_node*> q;
    q.push(node);    
    while (!q.empty())
    {
        const bst_node* node = q.front();
        q.pop();
        if (node)
        {
            if (node->left)
            {
                ss << '\t' << node->data->word << " -> " << node->left->data->word << ";\n";
                q.push(node->left.get());
            }
            if (node->right)
            {
                ss << '\t' << node->data->word << " -> " << node->right->data->word << ";\n";
                q.push(node->right.get());
            }            
        }
    }
    ss << "}\n";
    return ss.str();
}

void display(const ht_t& ht)
{
    int i = 0;
    for (const auto& w : ht.buckets)
    {
        std::cout << i++ << ":[";
        if (w != ht.end)
            std::cout << w->word << ", " << w->count;
        std::cout <<  "] ";
    }
    std::cout << "\n\n";
}

std::vector<word_count> order_by_word(std::vector<word_count> words)
{
    std::sort(words.begin(), words.end(), [](auto& lhs, auto& rhs) {
        return lhs.word < rhs.word;
    });
    return words;
}