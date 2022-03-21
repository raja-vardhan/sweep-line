#ifndef STATUS_H
#define STATUS_H

#include <stdlib.h>
#include <vector>
#include<iostream>
using namespace std;


/// Structure to store a line segment.
///
/// Stored as four coordinates signifying start and end points.
struct LineSegment
{
  double startX; //!< X-coordinate of start point
  double startY; //!< Y-coordinate of start point
  double endX;   //!< X-coordinate of end point
  double endY;   //!< Y-coordinate of end point
};


/// Strucutre to represent a node of the status queue.
/// 
/// Line segment is used as a key.
struct StatusQueueNode
{
  LineSegment l; //!< Line segment used as a key for the node
  StatusQueueNode *left; //!< Pointer to left child in the search tree
  StatusQueueNode *right; //!< Pointer to right child in the search tree
  int height;   //!< Height of the node in the search tree
};


/// Implementation of the status queue data strucuture.
///
/// Uses a balanced binary search tree called **AVL Tree**.
class StatusQueue
{

public:
  /// globalinsert variable to signify inserting two nodes for each line
  int globalinsert;

  /// Basic constructor
  StatusQueue()
  {
    globalinsert = 0;
  }

  /// Find height of a node in the tree
  /// @param N Pointer to node
  int height(StatusQueueNode *N)
  {
    if (N == NULL)
      return 0;
    return N->height;
  }

  /// Return the maximum of two integers a, b
  int max(int a, int b)
  {
    return (a > b) ? a : b;
  }

  /// Create a new node
  /// @param newl Line segment to be used as key
  StatusQueueNode *newstatus(LineSegment newl)
  {
    StatusQueueNode *node = new StatusQueueNode;
    node->l = newl;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
  }


  /// Right rotate about a point in the tree to rebalance
  StatusQueueNode *rightRotate(StatusQueueNode *y)
  {
    StatusQueueNode *x = y->left;
    StatusQueueNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
  }


  /// Left rotate about a point in the tree to rebalance
  StatusQueueNode *leftRotate(StatusQueueNode *x)
  {
    StatusQueueNode *y = x->right;
    StatusQueueNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
  }


  /// Find x co-ordinate of a point on the line given the two end points and y co-ordinate
  /// @param l Line segment on which the point lies
  /// @param y Y-coordinate of the point
  double findx(LineSegment l, double y)
  {
    return ((y - l.endY) * ((l.endX - l.startX) / (l.endY - l.startY))) + l.endX;
  }


  /// Get balance factor of a node
  /// 
  /// Equal to height of left subtree - height of right subtree
  int getBalance(StatusQueueNode *N)
  {
    if (N == NULL)
      return 0;
    return height(N->left) - height(N->right);
  }


  /// Insert a new line into the status queue.
  /// @param node Pointer to root node
  /// @param newl New line segment to be inserted
  /// @param ycor Current Y-coordinate of the sweep line
  StatusQueueNode *insert(StatusQueueNode *node, LineSegment newl, double ycor)
  {
    int *justinserted = &globalinsert;

    if (node == NULL)
    {
      *justinserted = 1;
      return (newstatus(newl));
    }

    if (findx(newl, ycor) < findx(node->l, ycor))
    {
      node->left = insert(node->left, newl, ycor);
      if (*justinserted == 1)
      {
        node->right = newstatus(node->l);
        node->l = newl;
        *justinserted = 0;
      }
    }
    else if (findx(newl, ycor) > findx(node->l, ycor))
    {
      // printf("Going right\n");
      node->right = insert(node->right, newl, ycor);
      if (*justinserted == 1)
      {
        // printf("copying to left leaf\n");
        node->left = newstatus(node->l);
        *justinserted = 0;
      }
    }
    else
    {
      return node;
    }
    node->height = 1 + max(height(node->left),
                           height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && findx(newl, ycor) < findx(node->left->l, ycor))
      return rightRotate(node);

    if (balance < -1 && findx(newl, ycor) > findx(node->right->l, ycor))
      return leftRotate(node);

    if (balance > 1 && findx(newl, ycor) > findx(node->left->l, ycor))
    {
      node->left = leftRotate(node->left);
      return rightRotate(node);
    }
    if (balance < -1 && findx(newl, ycor) < findx(node->right->l, ycor))
    {
      node->right = rightRotate(node->right);
      return leftRotate(node);
    }

    return node;
  }


  /// Find the node with the min value for bst deletion
  /// @returns Pointer to the minimum value node
  StatusQueueNode *minValueNode(StatusQueueNode *node)
  {
    StatusQueueNode *current = node;

    while (current->left != NULL)
      current = current->left;

    return current;
  }


  /// Delete a line segment
  /// @param root Pointer to root node
  /// @param newl Line segment to be deleted
  /// @param ycor Current Y-coordinate of the sweep line
  StatusQueueNode *deleteNode(StatusQueueNode *root, LineSegment newl, double ycor)
  {

    if (root == NULL)
      return root;

    if (findx(newl, ycor) < findx(root->l, ycor))
      root->left = deleteNode(root->left, newl, ycor);

    else if (findx(newl, ycor) > findx(root->l, ycor))
      root->right = deleteNode(root->right, newl, ycor);

    else
    {
      // node with only one child or no child
      if ((root->left == NULL) || (root->right == NULL))
      {
        StatusQueueNode *temp = root->left ? root->left : root->right;

        if (temp == NULL)
        {
          temp = root;
          root = NULL;
        }
        else
          *root = *temp;
        free(temp);
      }
      else
      {

        StatusQueueNode *temp = minValueNode(root->right);

        // Copy the inorder successor's data to this node
        root->l = temp->l;
        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->l, ycor);
      }
    }

    if (root == NULL)
      return root;

    root->height = 1 + max(height(root->left),
                           height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
      return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0)
    {
      root->left = leftRotate(root->left);
      return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
      return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0)
    {
      root->right = rightRotate(root->right);
      return leftRotate(root);
    }

    return root;
  }


  /// Print preorder of current tree
  void preOrder(StatusQueueNode *root)
  {
    if (root != NULL)
    {
      //printf("%f %f %f %f %d\n", root->l.startX, root->l.startY, root->l.endX, root->l.endY, root->height);
      cout << root->l.startX << " " << root->l.startY << " "
      << root->l.endX << " " << root->l.endY << " " << root->height << "\n";
      preOrder(root->left);
      preOrder(root->right);
    }
  }

  /// Get the left neighbor of a particular line segment from the status queue
  void getLeftNeighbor(StatusQueueNode *node, LineSegment l, double ycor, struct LineSegment *lastRight)
  {
    if (node->height == 1)
    {
      if (lastRight->startX == -1)
      {
        if (findx(node->l, ycor - 0.1) < findx(l, ycor - 0.1))
        {
          *lastRight = node->l;
        }
      }
      return;
    }
    if ((findx(l, ycor - 0.1) - 0.1) < findx(node->l, ycor - 0.1))
    {
      getLeftNeighbor(node->left, l, ycor, lastRight);
    }
    else if ((findx(l, ycor - 0.1) - 0.1) > findx(node->l, ycor - 0.1))
    {
      *lastRight = node->l;
      getLeftNeighbor(node->right, l, ycor, lastRight);
    }
  }

  /// Get the right neighbor of a particular line segment from the status queue
  void getRightNeighbor(StatusQueueNode *node, LineSegment l, double ycor, struct LineSegment *lastLeft)
  {
    if (node->height == 1)
    {
      if (lastLeft->startX == -1)
      {
        if (findx(node->l, ycor - 0.1) > findx(l, ycor - 0.1))
        {
          *lastLeft = node->l;
        }
      }
      return;
    }
    if ((findx(l, ycor - 0.1) + 0.1) < findx(node->l, ycor - 0.1))
    {
      *lastLeft = node->l;
      getRightNeighbor(node->left, l, ycor, lastLeft);
    }
    else if ((findx(l, ycor - 0.1) + 0.1) > findx(node->l, ycor - 0.1))
    {
      getRightNeighbor(node->right, l, ycor, lastLeft);
    }
  }

  /// Get left and right neighbouring segments of a point
  ///
  /// lastLeft is the right neighbour and lastRight is the left neighbour for the point
  void getNeighbors(StatusQueueNode *node, double xcor, double ycor, struct LineSegment *lastRight, struct LineSegment *lastLeft)
  {
    if (node->height == 1)
    {
      if (lastRight->startX == -1)
      {
        if (findx(node->l, ycor - 0.1) <= xcor)
        {
          *lastRight = node->l;
        }
      }
      if (lastLeft->startX == -1)
      {
        if (findx(node->l, ycor - 0.1) > xcor)
        {
          *lastLeft = node->l;
        }
      }
      return;
    }

    if (xcor < findx(node->l, ycor - 0.1))
    {
      // printf("go left");
      *lastLeft = node->l;
      getNeighbors(node->left, xcor, ycor, lastRight, lastLeft);
    }
    else if (xcor > findx(node->l, ycor - 0.1))
    {
      // printf("go right");
      *lastRight = node->l;
      if (node->right == NULL)
      {
        return;
      }

      getNeighbors(node->right, xcor, ycor, lastRight, lastLeft);
    }
  }
};

#endif