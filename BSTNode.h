#ifndef BST_NODE_H
#define BST_NODE_H

#include <iostream>
#include <vector>
#include "Macros.h"
using namespace std;

template<typename T>
class BSTNode{
  public: 
  typedef T value_t;

  value_t data;
  BSTNode **children;

  BSTNode(value_t data){
    this->data = data;
    children = new BSTNode*[2];
  }

  ~BSTNode(void){}

};

#endif
