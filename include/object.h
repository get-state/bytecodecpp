#ifndef CPPLOX_OBJ
#define CPPLOX_OBJ

#include <concepts>
#include <memory>

class CloxObj {
public:
  CloxObj();
  virtual ~CloxObj() = 0;
virtual void on_del() = 0;

private:
  /* data */
};

class CloxString : public virtual CloxObj {
	CloxString();
	
	private:
		
};

#endif /* ifndef CPPLOX_OBJ  */
