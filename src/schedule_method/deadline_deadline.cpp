#include"wmt.h"

namespace DAMCREM{
  bool WMT::operator<(const WMT &other) const {
    if( this->deadline_job() < other.deadline_job() ){ return true; }
    if( this->deadline_job() > other.deadline_job() ){ return false; }
    Time tt = this->deadline_job() - this->macrotask_exec_time_info().predicted_launched_to_deadline();
    Time ot = other.deadline_job() - other.macrotask_exec_time_info().predicted_launched_to_deadline();
    if( tt < ot ){ return true; }
    return false;
  }
  
  bool WMT::operator>(const WMT &other) const {
    if( this->deadline_job() > other.deadline_job() ){ return true; }
    if( this->deadline_job() < other.deadline_job() ){ return false; }
    Time tt = this->deadline_job() - this->macrotask_exec_time_info().predicted_launched_to_deadline();
    Time ot = other.deadline_job() - other.macrotask_exec_time_info().predicted_launched_to_deadline();
    if( tt > ot ){ return true; }
    return false;
  }

}

