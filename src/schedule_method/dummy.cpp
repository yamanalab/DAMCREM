#include"wmt.h"

namespace DAMCREM{
  
bool WMT::operator<(const WMT &other) const {
  throw std::logic_error("Invalid");
  return false;
}

bool WMT::operator>(const WMT &other) const { 
  throw std::logic_error("Invalid");
  return false;
}

}
