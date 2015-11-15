template<typename T>
class Singleton : boost::noncopyable {
 public:
  static getInstance() {
   pthread_once(&ptonce, Singleton::init);
   return *this;
  }

 private:
  Singleton() {}
  ~Singleton() {}
  static init() {
    value_ = new T();
  }
 
 private:
  static pthread_once_t ptonce_;
  static T* value_;
}


