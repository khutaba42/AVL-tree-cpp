#include <iostream>
#include "AVLTree.h"

/*
g++ -std=c++11 example.cpp -o example.exe -lc++
*/

avl::Comparison comp(const char& a, const char& b) {
    if (a < b) {
        return avl::Comparison::less;
    }
    else if(a > b) {
        return avl::Comparison::greater;
    }
    return avl::Comparison::equal;
}

int main(int argc, char* argv[]) {
    avl::Tree<char, comp> tree_1;

    for(int letter = (int) 'a'; letter <= (int) 'z' ; ++letter)
    {
        tree_1.insert(letter);
    }

    std::cout << "Tree of the whole alphabet" << std::endl;
    tree_1.display();

    std::cout << "size: " << tree_1.size() << std::endl;

    tree_1.remove('a');
    tree_1.remove('v');
    tree_1.remove('l');
    tree_1.remove('t');
    tree_1.remove('r');
    tree_1.remove('e');

    try
    {
        tree_1.remove('e');
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "Tree of the whole alphabet without a v l t r e" << std::endl;
    tree_1.display();
    
    std::cout << "size: " << tree_1.size() << std::endl;

    (tree_1.isEmpty()) ? (std::cout <<  "empty") : (std::cout <<  "not empty") << std::endl;

    tree_1.clear();

    std::cout << "size: " << tree_1.size() << std::endl;
    
    (tree_1.isEmpty()) ? (std::cout <<  "empty") : (std::cout <<  "not empty") << std::endl;

    return 0;
}
