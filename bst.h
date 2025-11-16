/* 
Gerardo Aguilar-Aguilera
5027294905
Sat Nov 15. 2025
*/
#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

template <typename Key, typename Value>
class Node
{
public:
    Node(const Key &key, const Value &value, Node<Key, Value> *parent);
    virtual ~Node();

    const std::pair<const Key, Value> &getItem() const;
    std::pair<const Key, Value> &getItem();
    const Key &getKey() const;
    const Value &getValue() const;
    Value &getValue();

    virtual Node<Key, Value> *getParent() const;
    virtual Node<Key, Value> *getLeft() const;
    virtual Node<Key, Value> *getRight() const;

    void setParent(Node<Key, Value> *parent);
    void setLeft(Node<Key, Value> *left);
    void setRight(Node<Key, Value> *right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value> *parent_;
    Node<Key, Value> *left_;
    Node<Key, Value> *right_;
};

template <typename Key, typename Value>
Node<Key, Value>::Node(const Key &key, const Value &value, Node<Key, Value> *parent)
    : item_(key, value),
      parent_(parent),
      left_(NULL),
      right_(NULL)
{
}

template <typename Key, typename Value>
Node<Key, Value>::~Node()
{
}

template <typename Key, typename Value>
const std::pair<const Key, Value> &Node<Key, Value>::getItem() const
{
    return item_;
}

template <typename Key, typename Value>
std::pair<const Key, Value> &Node<Key, Value>::getItem()
{
    return item_;
}

template <typename Key, typename Value>
const Key &Node<Key, Value>::getKey() const
{
    return item_.first;
}

template <typename Key, typename Value>
const Value &Node<Key, Value>::getValue() const
{
    return item_.second;
}

template <typename Key, typename Value>
Value &Node<Key, Value>::getValue()
{
    return item_.second;
}

template <typename Key, typename Value>
Node<Key, Value> *Node<Key, Value>::getParent() const
{
    return parent_;
}

template <typename Key, typename Value>
Node<Key, Value> *Node<Key, Value>::getLeft() const
{
    return left_;
}

template <typename Key, typename Value>
Node<Key, Value> *Node<Key, Value>::getRight() const
{
    return right_;
}

template <typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value> *parent)
{
    parent_ = parent;
}

template <typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value> *left)
{
    left_ = left;
}

template <typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value> *right)
{
    right_ = right;
}

template <typename Key, typename Value>
void Node<Key, Value>::setValue(const Value &value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
 * A templated unbalanced binary search tree.
 */
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    virtual void insert(const std::pair<const Key, Value> &keyValuePair);
    virtual void remove(const Key &key);
    void clear();
    bool isBalanced() const;
    void print() const;
    bool empty() const;

    template <typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> &tree);

public:
    /**
     * An internal iterator class for traversing the contents of the BST.
     */
    class iterator // TODO
    {
    public:
        iterator();
        std::pair<const Key, Value> &operator*() const;
        std::pair<const Key, Value> *operator->() const;
        bool operator==(const iterator &rhs) const;
        bool operator!=(const iterator &rhs) const;
        iterator &operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key, Value> *ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key &key) const;
    Value &operator[](const Key &key);
    Value const &operator[](const Key &key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value> *internalFind(const Key &k) const;
    Node<Key, Value> *getSmallestNode() const;
    static Node<Key, Value> *predecessor(Node<Key, Value> *current);

    // Provided helper functions
    virtual void printRoot(Node<Key, Value> *r) const;
    virtual void nodeSwap(Node<Key, Value> *n1, Node<Key, Value> *n2);

    // Additional helper functions
    void clearHelper(Node<Key, Value> *node);
    int get_height_balance(Node<Key, Value> *node, bool &balanced) const;
    static Node<Key, Value> *successor(Node<Key, Value> *current);

protected:
    Node<Key, Value> *root_;
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
 * Explicit constructor that initializes an iterator with a given node pointer.
 *
 * @param ptr The node to point the iterator at
 */
template <class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key, Value> *ptr)
    : current_(ptr)
{
}

/**
 * A default constructor that initializes the iterator to NULL.
 * This represents an "end" or invalid iterator.
 */
template <class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator()
    : current_(nullptr)
{
}

/**
 * Provides access to the item (key-value pair).
 * Dereferences the iterator to get the pair it points to.
 */
template <class Key, class Value>
std::pair<const Key, Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
 * Provides access to the address of the item.
 * Allows using -> to access members of the pair.
 */
template <class Key, class Value>
std::pair<const Key, Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
 * Checks if 'this' iterator's internals have the same value as 'rhs'.
 * Two iterators are equal if they point to the same node.
 */
template <class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator &rhs) const
{
    return current_ == rhs.current_;
}

/**
 * Checks if 'this' iterator's internals have a different value as 'rhs'.
 */
template <class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator &rhs) const
{
    return current_ != rhs.current_;
}

/**
 * Advances the iterator's location using an in-order sequencing.
 * This moves to the next node in sorted order (by key).
 */
template <class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator &
BinarySearchTree<Key, Value>::iterator::operator++()
{
    current_ = successor(current_);
    return *this;
}

/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
 * Default constructor for a BinarySearchTree.
 * Initializes an empty tree with root pointing to nullptr.
 */
template <class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree()
{
    root_ = nullptr;
}

/**
 * Destructor for the BST.
 * Calls clear() to delete all nodes and free memory.
 */
template <typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();
}

/**
 * Returns true if tree is empty.
 */
template <class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

/**
 * Prints a visual representation of the tree.
 */
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
 * Returns an iterator to the "smallest" item in the tree.
 * This is the leftmost node (smallest key).
 */
template <class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
 * Returns an iterator whose value means INVALID.
 * This represents "one past the end" of the tree.
 */
template <class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
 * Returns an iterator to the item with the given key, k
 * or the end iterator if k does not exist in the tree.
 */
template <class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key &k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key.
 */
template <class Key, class Value>
Value &BinarySearchTree<Key, Value>::operator[](const Key &key)
{
    Node<Key, Value> *curr = internalFind(key);
    if (curr == NULL)
        throw std::out_of_range("Invalid key");
    return curr->getValue();
}

template <class Key, class Value>
Value const &BinarySearchTree<Key, Value>::operator[](const Key &key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if (curr == NULL)
        throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
 * An insert method to insert into a Binary Search Tree.
 * The tree will not remain balanced when inserting.
 * If key is already in the tree, overwrites the current value with the updated value.
 *
 * Runtime: O(h) where h is the height of the tree
 */
template <class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    // CASE 1: empty tree
    if (root_ == nullptr)
    {
        // create root node
        root_ = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr);
        return;
    }

    // CASE 2: otherwise find insertion point
    Node<Key, Value> *curr = root_;
    Node<Key, Value> *parent = nullptr;

    while (curr != nullptr)
    {
        parent = curr;
        if (keyValuePair.first < curr->getKey())
        {
            curr = curr->getLeft(); // new key is smaller, go left
        }
        else if (keyValuePair.first > curr->getKey())
        {
            curr = curr->getRight(); // new key is larger, go right
        }
        else
        {
            // key already exists - just update the value
            curr->setValue(keyValuePair.second);
            return;
        }
    }
    // insert since null
    Node<Key, Value> *newNode = new Node<Key, Value>(
        keyValuePair.first, keyValuePair.second, parent);
    // link new node
    if (keyValuePair.first < parent->getKey()) // left, cuz smaller
    {
        parent->setLeft(newNode);
    }
    else
    {
        parent->setRight(newNode); // tight, cuz larger
    }
}

/**
 * A remove method to remove a specific key from a Binary Search Tree.
 * If a node has 2 children, swap with the predecessor and then remove.
 *
 * Runtime: O(h) where h is the height of the tree
 */
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key &key)
{
    // find node to remove
    Node<Key, Value> *node = internalFind(key);
    if (node == nullptr)
    {
        return; // not in tree
    }
    else
    {
        if (node->getLeft() != nullptr && node->getRight() != nullptr)
        {
            Node<Key, Value> *pred = predecessor(node);
            nodeSwap(node, pred);
        }
        Node<Key, Value> *parent = node->getParent();
        Node<Key, Value> *child = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();
        if (child != nullptr)
        {
            child->setParent(parent);
        }
        if (parent == nullptr)
        {
            root_ = child;
        }
        else if (parent->getLeft() == node)
        {
            parent->setLeft(child);
        }
        else
        {
            parent->setRight(child);
        }
        delete node;
    }
}

/**
 * Find the predecessor (previous node in sorted order) of current.
 *
 * TWO CASES:
 * 1. If current has a left subtree: predecessor is the rightmost node in that subtree
 * 2. If no left subtree: go up until we find an ancestor where we came from the right
 *
 * @param current The node to find the predecessor of
 * @return Pointer to predecessor node, or nullptr if no predecessor exists
 */
template <class Key, class Value>
Node<Key, Value> *
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value> *current)
{
    if (!current)
    {
        return nullptr;
    }
    Node<Key, Value> *left = current->getLeft();
    if (left != nullptr)
    {
        Node<Key, Value> *curr = left;
        while (curr->getRight() != nullptr)
        {
            curr = curr->getRight();
        }
        return curr;
    }

    Node<Key, Value> *parent = current->getParent();
    while (parent != nullptr && current == parent->getLeft())
    {
        current = parent;
        parent = parent->getParent();
    }

    return parent;
}

/**
 * Find the successor (next node in sorted order) of current.
 * This is the mirror image of predecessor.
 *
 * TWO CASES:
 * 1. If current has a right subtree: successor is the leftmost node in that subtree
 * 2. If no right subtree: go up until we find an ancestor where we came from the left
 *
 * @param current The node to find the successor of
 * @return Pointer to successor node, or nullptr if no successor exists
 */
template <class Key, class Value>
Node<Key, Value> *
BinarySearchTree<Key, Value>::successor(Node<Key, Value> *current)
{
    if (!current)
    {
        return nullptr;
    }

    // CASE 1: node has right subtree
    if (current->getRight() != nullptr)
    {
        current = current->getRight();
        while (current->getLeft() != nullptr)
        {
            current = current->getLeft();
        }
        return current;
    }

    // CASE 2: no right subtree
    Node<Key, Value> *parent = current->getParent();
    while (parent != nullptr && current == parent->getRight())
    {
        current = parent;
        parent = parent->getParent();
    }
    return parent;
}

/**
 * Recursive helper function to delete all nodes in a subtree.
 * Uses post-order traversal: delete children first, then parent.
 *
 * @param node Root of subtree to delete
 */
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key, Value> *node)
{
    if (!node)
    {
        return;
    }
    clearHelper(node->getLeft());
    clearHelper(node->getRight());
    delete node;
}

/**
 * A method to remove all contents of the tree and
 * reset the values in the tree for use again.
 *
 * Runtime: O(n) where n is number of nodes
 */
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    clearHelper(root_);
    root_ = nullptr;
}

/**
 * A helper function to find the smallest node in the tree.
 * The smallest node is the leftmost node (keep going left).
 *
 * Runtime: O(h) where h is the height of the tree
 */
template <typename Key, typename Value>
Node<Key, Value> *
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    if (!root_)
    {
        return nullptr;
    }

    Node<Key, Value> *current = root_;
    while (current->getLeft() != nullptr)
    {
        current = current->getLeft();
    }
    return current;
}

/**
 * Helper function to find a node with given key.
 *
 * @param key The key to search for
 * @return Pointer to node with that key, or nullptr if not found
 *
 * Runtime: O(h) where h is the height of the tree
 */
template <typename Key, typename Value>
Node<Key, Value> *BinarySearchTree<Key, Value>::internalFind(const Key &key) const
{
    Node<Key, Value> *curr = root_;

    while (curr != nullptr)
    {
        if (key < curr->getKey())
        {
            curr = curr->getLeft(); // Key is smaller, go left
        }
        else if (key > curr->getKey())
        {
            curr = curr->getRight(); // Key is larger, go right
        }
        else
        {
            return curr; // Found it!
        }
    }

    return nullptr; // Not found
}

/**
 * Helper function that calculates height and checks if tree is balanced.
 * A tree is balanced if at every node, the heights of left and right
 * subtrees differ by at most 1.
 *
 * @param node Current node being examined
 * @param balanced Reference to bool that tracks if tree is balanced
 * @return Height of subtree rooted at node
 */
template <typename Key, typename Value>
int BinarySearchTree<Key, Value>::get_height_balance(Node<Key, Value> *node, bool &balanced) const
{
    if (!node)
    {
        return 0;
    }

    int leftHeight = get_height_balance(node->getLeft(), balanced);
    int rightHeight = get_height_balance(node->getRight(), balanced);

    if (abs(leftHeight - rightHeight) > 1)
    {
        balanced = false;
    }
    int bigger;
    if (leftHeight > rightHeight)
    {
        bigger = leftHeight;
    }
    else
    {
        bigger = rightHeight;
    }

    return 1 + bigger;
}

/**
 * Return true if and only if the BST is balanced.
 * A balanced tree has the property that at every node,
 * the heights of left and right subtrees differ by at most 1.
 *
 * This checks if the tree satisfies the AVL balance property.
 */
template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    bool balanced = true;
    get_height_balance(root_, balanced);
    return balanced;
}

/**
 * Swaps the positions of two nodes in the tree.
 * This is a complex operation that updates all pointers correctly.
 */
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(Node<Key, Value> *n1, Node<Key, Value> *n2)
{
    if ((n1 == n2) || (n1 == NULL) || (n2 == NULL))
    {
        return;
    }
    Node<Key, Value> *n1p = n1->getParent();
    Node<Key, Value> *n1r = n1->getRight();
    Node<Key, Value> *n1lt = n1->getLeft();
    bool n1isLeft = false;
    if (n1p != NULL && (n1 == n1p->getLeft()))
        n1isLeft = true;
    Node<Key, Value> *n2p = n2->getParent();
    Node<Key, Value> *n2r = n2->getRight();
    Node<Key, Value> *n2lt = n2->getLeft();
    bool n2isLeft = false;
    if (n2p != NULL && (n2 == n2p->getLeft()))
        n2isLeft = true;

    Node<Key, Value> *temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if ((n1r != NULL && n1r == n2))
    {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if (n2r != NULL && n2r == n1)
    {
        n1->setRight(n2);
        n2->setParent(n1);
    }
    else if (n1lt != NULL && n1lt == n2)
    {
        n2->setLeft(n1);
        n1->setParent(n2);
    }
    else if (n2lt != NULL && n2lt == n1)
    {
        n1->setLeft(n2);
        n2->setParent(n1);
    }

    if (n1p != NULL && n1p != n2)
    {
        if (n1isLeft)
            n1p->setLeft(n2);
        else
            n1p->setRight(n2);
    }
    if (n1r != NULL && n1r != n2)
    {
        n1r->setParent(n2);
    }
    if (n1lt != NULL && n1lt != n2)
    {
        n1lt->setParent(n2);
    }

    if (n2p != NULL && n2p != n1)
    {
        if (n2isLeft)
            n2p->setLeft(n1);
        else
            n2p->setRight(n1);
    }
    if (n2r != NULL && n2r != n1)
    {
        n2r->setParent(n1);
    }
    if (n2lt != NULL && n2lt != n1)
    {
        n2lt->setParent(n1);
    }

    if (this->root_ == n1)
    {
        this->root_ = n2;
    }
    else if (this->root_ == n2)
    {
        this->root_ = n1;
    }
}

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif