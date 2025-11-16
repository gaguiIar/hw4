/* 
Gerardo Aguilar-Aguilera
5027294905
Sat Nov 15. 2025
*/
#ifndef RECCHECK
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int checkHeight(Node *node)
{
    //int return_val = -1;
    if (node == nullptr)
    {
        return 0;
    }
    // Recursive call
    int leftHeight = checkHeight(node->left);
    int rightHeight = checkHeight(node->right);

    //  a subtree has unequal paths
    if (leftHeight == -1 || rightHeight == -1)
    {
        return -1;
    }

    // left right child 
    if (node->left != nullptr && node->right != nullptr)
    {
        if (leftHeight != rightHeight)
        {
            return -1;
        }
        return leftHeight + 1;
    }
    // only left child
    if (node->left != nullptr)
    {
        if (leftHeight != 1)
        {
            return -1;
        }
        return leftHeight + 1;
    }

    // only right child
    if (node->right != nullptr)
    {
        if (rightHeight != 1)
        {
            return -1;
        }
        return rightHeight + 1;
    }
    return 1; // leaf node
}


bool equalPaths(Node * root)
{
    // Add your code below
    //base case:
    if(!root) {
      return true;
    }
    return (checkHeight(root) != -1);
}
