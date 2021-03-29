#pragma once

#include"option.h"

namespace DAMCREM{
  class MacrotaskExecTimeInfo{
  public:
    MacrotaskExecTimeInfo(const Option &option, uint64_t app_id) : app_id_(app_id), option_(option){}
    MacrotaskExecTimeInfo(std::vector<uint64_t> &&exec_time, const Option &option, uint64_t app_id);
    MacrotaskExecTimeInfo(const MacrotaskExecTimeInfo &src) = delete;
    MacrotaskExecTimeInfo(MacrotaskExecTimeInfo &&src) noexcept = default;

    MacrotaskExecTimeInfo& operator=(MacrotaskExecTimeInfo &&src) noexcept = default;

    /*! @name Getter */
    /*! @{ */
    const Option& option() const noexcept { return option_.get(); }
    const std::vector<uint64_t>& exec_time() const noexcept { return exec_time_; }
    const std::vector<double>& efficiency() const noexcept { return efficiency_; }
    const std::vector<std::pair<double, size_t>>& sorted_candidates() const noexcept { return sorted_candidates_; }
    uint64_t exec_time_for_prediction() const noexcept { return exec_time_for_prediction_; }
    /*! @} */
    
    /*! @name Getter (index-based) */
    /*! @{ */
    uint64_t exec_time(size_t i) const { return exec_time_.at(i); }
    double efficiency(size_t i) const { return efficiency_.at(i); }
    /*! @} */

    const auto lower_bound_of_sorted_candidates(double e) const {
      return std::lower_bound(sorted_candidates_.begin(), sorted_candidates_.end(), e,
                              [](const auto& a, double b) { return a.first < b; });
    }

    Time predicted_launched_to_deadline() const noexcept { return predicted_launched_to_deadline_; }
    
    void set_deadline(uint64_t v) noexcept {
      deadline_ = v;
      predicted_launched_to_deadline_ = exec_time_for_prediction_ + deadline_;
    }

     
  private:
    void init();
    
    uint64_t predict_macrotask_exec_time() const {
      // 推測方式によって，どのスレッド数を使用した時の実行時間を使うかを変える
      if( option().do_use_max_threads_for_prediction() ){ return exec_time_.back(); }
      return exec_time_.at(1);
    }

    uint64_t app_id_;
    
    std::reference_wrapper<const Option> option_;
    
    std::vector<uint64_t> exec_time_;

    std::vector<double> efficiency_;

    //! Sorted candidate pairs of efficiency and the corresponding number of threads.
    std::vector<std::pair<double, size_t>> sorted_candidates_;

    //! Execution time for prediction
    uint64_t exec_time_for_prediction_ = 0;

    //! Deadline of the macrotask
    uint64_t deadline_ = 0;

    uint64_t predicted_launched_to_deadline_ = 0;

    
  };

}

