#ifndef BNODE_H
#define BNODE_H

#include <iostream>
#include <vector>
#include "Macros.h"
using namespace std;

template<typename T>
class BNode {
  public:
  typedef T value_t ;
  typedef vector<value_t> container_t;
  typedef vector<BNode*> pcontainer_t;

  unsigned int order;
  container_t data;
  //BNode** children;
  pcontainer_t children;
  size_t sizeOfContainer;

  BNode( unsigned int order = 4)
  :order(order){
    //children = new BNode*[B_NODE_ORDER];
    children.resize(order,nullptr);
    data.resize(order, -1);
    sizeOfContainer = 0;
  }

  bool isFull(){
    return sizeOfContainer==order-1;
  }

  bool isLeaf(){
    for(int i = 0; i < children.size(); i++){
      if(children[i]!=NULL){return false;}
    }
    return true;
  }

  void pushBack(value_t val,BNode<T>* pointer){
    data.insert(data.begin(),val);
    children.insert(children.begin(),pointer);
    if(*(data.end()-1)==-1){data.erase(data.end()-1);}
    if(*(children.end()) == NULL){children.erase(children.end()-1);}
    sizeOfContainer++;
  }

  void pushPointer(BNode<T>* pointer){
    children.insert(children.begin(),pointer);
    if(*(children.end()) == NULL){children.erase(children.end()-1);}
  }

  void insertAt(value_t index, value_t val){
    data.insert(data.begin()+index,val);
    if(*(data.end()-1)==-1){data.erase(data.end()-1);}
    sizeOfContainer++;
  }

  BNode<T>* getNextChild(const value_t& val = 0){
    int i;
    for(i = 0; i < children.size();i++){
      if(children[i] != NULL && (val <= data[i] || data[i]==-1)){
        return children[i];
      }
    }
    return children[i];
  }

  BNode<T>* getNextLeaf(const value_t& val = 0){
    BNode<T>* child = this;
    while(!child->isLeaf()){
      child = child->getNextChild(val);
    }
    return child;
  }

  void splitNode(bool fullPointers = false){
    value_t val = data[order/2-1];
    BNode<T>* leftNode = new BNode<T>;
    BNode<T>* rightNode = new BNode<T>;
    if(fullPointers)
      rightNode->pushPointer(children[order]);
    for(int i = data.size()-1; i >= 0; i--){
      if(data[i]<val){
        leftNode->pushBack(data[i],children[i]);
        data[i] = -1;
        sizeOfContainer--;
      }
      else if(data[i]>val){
        rightNode->pushBack(data[i],children[i]);
        data[i] = -1;
        sizeOfContainer--;
      }
      else{
        if(fullPointers){leftNode->pushPointer(children[i]);}
      }
    }
    for(int i = 2; i < children.size(); i++){
      children[i] = NULL;
    }
    data[0] = data[order/2-1];
    data[order/2-1] = -1;

    children[0] = leftNode;
    children[1] = rightNode;
  }

  void insertFull(const value_t& val = 0){
    for(int i = 0; i < data.size(); i++){
      if(val <= data[i] || data[i] == -1){
        insertAt(i,val);
        splitNode();
        break;
      }
    }
  }

  void insertNotFull(const value_t& val = 0){
    for(int i = 0; i < data.size(); i++){
      if(val <= data[i] || data[i] == -1){
        insertAt(i,val);
        break;
      }
    }
  }

  void reorganize(BNode<T>* root){
    BNode<T>* temp = root;
    if(temp == this){return;}
    value_t val = data[0];
    bool organized = false;
    int i;
    while(!organized){
      for(i = 0; i < temp->children.size();i++){
        if(temp->children[i] == this){
          temp->children[i] = children[0];
          temp->data[i] = val;
          temp->children[i+1] = children[1];
          delete this;
          temp->sizeOfContainer++;
          organized = true;
          break;
        }
      }
      if(organized){
        if(temp->sizeOfContainer==temp->order){
          temp->splitNode(true);
          temp->reorganize(root);
        }
        return;
      }
      for(int i = 0; i < temp->children.size();i++){
        if(temp->children[i] != NULL && (val <= temp->data[i] || temp->data[i]==-1)){
          temp = temp->children[i];
          break;
        }
      }
    }
  }

  void recursionPrint(BNode<T>* current,int size,std::ostream &out){
    if(current){
      for(int i = current->children.size()-1; i >= 0;i--){
        if(current->data[i]!=-1){
          for(int j = 0; j < size; j++){
            out<<"\t";
          }
          out<<current->data[i]<<"\n";
        }
        recursionPrint(current->children[i],size+1,out);
      }
    }
  }

  void nodePrint(std::ostream &out){
    recursionPrint(this,0,out);
  }

  ~BNode(void){}
};


#endif
