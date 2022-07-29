//Sample by Fabio Galuppo - http://member.acm.org/~fabiogaluppo - fabiogaluppo@acm.org
//July 2022
//Inspired by Don Knuth's article on Algorithms for Scientific American in 1977
//This file is licensed to you under the MIT license

//cl /Fo.\obj\ /EHsc /O2 /std:c++20 program.cpp /link /out:program.exe
//g++ -Wall -O3 -std=c++20 program.cpp -o program.exe

//Windows:
//sort /+6 /r .\data\word_count.txt | program
//sort /+12 .\data\word_count.txt | program
//sort /+15 .\data\word_count.txt | program

//Unix, Linux:
//sort -nrk 2 ./data/word_count.txt | program
//sort -nk 3 ./data/word_count.txt | program
//sort -nk 4 ./data/word_count.txt | program

#include "utils.hpp"

void searching_computer_memory(std::vector<word_count> words);
void advantage_of_order(std::vector<word_count> words);
void binary_search_tree(std::vector<word_count> words);
void hashing(std::vector<word_count> words);
void improving_unsuccessful_searches(std::vector<word_count> words);

int main(int argc, char* argv[])
{
    std::vector<word_count> words = load_word_count_istream(std::cin);
    
    std::cout << '\n'
              << "Searching a Computer's Memory\n"
              << "-----------------------------\n";
    searching_computer_memory(words);
    
    std::cout << '\n'
              << "The Advantage of Order\n"
              << "----------------------\n";
    advantage_of_order(words);
    
    std::cout << '\n'
              << "Binary Tree Search\n"
              << "------------------\n";
    binary_search_tree(words);
    
    std::cout << '\n'
              << "Hashing\n"
              << "-------\n";
    hashing(words);
    
    std::cout << '\n'
              << "Improving Unsuccessful Searches\n"
              << "-------------------------------\n";
    improving_unsuccessful_searches(words);
}

void searching_computer_memory(std::vector<word_count> words)
{
    display(words);

    for (auto target : { "THAT", "THIS", "THESE", "THOSE", "YOU", "YOUTH" })
    {
        auto i = sequencial_search(words, target);
        std::cout << target;
        if (i != NOT_FOUND)
            std::cout << " found: " << std::to_string(words[i]) << " at " << i << '\n';
        else 
            std::cout << " not found\n";
    }
}

void advantage_of_order(std::vector<word_count> words)
{
    words = order_by_word(words);
    display(words);

    for (auto target : { "THAT", "THIS", "THESE", "THOSE", "YOU", "YOUTH" })
    {
        auto i = binary_search(words, target);
        std::cout << target;
        if (i != NOT_FOUND)
            std::cout << " found: " << std::to_string(words[i]) << " at " << i << '\n';
        else 
            std::cout << " not found\n";
    }
}

void binary_search_tree(std::vector<word_count> words)
{
    std::unique_ptr<bst_node> root = bst_build(words);
    display(root.get());
    
    for (auto target : { "THAT", "THIS", "THESE", "THOSE", "YOU", "YOUTH" })
    {
        auto p = bst_search_recursive(root.get(), target);
        std::cout << target;
        if (p)
            std::cout << " found: " << std::to_string(*p->data) << '\n';
        else 
            std::cout << " not found\n";
    }

    std::cout << to_graphviz_string(root.get());
}

void hashing(std::vector<word_count> words)
{
    ht_t ht = ht_build(words);
    display(ht);

    for (auto target : { "THAT", "THIS", "THESE", "THOSE", "YOU", "YOUTH" })
    {
        auto i = ht_search(ht, target);
        std::cout << target;
        if (i != NOT_FOUND)
            std::cout << " found: " << std::to_string(*ht.buckets[i]) << " at bucket " << i << '\n';
        else 
            std::cout << " not found\n";
    }
}

void improving_unsuccessful_searches(std::vector<word_count> words)
{
    words = order_by_word(words);
    ht_t ht = ht_build_2(words);
    display(ht);

    for (auto target : { "THAT", "THIS", "THESE", "THOSE", "YOU", "YOUTH" })
    {
        auto i = ht_search_2(ht, target);
        std::cout << target;
        if (i != NOT_FOUND)
            std::cout << " found: " << std::to_string(*ht.buckets[i]) << " at bucket " << i << '\n';
        else 
            std::cout << " not found\n";
    }
}

//////////////////////////////////
// Algorithms implementation
//////////////////////////////////

//Searching a Computer's Memory

std::size_t sequencial_search(const std::vector<word_count>& words, const std::string& target)
{
    std::size_t N = words.size();
    while (N--)
    {
        if (words[N].word == target)
            return N;
    }
    return NOT_FOUND;
}

//The Advantage of Order

std::size_t binary_search(const std::vector<word_count>& words, const std::string& target)
{
    std::size_t l = 0, r = words.size(), mid;
    while (l != r)
    {
        //mid = (r + l) / 2; //overflow case
        mid = l + (r - l) / 2; //no overflow case
        int comp = target.compare(words[mid].word);
        if (comp == 0)
            return mid;
        else if (comp < 0)
            r = mid;
        else /* if (comp > 0) */
            l = mid + 1;
    }
    return NOT_FOUND;
}

//Binary Tree Search

std::unique_ptr<bst_node> bst_insert_recursive(std::unique_ptr<bst_node> node, std::vector<word_count>::const_iterator data)
{
    if (!node)
        return std::make_unique<bst_node>(data);
    if (data->word < node->data->word)
        node->left = bst_insert_recursive(std::move(node->left), data);
    else
        node->right = bst_insert_recursive(std::move(node->right), data);
    return node;
}

std::unique_ptr<bst_node> bst_build(const std::vector<word_count>& words)
{
    std::unique_ptr<bst_node> root;
    for (auto iter = words.cbegin(); iter != words.cend(); ++iter)
        root = bst_insert_recursive(std::move(root), iter);
    return root;
}

const bst_node* bst_search_recursive(const bst_node* node, const std::string& target)
{
    if (!node)
        return nullptr;
    int comp = target.compare(node->data->word);
    if (comp == 0)
        return node;
    return bst_search_recursive((comp < 0 ? node->left : node->right).get(), target);
}

//Hashing

std::size_t hash(const std::string& word, std::size_t bucket_size)
{
    std::size_t h = 0;
    for (char ch : word)
        h += (ch - 'A' + 1);		
    return --h % bucket_size;
}

void ht_insert(ht_t& ht, std::vector<word_count>::const_iterator data)
{
    const std::size_t bucket_size = ht.buckets.size();
    std::size_t h = hash(data->word, bucket_size);
    while (ht.buckets[h] != ht.end)
        if (h-- == 0)
            h = bucket_size - 1;
    ht.buckets[h] = data;
}

ht_t ht_build(const std::vector<word_count>& words)
{
    ht_t ht{ std::vector<std::vector<word_count>::const_iterator>(1 + words.size(), words.cend()), words.cend() };
    for (auto iter = words.cbegin(); iter != ht.end; ++iter)
        ht_insert(ht, iter);
    return ht;
}

std::size_t ht_search(const ht_t& ht, const std::string& target)
{
    const std::size_t bucket_size = ht.buckets.size();
    std::size_t h = hash(target, bucket_size);
    while (ht.buckets[h] != ht.end)
    {
        if (ht.buckets[h]->word == target)
            return h;
        if (h-- == 0)
            h = bucket_size - 1;
    }
    return NOT_FOUND;
}

//Improving Unsuccessful Searches

void ht_insert_2(ht_t& ht, std::vector<word_count>::const_iterator data)
{
    const std::size_t bucket_size = ht.buckets.size();
    std::size_t h = hash(data->word, bucket_size);
    while (ht.buckets[h] != ht.end)
    {
        if (data->word.compare(ht.buckets[h]->word) > 0)
            std::swap(ht.buckets[h], data);
        if (h-- == 0)
            h = bucket_size - 1;
    }
    ht.buckets[h] = data;
}

ht_t ht_build_2(const std::vector<word_count>& words)
{
    ht_t ht{ std::vector<std::vector<word_count>::const_iterator>(1 + words.size(), words.cend()), words.cend() };
    for (auto iter = words.cbegin(); iter != ht.end; ++iter)
        ht_insert_2(ht, iter);
    return ht;
}

std::size_t ht_search_2(const ht_t& ht, const std::string& target)
{
    const std::size_t bucket_size = ht.buckets.size();
    std::size_t h = hash(target, bucket_size);
    while (ht.buckets[h] != ht.end)
    {
        int comp = target.compare(ht.buckets[h]->word);
        if (comp > 0)
            break;
        if (comp == 0)
            return h;
        if (h-- == 0)
            h = bucket_size - 1;
    }
    return NOT_FOUND;
}