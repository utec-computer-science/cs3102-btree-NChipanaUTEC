#ifndef TREE_H
#define TREE_H

#include "BNode.h"
#include "BPlusNode.h"
#include "BSTNode.h"
#include "NodeTraits.h"
#include "TreeHelper.h"
#include <iostream>
using namespace std;

template <typename Node>
class Tree {
  public:
  typedef Node node_t;
  typedef typename node_t::value_t value_t;

  node_t* root;
  unsigned int order;

  Tree(unsigned int order = 0):root(nullptr), order(order){
  }

  void insert(const value_t& val){
    add<NodeTraits<node_t, value_t>::flag_type>(&root, val, order);
  }

  template <int Flag> 
  void add(node_t**, const value_t&, unsigned int order);

  template <int Flag> 
  void print(node_t**, std::ostream&);

  ~Tree(void){
    //delete [] root;
  }

  void  print(std::ostream &out){
    print<NodeTraits<node_t, value_t>::flag_type>(&root,out);
  }
};

template <typename Node> 
std::ostream& operator<<(std::ostream &out, Tree<Node> tree) {
    tree.print(out);
    return out;
}

template<typename Node> template <int Flag>
void Tree<Node>::add(typename Tree<Node>::node_t ** root, const typename Tree<Node>::value_t &val, unsigned int order) {
  TreeHelper<Tree<Node>::node_t,Tree<Node>::value_t,Flag>::insert(root, val, order);
}

template<typename Node> template <int Flag>
void Tree<Node>::print(typename Tree<Node>::node_t ** root, std::ostream &out) {
  TreeHelper<Tree<Node>::node_t,Tree<Node>::value_t,Flag>::print(root, out);
}

#endif
