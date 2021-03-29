#include"application/macrotask_exec_time_info_manager.h"

#include<regex>

namespace DAMCREM{
  MacrotaskExecTimeInfoManager::MacrotaskExecTimeInfoManager(const Option &option, uint64_t app_id,
                                                             const MacrotaskGraph &graph){
    bool is_success = true;
    const std::vector<std::string> app_exec_time_data
      = util::load_text_from_file(option.file_path_config().app_exec_time_data_file_path(app_id), is_success);
    if( !is_success ){
      return;
    }

    std::smatch m;
    info_.resize(graph.num_macrotasks()+1, nullptr);
    for( const std::string &x : app_exec_time_data ){
      std::vector<uint64_t> t;
      if( !std::regex_match(x, m, std::regex("\\s*\\d+\\s*=\\s*\\d+\\s*(,\\s*\\d+\\s*)*")) ){ continue; }
      const std::vector<std::string> ss = util::split_str(x, "=", true, true);
      util::transform(util::split_str<uint64_t>(ss.at(1), ","), std::back_inserter(t), [](uint64_t v){ return v; });
      size_t macrotask_id = std::stoul(ss.at(0));
      info_.at(macrotask_id) = std::make_shared<MacrotaskExecTimeInfo>(std::move(t), option, app_id);
    }

    for( size_t i = 1; i <= graph.num_macrotasks(); ++i ){
      if( graph.macrotask(i).macrotask_group() != i ){
        info_.at(i) = info_.at(graph.macrotask(i).macrotask_group());
      }
      total_exec_time_seq_for_prediction_ += info_.at(i)->exec_time_for_prediction();
    }
    
    // 全てのマクロタスクを逐次実行する場合のコスト
    auto update_macrotask_deadlineAll = [&](auto f, std::vector<size_t> next, std::unordered_set<size_t> &s) -> Time {
      uint64_t cost = 0;
      for( size_t p : next ){
        if( !(s.emplace(p).second) ){ continue; }
        cost += f(f, graph.macrotask(p).next(), s) + info_.at(p)->exec_time_for_prediction();
      }
      return cost;
    };

    // クリティカルパスのみ考慮する場合のコスト
    auto update_macrotask_deadlineCritical = [&](auto f, MacrotaskID macrotask_id,
                                                 std::unordered_map<MacrotaskID, Time> &cost) -> void {
      for( MacrotaskID p : graph.macrotask(macrotask_id).previous() ){
        Time nc = cost[macrotask_id] + info_.at(macrotask_id)->exec_time_for_prediction();
        if( cost[p] > nc ){ continue; }
        cost[p] = nc;
        f(f, p, cost);
      }
      return;
    };
    
    if( option.do_use_critical_path_for_prediction() ){
      std::unordered_map<MacrotaskID, Time> cost;
      for( MacrotaskID final_macrotask_id : graph.final_macrotask_id() ){
        update_macrotask_deadlineCritical(update_macrotask_deadlineCritical, final_macrotask_id, cost);
      }
      for( size_t i = 1; i <= graph.num_macrotasks(); ++i ){ info_.at(i)->set_deadline(cost.at(i)); }
      total_exec_time_for_prediction_ = cost.at(0);
    }else{
      for( size_t i = 1; i <= graph.num_macrotasks(); ++i ){
        std::unordered_set<size_t> us;
        info_.at(i)->set_deadline(update_macrotask_deadlineAll(update_macrotask_deadlineAll, graph.macrotask(i).next(), us));
      }
      total_exec_time_for_prediction_ = total_exec_time_seq_for_prediction_;
    }

    return;
  }



}

