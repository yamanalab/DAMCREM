#include"wmt.h"

namespace DAMCREM{
  
  bool WMT::operator<(const WMT &other) const {
    // ジョブの到着時刻での判定．
    if( this->deadline_job() < other.deadline_job() ){ return true; }
    if( this->deadline_job() > other.deadline_job() ){ return false; }
    // マクロタスクがキューに入った時刻を利用．
    if( this->ready_time() < other.ready_time() ){ return true; }
    return false;
  }

  bool WMT::operator>(const WMT &other) const {
    // ジョブの到着時刻での判定．
    if( this->deadline_job() > other.deadline_job() ){ return true; }
    if( this->deadline_job() < other.deadline_job() ){ return false; }
    // マクロタスクがキューに入った時刻を利用．
    if( this->ready_time() > other.ready_time() ){ return true; }
    return false;
  }

}

