#ifndef BPLUSNODE_H
#define BPLUSNODE_H

#include <iostream>
#include <vector>
#include "Macros.h"
using namespace std;

// BPlusNode
template<typename T>
class BPlusNode {
  public: 
  typedef T value_t ;
  typedef vector<value_t> container_t;

  unsigned int order;
  container_t data;
  BPlusNode** children;
  BPlusNode* next;

  BPlusNode(value_t data, unsigned int order = 4)
  :order(order){
    children = new BPlusNode*[order];
  }
  
  ~BPlusNode(void){}
};


#endif