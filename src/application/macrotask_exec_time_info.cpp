#include"application/macrotask_exec_time_info.h"

#include<cassert>

namespace DAMCREM{
  MacrotaskExecTimeInfo::MacrotaskExecTimeInfo(std::vector<uint64_t> &&exec_time,
                                               const Option &option, uint64_t app_id)
    : app_id_(app_id), option_(option), exec_time_(std::move(exec_time)){
    if( exec_time_.size() <= 1 ){ return; }
    init();
    return;
  }

  void MacrotaskExecTimeInfo::init(){
    // Calculate efficiency
    efficiency_.emplace_back(0.0);
    for( size_t i = 1, n = exec_time_.size(); i < n; ++i ){
      efficiency_.emplace_back(1.0 * exec_time_.at(1) / (exec_time_.at(i) * i));
      sorted_candidates_.emplace_back(efficiency_.back(), i);
    }
    
    // Make candidates
    std::sort(sorted_candidates_.begin(), sorted_candidates_.end(), std::greater<std::pair<double, size_t>>{});
    size_t max_nt = 0;
    // Delete the numbers of allocated threads whose efficiency is not good.
    // e.g. if list of each first of en are [1.0, 0.9, 0.5, 0.7, 0.6, 0.5, 0.3] and lowerbound is 0.7, the candidate is {1, 2, 3, 4}.
    // Note, 3 is not selected during execution, because the efficiency is less than that of 4 threads allocation.
    auto result = std::partition_point(sorted_candidates_.begin(), sorted_candidates_.end(),
                                       [&](const auto &x){ return (x.first >= option().get_lowerbound_of_priority(app_id_)); });
    sorted_candidates_.erase(result, sorted_candidates_.end());
    result = std::remove_if(sorted_candidates_.begin(), result,
                            [&](const auto &x){ return (x.second < (max_nt = std::max(max_nt, x.second))); });
    sorted_candidates_.erase(result, sorted_candidates_.end());
    std::sort(sorted_candidates_.begin(), sorted_candidates_.end());

    exec_time_.erase(exec_time_.begin()+max_nt+1, exec_time_.end());
    efficiency_.erase(efficiency_.begin()+max_nt+1, efficiency_.end());
    
    // Set execution time for prediction
    exec_time_for_prediction_ = predict_macrotask_exec_time();

    predicted_launched_to_deadline_ = exec_time_for_prediction_ + deadline_;
    
    return;
  }



}

