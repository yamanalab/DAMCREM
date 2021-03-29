#include"executor.h"

namespace DAMCREM{
template<class TimeManagerClass>
void Executor<TimeManagerClass>::calculate_allocated_computational_resource(){
  size_t l = mq_.waiting_queue().size();
  size_t available_num_threads =
    option_.max_computational_resource().num_threads() - current_computational_resource_.num_threads();
  for( size_t i = 0; i < l; ++i ){
    auto x = mq_.waiting_queue().top();
    size_t nt = x.macrotask_exec_time_info().exec_time().size()-1;
    if( available_num_threads < nt ){
      break;
    }
    ComputationalResource cr(nt);
    available_num_threads -= cr.num_threads();
    mq_.exec_next_queue_emplace(std::move(x), cr);
    mq_.waiting_queue_pop_front();
  }
  return;
}
}  // namespace DAMCREM

