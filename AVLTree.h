#ifndef _AVL_TREE_CPP_H_
#define _AVL_TREE_CPP_H_

#include <iostream> // for the errors and to display the tree
#include <cassert>

#include "Stack.h"
#include "AVLUtility.h"

namespace avl
{
    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &) = AVLTree_CompareUsingOperators<DATA_t>>
    class Tree
    {
    public:
        Tree();
        ~Tree();

        void insert(const DATA_t &data);
        void remove(const DATA_t &data);

        void clear();
        const bool isEmpty() const;
        const int size() const;

        const DATA_t &find(const DATA_t &data) const;
        const DATA_t &getMin() const;
        const DATA_t &getMax() const;

        template <typename FunctionObject>
        void in_order_traversal(FunctionObject do_something)
        {
            in_order_traversal_aux_recursive(__root, do_something);
        }

        template <typename FunctionObject>
        void reverse_in_order_traversal(FunctionObject do_something)
        {
            reverse_in_order_traversal_aux_recursive(__root, do_something);
        }

        void display();

        // error classes
        class NoSuchElementException : public std::exception
        {
        public:
            const char *what() const noexcept override { return "There is no such element"; }
        };
        class ElementAlreadyExistsException : public std::exception
        {
        public:
            const char *what() const noexcept override { return "Element already exists"; }
        };

        struct Node
        {
            DATA_t __data;
            Node *__left, *__right;
            int __height;

            Node(const DATA_t &data) : __data(data),
                                       __left(nullptr),
                                       __right(nullptr),
                                       __height(0)
            {
            }

            void updateValues()
            {
                __height = 1 + max(__left != nullptr ? __left->__height : -1,
                                   __right != nullptr ? __right->__height : -1);
            }

            int balanceFactor()
            {
                return (__left != nullptr ? __left->__height : -1) - (__right != nullptr ? __right->__height : -1);
            }

            bool isLeaf() const
            {
                return ((__left == nullptr) && (__right == nullptr));
            }

            bool hasLeft() const
            {
                return __left != nullptr;
            }

            bool hasRight() const
            {
                return __right != nullptr;
            }

            // returns a pointer to the node that "replaced" the previous after rotation
            Node *left_rotate()
            {
                Node *R = __right;
                __right = __right->__left;
                R->__left = this;

                this->updateValues(); // the order is important
                R->updateValues();

                return R;
            }

            // returns a pointer to the node that "replaced" the previous after rotation
            Node *right_rotate()
            {
                Node *L = __left;
                __left = __left->__right;
                L->__right = this;

                this->updateValues(); // the order is important
                L->updateValues();

                return L;
            }

            /*friend*/ void swapNodes(Node *&first, Node *&second)
            {
                if ((first == nullptr) || (second == nullptr) || (/*self swap bug*/ first == second))
                {
                    return;
                }
                swap(first->__left, second->__left);     // switch lefts
                swap(first->__right, second->__right);   // switch rights
                swap(first->__height, second->__height); // switch heights
                swap(first, second);                     // switch places (parents in tree)
            }
        };

    private:
        Node *__root;
        Node *__min_element;
        Node *__max_element;
        int __size;

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
           +---------------------------------------------------------------+
           |                                                               |
           |                         Tree Helper                           |
           |                                                               |
           +---------------------------------------------------------------+
        <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

        // deletes the tree with a pointer to the root, return teh size of tree it deleted
        int clear_aux(Node *root)
        {
            if (root == nullptr)
            {
                return 0;
            }
            int size = 1 + clear_aux(root->__left) + clear_aux(root->__right);
            delete root;
            return size;
        }

        // return ????????
        Node **find_node(const DATA_t &data)
        {
            Node **temp = &__root;
            while (*temp != nullptr)
            {
                Comparison result = ComparisonFunc(data, (*temp)->__data);
                if (result == Comparison::less)
                {
                    temp = &((*temp)->__left);
                }
                else if (result == Comparison::greater)
                {
                    temp = &((*temp)->__right);
                }
                else
                {
                    return temp;
                }
            }
            throw NoSuchElementException();
        }

        DATA_t &find_aux(const DATA_t &data)
        {
            Node *temp = *find_node(data);
            if (temp == nullptr)
            {
                throw NoSuchElementException();
            }
            return temp->__data;
        }

        Stack<Node *&> &find_path(const DATA_t &data, Stack<Node *&> &empty_path)
        {
            Node **temp = &__root;
            while ((*temp) != nullptr)
            {
                empty_path.push_back(*temp);
                Comparison result = ComparisonFunc(data, (*temp)->__data);
                if (result == Comparison::less)
                {
                    temp = &((*temp)->__left);
                }
                else if (result == Comparison::greater)
                {
                    temp = &((*temp)->__right);
                }
                else
                {
                    break;
                }
            }
            if ((*temp) == nullptr)
            {
                empty_path.push_back(*temp);
            }

            return empty_path;
        }

        // return a pointer the node holding the min data
        Node *findMin() const
        {
            Node *temp = __root;
            if (temp == nullptr)
                throw NoSuchElementException();
            while (temp->__left != nullptr)
            {
                temp = temp->__left;
            }
            return temp;
        }

        // return a pointer the node holding the max data
        Node *findMax() const
        {
            Node *temp = __root;
            if (temp == nullptr)
                throw NoSuchElementException();
            while (temp->__right != nullptr)
            {
                temp = temp->__right;
            }
            return temp;
        }

        void balance(Stack<Node *&> &path)
        {
            // std::cout << "before balancing\n";
            // display();
            Node *curr = nullptr;
            while (!path.isEmpty())
            {
                // display();
                curr = path.back();
                // assert(curr != nullptr);
                // if (curr == nullptr)
                //{
                //    path.pop_back();
                //    continue;
                //}
                if (curr == nullptr)
                {

                    path.pop_back();
                    continue;
                }

                Node *&curr_reference = path.back();
                path.pop_back();
                // update height
                curr->updateValues();

                if (curr->balanceFactor() >= 2 && curr->__left->balanceFactor() >= 0)
                { // left - left
                    curr_reference = curr->right_rotate();
                }
                else if (curr->balanceFactor() >= 2)
                { // left - right
                    curr_reference->__left = curr->__left->left_rotate();
                    curr_reference = curr->right_rotate();
                }

                else if (curr->balanceFactor() <= -2 and curr->__right->balanceFactor() <= 0)
                { // right - right
                    curr_reference = curr->left_rotate();
                }
                else if (curr->balanceFactor() <= -2)
                { // right - left
                    curr_reference->__right = curr->__right->right_rotate();
                    curr_reference = curr->left_rotate();
                }
            }
            // std::cout << "after balancing\n";
            // display();
        }
        bool insert_aux(const DATA_t &data)
        {
            Stack<Node *&> path;
            find_path(data, path);
            if (path.back() != nullptr)
            { // duplicate
                return false;
            }
            path.back() = new Node(data); // beware of bad_alloc
            path.pop_back();              // new inserted node dont need balancing
            // balance path
            balance(path /*, true*/);
            return true;
        }

        bool remove_aux(const DATA_t &data)
        {
            Stack<Node *&> path;
            find_path(data, path);
            if (path.isEmpty() || path.back() == nullptr)
            {
                return false;
            }
            // path NOT empty and back is VALID
            Node *curr = path.back();
            assert(curr != nullptr);
            if (curr->isLeaf())
            {
                delete curr;
                path.back() = nullptr;
                path.pop_back();
            }
            else if (curr->hasLeft() && !curr->hasRight())
            {
                path.back() = curr->__left;
                delete curr;
                path.pop_back();
            }
            else if (!curr->hasLeft() && curr->hasRight())
            {
                path.back() = curr->__right;
                delete curr;
                path.pop_back();
            }
            else
            { // (curr->hasLeft() && curr->hasRight())

                Stack<Node *&> path_to_successor;
                // find successor
                Node **successor = &curr->__right; // go right once
                while ((*successor) != nullptr)    // go all the way to the left
                {
                    path_to_successor.push_back(*successor);
                    successor = &((*successor)->__left);
                }

                if (path_to_successor.isEmpty())
                {
                    Node *toDelete = path.back();
                    path.back() = (*successor);
                    delete toDelete;
                }
                // successor DOESN'T have left by definition
                // else if (path.back() == curr->__right)
                //{
                //    path.back()->__left = curr->__left;
                //    path.back()->updateValues();
                //    path[index_of_pointer_to_middle_node] = path.back();
                //    delete curr;
                //}
                else
                {
                    // swap(path[index_of_pointer_to_middle_node]->__left,    path.back()->__left);     // switch lefts
                    // swap(path[index_of_pointer_to_middle_node]->__right,   path.back()->__right);   // switch rights
                    // swap(path[index_of_pointer_to_middle_node]->__height,  path.back()->__height); // switch heights
                    // swap(path[index_of_pointer_to_middle_node],            path.back());
                    swap(path.back()->__data, path_to_successor.back()->__data);
                    if (path_to_successor.back()->isLeaf())
                    {
                        delete path_to_successor.back();
                        path_to_successor.back() = nullptr;
                        path_to_successor.pop_back();
                    }
                    else
                    {
                        Node *toDelete = path_to_successor.back();
                        path_to_successor.back() = path_to_successor.back()->__right;
                        delete toDelete;
                        path_to_successor.pop_back();
                    }
                }
                balance(path_to_successor /*, false */);
            }
            // balance path
            balance(path /*, false */);

            return true;
        }

        template <typename FunctionObject>
        bool in_order_traversal_aux_recursive(Node *const root, FunctionObject do_something) const
        {
            if (root == nullptr)
            {
                return false;
            }
            in_order_traversal_aux_recursive(root->__left, do_something);
            do_something(root->__data);
            in_order_traversal_aux_recursive(root->__right, do_something);
            return true;
        }

        template <typename FunctionObject>
        bool reverse_in_order_traversal_aux_recursive(Node *const root, FunctionObject do_something) const
        {
            if (root == nullptr)
            {
                return false;
            }
            reverse_in_order_traversal_aux_recursive(root->__right, do_something);
            do_something(root->__data);
            reverse_in_order_traversal_aux_recursive(root->__left, do_something);
            return true;
        }

        void display_aux(Node *cur, int depth = 0, short nodeType = 0)
        {   // nodeType == 0 is root
            // nodeType == 1 is left
            // nodeType == 2 is right
            if (cur->__left) {
                display_aux(cur->__left, depth + 1, 1);
            }

            for (int i = 0; i < depth; i++) // padding
            {
                printf("     ");
            }

            if (nodeType == 1) // left
            {
                printf("┌---"); // or use "┌───" for a more smooth look
            }
            else if (nodeType == 2) // right
            {
                printf("└---"); // or use "└───" for a more smooth look
            }
            else // root
            {
                printf("*---");
            }
                

            // if you'd like to print hte height as well comment out the below comment
            std::cout << "[" << cur->__data /* << ",  " << cur->__height */ << "]" << std::endl;

            if (cur->__right)
            {
                display_aux(cur->__right, depth + 1, 2);
            }
        }
    };

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    Tree<DATA_t, ComparisonFunc>::Tree() : __root(nullptr), __min_element(nullptr), __max_element(nullptr), __size(0)
    {
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    Tree<DATA_t, ComparisonFunc>::~Tree()
    {
        clear();
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    void Tree<DATA_t, ComparisonFunc>::insert(const DATA_t &data)
    {
        if (insert_aux(data)) // insert successful
        {
            __size++;
            __min_element = findMin();
            __max_element = findMax();
        }
        else
        {
            throw ElementAlreadyExistsException();
        }
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    void Tree<DATA_t, ComparisonFunc>::remove(const DATA_t &data)
    {
        if (remove_aux(data)) // deletion successful
        {
            __size--;
            if (isEmpty())
            {
                __min_element = nullptr;
                __max_element = nullptr;
            }
            else
            {
                __min_element = findMin();
                __max_element = findMax();
            }
        }
        else
        {
            throw NoSuchElementException();
        }
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    void Tree<DATA_t, ComparisonFunc>::clear()
    {
        __size -= clear_aux(__root);
        assert(__size == 0);
        __root = nullptr;
        __min_element = nullptr;
        __max_element = nullptr;
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    const bool Tree<DATA_t, ComparisonFunc>::isEmpty() const
    {
        return (__root == nullptr);
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    const int Tree<DATA_t, ComparisonFunc>::size() const
    {
        return __size;
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    const DATA_t &Tree<DATA_t, ComparisonFunc>::find(const DATA_t &data) const
    {
        return find_aux(data);
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    const DATA_t &Tree<DATA_t, ComparisonFunc>::getMin() const
    {
        if (__root == nullptr)
        {
            throw NoSuchElementException();
        }
        return findMin()->__data;
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    const DATA_t &Tree<DATA_t, ComparisonFunc>::getMax() const
    {
        if (__root == nullptr)
        {
            throw NoSuchElementException();
        }
        return findMax()->__data;
    }

    template <typename DATA_t, Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)>
    void Tree<DATA_t, ComparisonFunc>::display()
    {
        std::cout << "\n";
        if (!isEmpty())
            display_aux(__root);
        else
            std::cout << "Empty";
        std::cout << "\n";
    }
};

#endif // _AVL_TREE_CPP_H_
