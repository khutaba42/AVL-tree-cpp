# Documentation: AVL Tree implementation in C++

## Intro.

This library can be used without relying on the standard library on anything but the error classes (`std::exception`).

This tree by default makes comparisons between `DATA_t` elements using the `<` and the `>` operators, if theres another way you would like to compare them, create a function of this signature
```C++
Comparison (*ComparisonFunc)(const DATA_t &, const DATA_t &)
```
where the `Comparison` enum is 
```C++
enum class Comparison {
    less    = 0,
    equal   = 1,
    greater = 2
};
```

## (Public) Methods 

### `Tree()`:

an argument-less constructor that creates an empty tree. Time Complexity: $O(1)$.

### `~Tree()`:

a destructor. Time Complexity: $O(n)$.

### `insert(const DATA_t &data)`:

inserts the argument into the tree. Time Complexity: $\theta(log\,n)$. Beware, it will throw an `ElementAlreadyExistsException` error if element was already in.

### `remove(const DATA_t &data)`:

removes the argument from the tree. Time Complexity: $\theta(log\,n)$. Beware, it will throw an `NoSuchElementException` error if element wasn't already in.

### `isEmpty()`:

return `true` if the tree is empty. Time Complexity: $O(1)$.

### `size()`:

returns the number of elements in the tree. Time Complexity: $O(1)$.

### `find(const DATA_t &data)`:

takes in an element and looks for the same element in the tree by using the `=` operator. Time Complexity: $O(log\,n)$. Beware, this methods throws a `NoSuchElementException` error if no such element is found.

### `getMin()`:

return the minimum element in the tree. Time Complexity: $O(1)$. Beware, this methods throws a `NoSuchElementException` error if no such element is found.

### `getMax()`:

return the maximum element in the tree. Time Complexity: $O(1)$. Beware, this methods throws a `NoSuchElementException` error if no such element is found.

### `in_order_traversal(FunctionObject do_something)`:

if the tree is empty then the function returns `false`, otherwise it take in as an argument a function object (or a function pointer) that takes in as an argument `DATA_t` and performs an operation on it, this will be done in-order. Time Complexity: $O(n)$. Space Complexity: $O(log\,n)$. Beware, if you change the data in a way that causes the comparison between the elements to change this will cause undefined behavior.

### `reverse_in_order_traversal_aux_recursive(FunctionObject do_something)`:

if the tree is empty then the function returns `false`, otherwise it take in as an argument a function object (or a function pointer) that takes in as an argument `DATA_t` and performs an operation on it, this will be done like in-order but in reverse. Time Complexity: $O(n)$. Space Complexity: $O(log\,n)$. Beware, if you change the data in a way that causes the comparison between the elements to change this will cause undefined behavior.

### `display()`:

displays the tree in the terminal in a visual way. and displays the Word `Empty` in case the tree was empty.

## Error classes

all error classes are (publicly) inherited from `std::exception` from the standard library with the `what()` method implemented.

`NoSuchElementException`

`ElementAlreadyExistsException`