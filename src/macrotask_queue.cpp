#include"macrotask_queue.h"

namespace DAMCREM{

std::vector<ExecutingMacrotask> MacrotaskQueue::executing_queue_finished_emt(){
  static const std::chrono::seconds zero(0);
  size_t n = executing_queue_.size();
  std::vector<ExecutingMacrotask> result;
  ExecutingQueue tmp;
  std::swap(tmp, executing_queue_);
  result.reserve(n);

  for( size_t i = 0; i < n; ++i ){
    if( tmp.at(i).wait_for(zero) == std::future_status::timeout ){
      executing_queue_.emplace_back(std::move(tmp.at(i)));
    }else{
      result.emplace_back(tmp.at(i).get());
    }
  }

  return result;
}

std::vector<ExecutingMacrotask> MacrotaskQueue::executing_queue_simulator_finished_emt(const Time current_time){
  std::vector<ExecutingMacrotask> result;
  while( (executing_queue_simulator_.size()) && (std::get<0>(executing_queue_simulator_.top()) <= current_time) ){
    result.emplace_back(executing_queue_simulator_.top());
    executing_queue_simulator_.pop();
  }
  return result;
}


}

