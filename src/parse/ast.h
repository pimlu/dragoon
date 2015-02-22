#pragma once
#include "../global.h"

namespace ast {

class Node {

};

class Module {
public:
  string name;
private:
  friend std::ostream& operator<<(std::ostream&, const Module&);
};

}
