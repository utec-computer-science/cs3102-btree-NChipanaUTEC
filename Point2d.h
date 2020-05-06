template<typename T>
class  Point_2d {
  typedef T value_t;
  value_t __x;
  value_t __y;

  public: 
    Point_2d(value_t _x = 0, value_t _y = 0)
    :__x(_x), __y(_y){
    }

    value_t x(){
      return __x;
    }

    value_t y(){
      return __y;
    }
};