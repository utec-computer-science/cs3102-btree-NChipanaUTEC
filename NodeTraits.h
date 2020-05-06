#ifndef NODE_TRAITS_H
#define NODE_TRAITS_H

#include <iostream>
#include "BNode.h"
#include "BPlusNode.h"
#include "BSTNode.h"
using namespace std;

template <typename Node, typename ValueNode>
struct NodeTraits{
  static const int  flag_type = 0;
};

template <typename ValueNode>
struct NodeTraits< BSTNode<ValueNode>, ValueNode >{
  static const int  flag_type = BST_NODE_FLAGXX;
  
};

template <typename ValueNode> 
struct NodeTraits< BNode<ValueNode>, ValueNode >{
  static const int  flag_type = B_NODE_FLAGXX;
};

// BPlusNode
template <typename ValueNode> 
struct NodeTraits< BPlusNode<ValueNode>, ValueNode >{
  static const int  flag_type = B_PLUS_NODE_FLAGXX;
};

#endif