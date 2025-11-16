/* 
Gerardo Aguilar-Aguilera
5027294905
Sat Nov 15. 2025
*/
#ifndef AVLBST_H
#define AVLBST_H
#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError
{
};

/**
 * A special kind of node for an AVL tree, which adds the balance as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key &key, const Value &value, AVLNode<Key, Value> *parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance() const;
    void setBalance(int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value> *getParent() const override;
    virtual AVLNode<Key, Value> *getLeft() const override;
    virtual AVLNode<Key, Value> *getRight() const override;

protected:
    int8_t balance_; // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class constructor
 */
template <class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key &key, const Value &value, AVLNode<Key, Value> *parent) : Node<Key, Value>(key, value, parent), balance_(0)
{
}

/**
 * A destructor which does nothing.
 */
template <class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{
}

/**
 * A getter for the balance of a AVLNode.
 */
template <class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
 * A setter for the balance of a AVLNode.
 */
template <class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
 * Adds diff to the balance of a AVLNode.
 */
template <class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value> *>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value> *>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value> *>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert(const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key &key);                              // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value> *n1, AVLNode<Key, Value> *n2);

    // Add helper functions here
    void insert_fix(AVLNode<Key, Value> *parent, AVLNode<Key, Value> *node); // done
    void remove_fix(AVLNode<Key, Value> *node, int8_t diff);
    void rotate_left(AVLNode<Key, Value> *node);
    void rotate_right(AVLNode<Key, Value> *node);
};

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template <class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    const Key &key = new_item.first;
    const Value &value = new_item.second;
    // empty tree
    if (this->root_ == nullptr)
    {
        this->root_ = new AVLNode<Key, Value>(key, value, nullptr);
        return;
    }
    AVLNode<Key, Value> *curr = static_cast<AVLNode<Key, Value> *>(this->root_);
    AVLNode<Key, Value> *parent = nullptr;
    // look for insertion point
    while (curr != nullptr)
    {
        parent = curr;
        if (key < curr->getKey())
        { // go left
            curr = curr->getLeft();
        }
        else if (key > curr->getKey())
        {
            curr = curr->getRight();
        }
        else if (new_item.first == curr->getKey())
        { // key alr exists
            curr->setValue(value);
            return;
        }
    }
    // found insertion point, insert
    AVLNode<Key, Value> *new_node = new AVLNode<Key, Value>(key, value, parent);

    if (key < parent->getKey())
    {
        parent->setLeft(new_node);
    }
    else
    {
        parent->setRight(new_node);
    }

    // Balance and fix
    int old_balance = parent->getBalance();

    if (old_balance == 0) // perfectly balanced
    {
        if (parent->getLeft() == new_node)
        {
            parent->setBalance(-1);
        }
        else
        {
            parent->setBalance(1);
        }

        insert_fix(parent, new_node);
    }
    else
    {
        parent->setBalance(0);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template <class Key, class Value>
void AVLTree<Key, Value>::remove(const Key &key)
{
    // TODO
    AVLNode<Key, Value> *toRemove =
        static_cast<AVLNode<Key, Value> *>(
            BinarySearchTree<Key, Value>::internalFind(key));

    if (toRemove == nullptr)
    {
        return;
    }

    if (toRemove->getLeft() != nullptr && toRemove->getRight() != nullptr)
    {
        AVLNode<Key, Value> *pred =
            static_cast<AVLNode<Key, Value> *>(
                BinarySearchTree<Key, Value>::predecessor(toRemove));
        nodeSwap(toRemove, pred);
    }
    AVLNode<Key, Value> *parent = toRemove->getParent();

    int8_t diff = 0;
    if (parent != nullptr)
    {
        if (parent->getLeft() == toRemove)
        {
            diff = 1; // removed from left
        }
        else
        {
            diff = -1; // removed from right
        }
    }

    AVLNode<Key, Value> *child = nullptr;
    if (toRemove->getLeft() != nullptr)
    {
        child = toRemove->getLeft();
    }
    else
    {
        child = toRemove->getRight();
    }
    if (child != nullptr)
    {
        child->setParent(parent);
    }
    if (parent == nullptr)
    {
        this->root_ = child;
    }
    else
    {
        if (parent->getLeft() == toRemove)
        {
            parent->setLeft(child);
        }
        else
        {
            parent->setRight(child);
        }
    }
    delete toRemove;
    if (parent != nullptr)
    {
        remove_fix(parent, diff);
    }
}
template <typename Key, typename Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key, Value> *parent,
                                     AVLNode<Key, Value> *node)
{
    if (parent == nullptr)
        return;
    AVLNode<Key, Value> *grand = parent->getParent();
    if (grand == nullptr)
        return;

    bool parentIsLeftChild = (grand->getLeft() == parent);

    if (parentIsLeftChild)
    {
        grand->updateBalance(-1); // left subtree got taller
    }
    else
    {
        grand->updateBalance(1); // right subtree got taller
    }

    int gpBalance = grand->getBalance();

    // Case 1: height at grandparent didn't change overall
    if (gpBalance == 0)
    {
        return;
    }

    // LEFT SIDE CASES
    if (parentIsLeftChild)
    {
        // balanced height went up
        if (gpBalance == -1)
        {
            insert_fix(grand, parent);
        }
        else
        { // gpBalance == -2 rotate

            // Left-Left case
            if (parent->getLeft() == node)
            {
                rotate_right(grand);
                parent->setBalance(0);
                grand->setBalance(0);
            }
            // Left-Right case
            else
            {
                rotate_left(parent);
                rotate_right(grand);

                int nb = node->getBalance();
                if (nb == -1)
                {
                    parent->setBalance(0);
                    grand->setBalance(1);
                }
                else if (nb == 0)
                {
                    parent->setBalance(0);
                    grand->setBalance(0);
                }
                else
                { // nb == 1
                    parent->setBalance(-1);
                    grand->setBalance(0);
                }
                node->setBalance(0);
            }
        }
    }
    // RIGHT SIDE CASES
    else
    {
        if (gpBalance == 1)
        {
            insert_fix(grand, parent);
        }
        else
        { // gpBalance == 2 rotate

            // Right-Right
            if (parent->getRight() == node)
            {
                rotate_left(grand);
                parent->setBalance(0);
                grand->setBalance(0);
            }
            // Right-Left
            else
            {
                rotate_right(parent);
                rotate_left(grand);

                int nb = node->getBalance();
                if (nb == 1)
                {
                    parent->setBalance(0);
                    grand->setBalance(-1);
                }
                else if (nb == 0)
                {
                    parent->setBalance(0);
                    grand->setBalance(0);
                }
                else
                { // nb == -1
                    parent->setBalance(1);
                    grand->setBalance(0);
                }
                node->setBalance(0);
            }
        }
    }
}
template <typename Key, typename Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key, Value> *node, int8_t diff)
{
    if (node == nullptr)
    {
        return;
    }
    AVLNode<Key, Value> *parent = node->getParent();
    int next_diff = 0;
    if (parent != nullptr)
    {
        if (parent->getLeft() == node)
        {
            next_diff = 1; // left got shorter
        }
        else
        {
            next_diff = -1; // right got shorter
        }
    }
    int new_balance = node->getBalance() + diff;
    if (diff == 1)
    {
        if (new_balance == 2)
        {
            AVLNode<Key, Value> *child = node->getRight();
            if (child->getBalance() == 1)
            {
                rotate_left(node);
                node->setBalance(0);
                child->setBalance(0);
                remove_fix(parent, next_diff); // subtree height decreased, keep going up
            }
            // Special "child balanced" case
            else if (child->getBalance() == 0)
            {
                rotate_left(node);
                node->setBalance(1);
                child->setBalance(-1);
            }
            // Right-Left case
            else
            { // child->getBalance() == -1
                AVLNode<Key, Value> *grandchild = child->getLeft();
                rotate_right(child);
                rotate_left(node);

                if (grandchild->getBalance() == -1)
                {
                    node->setBalance(0);
                    child->setBalance(1);
                }
                else if (grandchild->getBalance() == 0)
                {
                    node->setBalance(0);
                    child->setBalance(0);
                }
                else
                { // grandchild->getBalance() == 1
                    node->setBalance(-1);
                    child->setBalance(0);
                }
                grandchild->setBalance(0);
                remove_fix(parent, next_diff); // height decreased, keep going up
            }
        }
        // node is slightly right-heavy now, but height unchanged
        else if (new_balance == 1)
        {
            node->setBalance(1);
            // stop: no further fix needed
        }
        // new_balance == 0: node was left-heavy (-1), left shrank, now perfectly balanced
        else
        { // new_balance == 0
            node->setBalance(0);
            // height decreased at this node → propagate upward
            remove_fix(parent, next_diff);
        }
    }

    else if (diff == -1)
    {
        // If new balance is -2, node is too LEFT-heavy
        if (new_balance == -2)
        {
            AVLNode<Key, Value> *child = node->getLeft();

            // Left-Left case
            if (child->getBalance() == -1)
            {
                rotate_right(node);
                node->setBalance(0);
                child->setBalance(0);
                remove_fix(parent, next_diff); // height decreased, keep going up
            }
            // Special "child balanced" case
            else if (child->getBalance() == 0)
            {
                rotate_right(node);
                node->setBalance(-1);
                child->setBalance(1);
            }
            // Left-Right case
            else
            { // child->getBalance() == 1
                AVLNode<Key, Value> *grandchild = child->getRight();
                rotate_left(child);
                rotate_right(node);

                if (grandchild->getBalance() == 1)
                {
                    node->setBalance(0);
                    child->setBalance(-1);
                }
                else if (grandchild->getBalance() == 0)
                {
                    node->setBalance(0);
                    child->setBalance(0);
                }
                else
                { // grandchild->getBalance() == -1
                    node->setBalance(1);
                    child->setBalance(0);
                }
                grandchild->setBalance(0);
                remove_fix(parent, next_diff); // height decreased, keep going up
            }
        }
        else if (new_balance == -1)
        {
            node->setBalance(-1);
        }
        else
        {
            node->setBalance(0);
            remove_fix(parent, next_diff);
        }
    }
}
template <class Key, class Value>
void AVLTree<Key, Value>::rotate_left(AVLNode<Key, Value> *node)
{
    AVLNode<Key, Value> *child = node->getRight();
    AVLNode<Key, Value> *parent = node->getParent();
    AVLNode<Key, Value> *subtreeA = child->getLeft();

    node->setRight(subtreeA);
    if (subtreeA != nullptr)
    {
        subtreeA->setParent(node);
    }

    child->setLeft(node);
    node->setParent(child);

    child->setParent(parent);

    if (parent == nullptr)
    {
        this->root_ = child;
    }
    else if (parent->getLeft() == node)
    {
        parent->setLeft(child);
    }
    else
    {
        parent->setRight(child);
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotate_right(AVLNode<Key, Value> *node)
{
    AVLNode<Key, Value> *child = node->getLeft();
    AVLNode<Key, Value> *parent = node->getParent();
    AVLNode<Key, Value> *subtreeA = child->getRight();

    node->setLeft(subtreeA);
    if (subtreeA != nullptr)
    {
        subtreeA->setParent(node);
    }

    child->setRight(node);
    node->setParent(child);

    child->setParent(parent);

    if (parent == nullptr)
    {
        this->root_ = child;
    }
    else if (parent->getLeft() == node)
    {
        parent->setLeft(child);
    }
    else
    {
        parent->setRight(child);
    }
}
template <class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value> *n1, AVLNode<Key, Value> *n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif
