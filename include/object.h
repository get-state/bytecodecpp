#ifndef CPPLOX_OBJ
#define CPPLOX_OBJ

#include <concepts>
#include <memory>

class cloxObj {
public:
  cloxObj(){};

  virtual ~cloxObj(){

  };

  virtual void free() = 0;

private:
  /* data */
};

class cloxString : cloxObj {
public:
  cloxString() { data = nullptr; };

  cloxString(std::string &s) { this->data = new std::string(s); }

  cloxString(std::string s) { data = new std::string(s); }

  cloxString(const cloxString &cloxStr) { data = new std::string(*cloxStr.data); }

  cloxString(cloxString &&cloxStr) {
    data = cloxStr.data;
    cloxStr.data = nullptr;
  }
  virtual ~cloxString() override { this->free(); };

  virtual void free() override { 
	delete data;
  	data = nullptr;
  };

  std::string &getStr() { return *data; }

private:
  /* data */
  std::string* data;
};

#endif /* ifndef CPPLOX_OBJ  */
