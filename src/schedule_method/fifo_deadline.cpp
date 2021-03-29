#include"wmt.h"

namespace DAMCREM{
  bool WMT::operator<(const WMT &other) const {
    // ジョブの到着時刻での判定．
    if( this->query().received_time() < other.query().received_time() ){ return true; }
    if( this->query().received_time() > other.query().received_time() ){ return false; }
    // ジョブの到着時刻が同じなら，ジョブの最後のマクロタスクの実行完了までにかかる時間で判定．
    if( this->macrotask_exec_time_info().predicted_launched_to_deadline() > other.macrotask_exec_time_info().predicted_launched_to_deadline() ){ return true; }
    return false;
  }

  bool WMT::operator>(const WMT &other) const {
    // ジョブの到着時刻での判定．
    if( this->query().received_time() > other.query().received_time() ){ return true; }
    if( this->query().received_time() < other.query().received_time() ){ return false; }
    // ジョブの到着時刻が同じなら，ジョブの最後のマクロタスクの実行完了までにかかる時間で判定．
    if( this->macrotask_exec_time_info().predicted_launched_to_deadline() < other.macrotask_exec_time_info().predicted_launched_to_deadline() ){ return true; }
    return false;
  }

}

