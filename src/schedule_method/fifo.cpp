#include"wmt.h"

namespace DAMCREM{
  
  bool WMT::operator<(const WMT &other) const {
    // マクロタスクがキューに入った時刻を利用．
    if( this->ready_time() < other.ready_time() ){ return true; }
    return false;
  }

  bool WMT::operator>(const WMT &other) const {
    // マクロタスクがキューに入った時刻を利用．
    if( this->ready_time() > other.ready_time() ){ return true; }
    return false;
  }

}

