#pragma once

#include"macrotask.h"

namespace DAMCREM{
  class MacrotaskGraph{
  public:
    MacrotaskGraph(size_t num_macrotasks, const std::unordered_map<std::string, std::string> &app_data);
    MacrotaskGraph(const MacrotaskGraph &src) noexcept = delete;
    MacrotaskGraph(MacrotaskGraph &&src) noexcept = default;

    size_t num_macrotasks() const noexcept { return num_macrotasks_; }
    const std::vector<Macrotask>& macrotask() const noexcept { return macrotask_; }
    const std::unordered_set<size_t> final_macrotask_id() const noexcept { return final_macrotask_id_; }

    const Macrotask& macrotask(size_t i) const { return macrotask_.at(i); }
    
  private:
    const size_t num_macrotasks_;

    std::vector<Macrotask> macrotask_;

    std::unordered_set<size_t> final_macrotask_id_;

    
  };


}

