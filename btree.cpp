#include <iostream>
#include <vector>
#include <type_traits>
#include <cstdint>

template <typename T, int S>
class BNode {
public:
  typedef typename T::value_t value_t;
  typedef typename T::container_t container_t;
  typedef typename T::iterator_t iterator_t;
  typedef std::vector< BNode<T,S>* > pcontainer_t;

  container_t keys;
  pcontainer_t ptrs;
  std::size_t  order;
  std::size_t currentNodes;

  BNode(void):order(S){
    keys=container_t(order,-1);
    ptrs=pcontainer_t(order,NULL);
    currentNodes=0;
  }
  BNode(value_t val):order(S){
    keys=container_t(order,-1);
    ptrs=pcontainer_t(order,NULL);
    currentNodes=1;
    keys[0] = val;
  }

  bool isFull(){
    return currentNodes==order-1;
  }

  bool isLeaf(){
    for(int i = 0; i < ptrs.size(); i++){
      if(ptrs[i]!=NULL){return false;}
    }
    return true;
  }

  void pushBack(value_t val,BNode<T,S>* pointer){
    keys.insert(keys.begin(),val);
    ptrs.insert(ptrs.begin(),pointer);
    if(*(keys.end()-1)==-1){keys.erase(keys.end()-1);}
    if(*(ptrs.end()) == NULL){ptrs.erase(ptrs.end()-1);}
    currentNodes++;
  }

  void pushPointer(BNode<T,S>* pointer){
    ptrs.insert(ptrs.begin(),pointer);
    if(*(ptrs.end()) == NULL){ptrs.erase(ptrs.end()-1);}
  }

  void insertAt(value_t index, value_t val){
    keys.insert(keys.begin()+index,val);
    if(*(keys.end()-1)==-1){keys.erase(keys.end()-1);}
    currentNodes++;
  }

  BNode<T,S>* getNextChild(const value_t& val = 0){
    for(int i = 0; i < ptrs.size();i++){
      if(ptrs[i] != NULL && (val <= keys[i] || keys[i]==-1)){
        return ptrs[i];
      }
    }
  }

  BNode<T,S>* getNextLeaf(const value_t& val = 0){
    BNode<T,S>* child = this;
    while(!child->isLeaf()){
      child = child->getNextChild(val);
    }
    return child;
  }

  void splitNode(bool fullPointers = false){
    value_t val = keys[S/2-1];
    BNode<T,S>* leftNode = new BNode<T,S>;
    BNode<T,S>* rightNode = new BNode<T,S>;
    if(fullPointers)
      rightNode->pushPointer(ptrs[S]);
    for(int i = keys.size()-1; i >= 0; i--){
      if(keys[i]<val){
        leftNode->pushBack(keys[i],ptrs[i]);
        keys[i] = -1;
        currentNodes--;
      }
      else if(keys[i]>val){
        rightNode->pushBack(keys[i],ptrs[i]);
        keys[i] = -1;
        currentNodes--;
      }
      else{
        if(fullPointers){leftNode->pushPointer(ptrs[i]);}
      }
    }
    for(int i = 2; i < ptrs.size(); i++){
      ptrs[i] = NULL;
    }
    keys[0] = keys[S/2-1];
    keys[S/2-1] = -1;

    ptrs[0] = leftNode;
    ptrs[1] = rightNode;
  }

  void insertFull(const value_t& val = 0){
    for(int i = 0; i < keys.size(); i++){
      if(val <= keys[i] || keys[i] == -1){
        insertAt(i,val);
        splitNode();
        break;
      }
    }
  }

  void insertNotFull(const value_t& val = 0){
    for(int i = 0; i < keys.size(); i++){
      if(val <= keys[i] || keys[i] == -1){
        insertAt(i,val);
        break;
      }
    }
  }

  void reorganize(BNode<T,S>* root){
    BNode<T,S>* temp = root;
    if(temp == this){return;}
    value_t val = keys[0];
    bool organized = false;
    int i;
    while(!organized){
      for(i = 0; i < temp->ptrs.size();i++){
        if(temp->ptrs[i] == this){
          temp->ptrs[i] = ptrs[0];
          temp->keys[i] = val;
          temp->ptrs[i+1] = ptrs[1];
          delete this;
          temp->currentNodes++;
          organized = true;
          break;
        }
      }
      if(organized){
        if(temp->currentNodes==S){
          temp->splitNode(true);
          temp->reorganize(root);
        }
        return;
      }
      for(int i = 0; i < temp->ptrs.size();i++){
        if(temp->ptrs[i] != NULL && (val <= temp->keys[i] || temp->keys[i]==-1)){
          temp = temp->ptrs[i];
          break;
        }
      }
    }
  }

  void recursionPrint(BNode<T,S>* current,int size){
    if(current){
      for(int i = current->ptrs.size()-1; i >= 0;i--){
        if(current->keys[i]!=-1){
          for(int j = 0; j < size; j++){
            std::cout<<"\t";
          }
          std::cout<<current->keys[i]<<"\n";
        }
        recursionPrint(current->ptrs[i],size+1);
      }
    }
  }

  void nodePrint(){
    recursionPrint(this,0);
  }

  ~BNode(void){}
};

template <typename T>
class SS_Traits{
public:
  typedef T  value_t;
  typedef std::vector<value_t> container_t;
  typedef typename container_t::iterator iterator_t;

  class simple_search{
  public:
    int operator() (container_t a, value_t v){
      for(int i = 0; i < a.size(); i++){
        if(a[i] == v){return i;}
      }
      return -1;
    }
  };

  class post_order_print{
  public:
    void operator() (container_t keys, int size){
      for(auto it: keys){
        if(it!=-1){
          for(int i = 0; i < size; i++){
            std::cout << "\t";
          }
          std::cout<<it<<"\n";
        }
      }
    }
  };

  typedef simple_search functor_t;
  typedef post_order_print print_t;
};

template <typename T>
class BS_Traits{
public:
  typedef T  value_t;
  typedef std::vector<value_t> container_t;
  typedef typename container_t::iterator iterator_t;

  class binary_search{
  public:
    int operator() (container_t a, value_t v){
      int left = 0;
      int right = a.size()-1;
      int pivot;
      while(left <= right){
        pivot = left + (right-left)/2;
        if(a[pivot]==v){
          return pivot;
        }
        else if(a[pivot] < v){
          left = pivot + 1;
        }
        else{
          right = pivot - 1;
        }
      }
      return -1;
    }
  };

  class pre_order_print{
  public:
    void operator() (container_t keys, int size){
      for(auto it: keys){
        if(it!=-1){
          for(int i = 0; i < size; i++){
            std::cout << "\t";
          }
          std::cout<<it<<"\n";
        }
      }
    }
  };

  typedef binary_search functor_t;
  typedef pre_order_print print_t;
};

template <typename T, int S>
class BTree {
public:
  typedef typename T::value_t value_t;
  typedef typename T::container_t container_t;
  typedef typename T::functor_t functor_t;
  typedef typename T::print_t print_t;

  BNode<T,S>* root;
  //print_t print;
  functor_t search;

  BTree(void):root(NULL){
  }

  ~BTree(void){}


  void print(BNode<T,S>* current){
    current->nodePrint();
  }


  void insert(const value_t& val = 0){
    if(root == NULL){
      root = new BNode<T,S>;
    }
    BNode<T,S>* head = root;

    head = head->getNextLeaf(val);

    if(head->isFull()){
      head->insertFull(val);
      head->reorganize(root);
    }
    else{
      head->insertNotFull(val);
    }

  }

  bool find(const value_t& val = 0){
    BNode<T,S>* temp = root;
    functor_t bs;
    while(true){
      if(bs(temp->keys,val) != -1){
        return true;
      }
      else{
        if(temp->isLeaf())return false;
        temp = temp->getNextChild(val);
      }
    }
  }

  template <typename _T, int _S>
  friend std::ostream& operator<<(std::ostream& out, BTree<_T,_S> tree){
    tree.print(tree.root);
    // postPrint(tree.root);
    return out;
  }

};

int main() {
  typedef BS_Traits<int> btrait_t;
  typedef SS_Traits<int> strait_t;
  BTree<strait_t,4> tree;
  for(int i = 1; i <= 100; i ++){
    tree.insert(i);
  }
  std::cout <<"Encontro 48: " <<tree.find(48)<<"\n";
  std::cout<<tree<< std::endl;
}
