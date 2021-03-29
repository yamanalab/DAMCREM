#include"executor.h"

namespace DAMCREM{
template<class TimeManagerClass>
void Executor<TimeManagerClass>::calculate_allocated_computational_resource(){
  size_t l = mq_.waiting_queue().size();
  size_t available_num_threads =
    option_.max_computational_resource().num_threads() - current_computational_resource_.num_threads();
  l = (l<=available_num_threads ? l : available_num_threads);
  for( size_t i = 0; i < l; ++i ){
    auto x = mq_.waiting_queue().top();
    ComputationalResource cr(1);
    mq_.exec_next_queue_emplace(std::move(x), cr);
    mq_.waiting_queue_pop_front();
  }
  return;
}
  

}  // namespace DAMCREM

