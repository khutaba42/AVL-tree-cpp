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

    tree_1.display();

    std::cout << tree_1.size() << std::endl;
    
    tree_1.remove('s');

    tree_1.display();
    
    std::cout << tree_1.size() << std::endl;

    std::cout << tree_1.isEmpty() << std::endl;

    tree_1.clear();

    std::cout << tree_1.size() << std::endl;
    
    std::cout << tree_1.isEmpty() << std::endl;

    return 0;
}
