#include <iostream>
#include <vector>

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
    void operator() (void){
      std::cout << "post order" << std::endl;
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
    void operator() (void){
      std::cout << "pre order" << std::endl;
    }
  };

  typedef binary_search functor_t;
  typedef pre_order_print print_t;
};

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

  ~BNode(void){}
};

template <typename T, int S>
class BTree {
public:
  typedef typename T::value_t value_t;
  typedef typename T::container_t container_t;
  typedef typename T::functor_t functor_t;
  typedef typename T::print_t print_t;

  BNode<T,S>* root;
  print_t print;
  functor_t search;

  BTree(void):root(NULL){
  }

  ~BTree(void){}

  void insertNotFull(BNode<T,S>*temp, const value_t& val = 0){
    for(int i = 0; i < temp->keys.size(); i++){
      if(val <= temp->keys[i] || temp->keys[i] == -1){
        temp->insertAt(i,val);
        break;
      }
    }
  }

  void insertFull(BNode<T,S>*temp, const value_t& val = 0){
    for(int i = 0; i < temp->keys.size(); i++){
      if(val <= temp->keys[i] || temp->keys[i] == -1){
        temp->insertAt(i,val);
        temp->splitNode();
        break;
      }
    }
  }

  void reorganize(BNode<T,S>* moveNode){
    BNode<T,S>* temp = root;
    if(temp == moveNode){return;}
    value_t val = moveNode->keys[0];
    bool organized = false;
    while(!organized){
      for(int i = 0; i < temp->ptrs.size();i++){
        if(temp->ptrs[i] == moveNode){
          temp->ptrs[i] = moveNode->ptrs[0];
          temp->keys[i] = val;
          temp->ptrs[i+1] = moveNode->ptrs[1];
          delete moveNode;
          temp->currentNodes++;
          organized = true;
          break;
        }
      }
      if(organized)break;
      for(int i = 0; i < temp->ptrs.size();i++){
        if(temp->ptrs[i] != NULL && (val <= temp->keys[i] || temp->keys[i]==-1)){
          temp = temp->ptrs[i];
          break;
        }
      }
    }
    if(temp->currentNodes==S){
      temp->splitNode(true);
    }
  }


  void insert(const value_t& val = 0){
    if(root == NULL){root = new BNode<T,S>;}

    BNode<T,S>* temp = root;

    while(!temp->isLeaf()){
      for(int i = 0; i < temp->ptrs.size();i++){
        if(temp->ptrs[i] != NULL && (val <= temp->keys[i] || temp->keys[i]==-1)){
          temp = temp->ptrs[i];
          break;
        }
      }
    }

    if(temp->isFull()){
      insertFull(temp,val);
      reorganize(temp);
    }
    else{
      insertNotFull(temp,val);
    }



    for(int j = 0; j < root->keys.size(); j++){
      if(root->keys[j]!=-1)
        std::cout<<root->keys[j]<<" ";
    }
    std::cout<<"\n";
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
        for(int i = 0; i < temp->ptrs.size();i++){
          if(temp->ptrs[i]!=NULL && (temp->keys[i]>=val||temp->keys[i]==-1)){
            temp = temp->ptrs[i];
            break;
          }
        }
      }
    }
  }

  template <typename _T, int _S>
  friend std::ostream& operator<<(std::ostream& out, BTree<_T,_S> tree){
    tree.print();// (out)
    // IN PRE POST LEVEL ORDER
    return out;
  }

};

int main() {
  typedef BS_Traits<int> btrait_t;
  typedef SS_Traits<int> strait_t;
  BTree<strait_t,4> tree;
  tree.insert(1);
  tree.insert(2);
  tree.insert(3);
  tree.insert(4);
  tree.insert(5);
  tree.insert(6);
  tree.insert(7);
  tree.insert(8);
  tree.insert(9);
  tree.insert(10);
  std::cout <<"Encontro 5: " <<tree.find(5)<<"\n";
  /*
  std::cout<<tree<< std::endl;
  typedef SS_Traits<float> strait_t;
  BTree<strait_t,10> stree;
  std::cout<<stree<< std::endl;*/
}
