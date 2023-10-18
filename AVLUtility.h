#ifndef _AVL_UTILITY_H_
#define _AVL_UTILITY_H_

namespace avl {
    template <typename T>
    void swap(T &first, T &second)
    {
        T temp = first;
        first = second;
        second = temp;
    }

    // assumes T has operator <
    template <typename T>
    inline T max(const T &first, const T &second) { return (first < second) ? (second) : (first); }
    
    enum class Comparison {
        less    = 0,
        equal   = 1,
        greater = 2
    };

    template <typename T>
    inline Comparison AVLTree_CompareUsingOperators(const T &left, const T &right)
    {
        if (left < right)
            return Comparison::less;
        else if (left > right)
            return Comparison::greater;
        else
            return Comparison::equal;
    }
};

#endif // _AVL_UTILITY_H_
