#include <iostream>
#include <vector>
#include <stack>
#include <time.h>
#include <chrono>
#include <stdio.h>
#include <unistd.h>

#include "Tree.h"


int main() {
  typedef BSTNode<float> bst_node;
  typedef BNode<int> b_node;
  typedef Tree<bst_node> bst_tree;
  typedef Tree<b_node> b_tree;

  b_tree tree(4);
  for(int i = 1; i <= 100; i++){
    tree.insert(i);
    cout << tree << endl;
  }
}
