#pragma once

#include<memory>

#include"application/macrotask_exec_time_info.h"
#include"application/macrotask_graph.h"

namespace DAMCREM{
  class MacrotaskExecTimeInfoManager{
  public:
    MacrotaskExecTimeInfoManager(const Option &option, uint64_t app_id, const MacrotaskGraph &graph);
    MacrotaskExecTimeInfoManager(const MacrotaskExecTimeInfoManager &src) noexcept = delete;
    MacrotaskExecTimeInfoManager(MacrotaskExecTimeInfoManager &&src) noexcept = default;

    const std::vector<std::shared_ptr<MacrotaskExecTimeInfo>>& info() const noexcept { return info_; }
    const MacrotaskExecTimeInfo& info(size_t macrotask_id) const noexcept { return *(info_.at(macrotask_id)); }

    uint64_t total_exec_time_seq_for_prediction() const noexcept { return total_exec_time_seq_for_prediction_; }

    Time total_exec_time_for_prediction() const noexcept { return total_exec_time_for_prediction_; }

  private:
    std::vector<std::shared_ptr<MacrotaskExecTimeInfo>> info_;

    uint64_t total_exec_time_seq_for_prediction_ = 0;

    Time total_exec_time_for_prediction_ = 0;

  };


}

