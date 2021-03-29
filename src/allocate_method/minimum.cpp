#include"executor.h"

namespace DAMCREM{
template<class TimeManagerClass>
void Executor<TimeManagerClass>::calculate_allocated_computational_resource(){
  size_t l = mq_.waiting_queue().size();
  ComputationalResource tmp_cr = current_computational_resource_;
  for( size_t i = 0; i < l; ++i ){
    auto x = mq_.waiting_queue().top();
    ComputationalResource cr(1);
    tmp_cr += cr;
    // 使用可能な計算機資源をオーバしていないかチェックする
    if( !(option_.max_computational_resource().is_valid(tmp_cr)) ){
      break;
    }
    mq_.exec_next_queue_emplace(std::move(x), cr);
    mq_.waiting_queue_pop_front();
  }
  return;
}
  

}

