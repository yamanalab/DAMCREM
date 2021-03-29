#include"wmt.h"

namespace DAMCREM{
  
  bool WMT::operator<(const WMT &other) const {
    // ジョブの到着時刻での判定．
    if( this->query().received_time() < other.query().received_time() ){ return true; }
    if( this->query().received_time() > other.query().received_time() ){ return false; }
    // マクロタスクがキューに入った時刻を利用．
    if( this->ready_time() < other.ready_time() ){ return true; }
    return false;
  }

  bool WMT::operator>(const WMT &other) const {
    // ジョブの到着時刻での判定．
    if( this->query().received_time() > other.query().received_time() ){ return true; }
    if( this->query().received_time() < other.query().received_time() ){ return false; }
    // マクロタスクがキューに入った時刻を利用．
    if( this->ready_time() > other.ready_time() ){ return true; }
    return false;
  }

}

