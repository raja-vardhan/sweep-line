#ifndef EVENT_H
#define EVENT_H

#include<iostream>
#include<stdlib.h>
#include<vector>
#include"StatusQueue.h"
using namespace std;

/// Structure to represent a node of the event queue.
struct EventQueueNode
{
  double xc;              //!< X-coordinate of event point
  double yc;              //!< Y-coordinate of event point
  EventQueueNode *left;   //!< Pointer to left child in the search tree
  EventQueueNode *right;  //!< Pointer to right child in the search tree
  vector<LineSegment> U;  //!< Set of segments whose upper endpoint is the event point
  vector<LineSegment> L;  //!< Set of segments whose lower endpoint is the event point
  vector<LineSegment> C;  //!< Set of segments with the event point as an interior point
  int height;             //!< Height of a node in the search tree
};

/// Implementation of the event queue data strucuture.
///
/// Uses a balanced binary search tree called **AVL Tree**.
class EventQueue
{

public:
  /// Find height of a node
  /// @param N Pointer to the node
  int height(EventQueueNode *N)
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

  /// Create a new event point
  ///
  /// Type specifies the type of the event point. Value for type is
  ///
  /// 1 - upper endpoint
  ///
  /// 2 - lower endpoint
  ///
  /// 3 - intersection point
  /// @param xc X-coordinate of event point
  /// @param yc Y-coordinate of event point
  /// @param xs X-coordinate of start point of line segment
  /// @param ys Y-coordinate of start point of line segment
  /// @param xe X-coordinate of end point of line segment
  /// @param ye Y-coordinate of end point of line segment
  /// @param type Type of the event point
  EventQueueNode *newq(double xc, double yc, double xs, double ys, double xe, double ye, int type)
  {

    EventQueueNode *node = new EventQueueNode;

    node->xc = xc;
    node->yc = yc;
    LineSegment *l = new LineSegment;
    l->startX = xs;
    l->startY = ys;
    l->endX = xe;
    l->endY = ye;
    if (type == 1)
    {
      node->U.push_back(*l);
    }
    else if (type == 2)
    {
      node->L.push_back(*l);
    }
    else
      node->C.push_back(*l);

    node->left = NULL;
    node->right = NULL;
    node->height = 1;

    return (node);
  }

  /// Right rotate about a point in the tree to rebalance
  EventQueueNode *rightRotate(EventQueueNode *y)
  {
    EventQueueNode *x = y->left;
    EventQueueNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
  }

  /// Left rotate about a point in the tree to rebalance
  EventQueueNode *leftRotate(EventQueueNode *x)
  {
    EventQueueNode *y = x->right;
    EventQueueNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
  }

  /// Used as a comparator for insertion and deletion
  int mygreater(double x1, double y1, double x2, double y2)
  {
    if (y1 < y2)
      return 0;
    else if (y1 > y2)
      return 1;
    else
    {
      if (x1 > x2)
        return 0;
      else if (x1 < x2)
        return 1;
    }
    return 0;
  }


  /// Used as a comparator for insertion and deletion
  int mylesser(double x1, double y1, double x2, double y2)
  {
    if (y1 < y2)
      return 1;
    else if (y1 > y2)
      return 0;
    else
    {
      if (x1 > x2)
        return 1;
      else if (x1 < x2)
        return 0;
    }
    return 0;
  }

  /// Get balance factor of a node
  int getBalance(EventQueueNode *N)
  {
    if (N == NULL)
      return 0;
    return height(N->left) - height(N->right);
  }

  /// Insert a new point in the event queue.
  ///
  /// Type specifies the type of the event point. Value for type is
  ///
  /// 1 - upper endpoint
  ///
  /// 2 - lower endpoint
  ///
  /// 3 - intersection point
  /// @param xc X-coordinate of event point
  /// @param yc Y-coordinate of event point
  /// @param xs X-coordinate of start point of line segment
  /// @param ys Y-coordinate of start point of line segment
  /// @param xe X-coordinate of end point of line segment
  /// @param ye Y-coordinate of end point of line segment
  /// @param type Type of the event point
  EventQueueNode *insert(EventQueueNode *node, double xc, double yc, double xs, double ys, double xe, double ye, int type)
  {
    // printf("start insert\n");
    if (node == NULL)
      return (newq(xc, yc, xs, ys, xe, ye, type));

    if (yc < node->yc)
    {
      // printf("Going left\n");

      node->left = insert(node->left, xc, yc, xs, ys, xe, ye, type);
    }
    else if (yc > node->yc)
    {
      // printf("Going right\n");
      node->right = insert(node->right, xc, yc, xs, ys, xe, ye, type);
    }
    else if (xc > node->xc)
    {
      // printf("Going left\n");
      node->left = insert(node->left, xc, yc, xs, ys, xe, ye, type);
    }
    else if (xc < node->xc)
    {
      // printf("Going right\n");
      node->right = insert(node->right, xc, yc, xs, ys, xe, ye, type);
    }
    else
    {
      int counter = 1;
      if (type == 1)
      {
        for (int EventQueueNode = 0; EventQueueNode < node->U.size(); EventQueueNode++)
        {
          LineSegment l = node->U[EventQueueNode];
          if (l.startX == xs && l.startY == ys && l.endX == xe && l.endY == ye)
            counter = 0;
        }
        if (counter == 1)
        {
          LineSegment *l = new LineSegment;
          l->startX = xs;
          l->startY = ys;
          l->endX = xe;
          l->endY = ye;
          node->U.push_back(*l);
        }
      }
      else if (type == 2)
      {
        for (int EventQueueNode = 0; EventQueueNode < node->L.size(); EventQueueNode++)
        {
          LineSegment l = node->L[EventQueueNode];
          if (l.startX == xs && l.startY == ys && l.endX == xe && l.endY == ye)
            counter = 0;
        }
        if (counter == 1)
        {
          LineSegment *l = new LineSegment;
          l->startX = xs;
          l->startY = ys;
          l->endX = xe;
          l->endY = ye;
          node->L.push_back(*l);
        }
      }
      else if (type == 3)
      {
        for (int EventQueueNode = 0; EventQueueNode < node->C.size(); EventQueueNode++)
        {
          LineSegment l = node->C[EventQueueNode];
          if (l.startX == xs && l.startY == ys && l.endX == xe && l.endY == ye)
            counter = 0;
        }
        if (counter == 1)
        {

          LineSegment *l = new LineSegment;
          l->startX = xs;
          l->startY = ys;
          l->endX = xe;
          l->endY = ye;
          node->C.push_back(*l);
        }
      }
      if (counter == 0)
        return node;
    }

    // printf("begin balancing\n");
    node->height = 1 + max(height(node->left),
                           height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && mylesser(xc, yc, node->left->xc, node->left->yc))
      return rightRotate(node);

    if (balance < -1 && mygreater(xc, yc, node->right->xc, node->right->yc))
      return leftRotate(node);

    if (balance > 1 && mygreater(xc, yc, node->left->xc, node->left->yc))
    {
      node->left = leftRotate(node->left);
      return rightRotate(node);
    }
    if (balance < -1 && mylesser(xc, yc, node->right->xc, node->right->yc))
    {
      node->right = rightRotate(node->right);
      return leftRotate(node);
    }
    // printf("done balancing\n");
    return node;
  }

  /// Find the minimum value node for bst deletion
  /// @returns Pointer to the minimum value node
  EventQueueNode *minValueNode(EventQueueNode *node)
  {
    EventQueueNode *current = node;

    while (current->left != NULL)
      current = current->left;

    return current;
  }

  /// Find the node to pop from the event queue
  /// @returns Pointer to the maximum value node
  EventQueueNode *maxValueNode(EventQueueNode *node)
  {
    EventQueueNode *current = node;

    if (current == NULL)
    {
      return current;
    }

    /* loop down to find the rightmost leaf */
    while (current->right != NULL)
      current = current->right;

    return current;
  }


  /// Delete the event point that is to be popped from the event queue
  EventQueueNode *deleteNode(EventQueueNode *root, double xc, double yc)
  {

    if (root == NULL)
      return root;

    if (yc < root->yc)
      root->left = deleteNode(root->left, xc, yc);

    else if (yc > root->yc)
      root->right = deleteNode(root->right, xc, yc);

    else if (xc > root->xc)
      root->left = deleteNode(root->left, xc, yc);
    else if (xc < root->xc)
      root->right = deleteNode(root->right, xc, yc);
    else
    {
      // node with only one child or no child
      if ((root->left == NULL) || (root->right == NULL))
      {
        EventQueueNode *temp = root->left ? root->left : root->right;

        if (temp == NULL)
        {
          // printf("Deleting last element\n");
          temp = root;
          root = NULL;
        }
        else
          *root = *temp;
        free(temp);
      }
      else
      {

        EventQueueNode *temp = minValueNode(root->right);

        // Copy the inorder successor's data to this node
        root->xc = temp->xc;
        root->yc = temp->yc;
        root->U = temp->U;
        root->C = temp->C;
        root->L = temp->L;
        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->xc, temp->yc);
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
  void preOrder(EventQueueNode *root)
  {
    if (root != NULL)
    {
      //printf("%f %f %d\n", root->xc, root->yc, root->height);
      cout << root->xc << " " << root->yc << " " << root->height << "\n";

      for (size_t i = 0; i < root->U.size(); i++)
      {
        //printf(" U:%f %f %f %f\n", root->U[i].startX, root->U[i].startY, root->U[i].endX, root->U[i].endY);
        cout << root->U[i].startX << " " << root->U[i].startY << " "
        << root->U[i].endX << " " << root->U[i].endY << "\n";
      }

      for (size_t i = 0; i < root->L.size(); i++)
      {
        //printf(" L:%f %f %f %f\n", root->L[i].startX, root->L[i].startY, root->L[i].endX, root->L[i].endY);
        cout << root->L[i].startX << " " << root->L[i].startY << " "
        << root->L[i].endX << " " << root->L[i].endY << "\n";
      }
      preOrder(root->left);
      preOrder(root->right);
    }
  }
};

#endif