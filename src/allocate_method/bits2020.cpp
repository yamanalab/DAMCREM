#include"executor.h"

namespace DAMCREM{
template<class TimeManagerClass>
void Executor<TimeManagerClass>::calculate_allocated_computational_resource(){
  size_t num_waiting_macrotasks = mq_.waiting_queue().size();

  size_t available_num_threads =
    option_.max_computational_resource().num_threads() - current_computational_resource_.num_threads();

  if( available_num_threads <= num_waiting_macrotasks ){
    for( size_t i = 0; i < available_num_threads; ++i ){
      auto x = mq_.waiting_queue().top();
      ComputationalResource cr(1);
      mq_.exec_next_queue_emplace(std::move(x), cr);
      mq_.waiting_queue_pop_front();
    }
    return;
  }

  std::vector<WMT> queue_;
  for( size_t i = 0; i < num_waiting_macrotasks; ++i ){
    queue_.emplace_back(mq_.waiting_queue().top());
    mq_.waiting_queue_pop_front();
  }

  std::unordered_map<AppID, std::unordered_map<MacrotaskID, std::pair<size_t, size_t>>> macrotask_info;
  std::vector<double> priority_list;
  for( size_t i = 0; i < num_waiting_macrotasks; ++i ){
    const Application &app = queue_.at(i).app();
    AppID app_id = app.app_id();
    MacrotaskID macrotask_group_id = app.macrotask_graph().macrotask(queue_.at(i).macrotask_id()).macrotask_group();
    if( macrotask_info[app_id].count(macrotask_group_id) == 0 ){
      macrotask_info[app_id].emplace(std::piecewise_construct,
                                     std::forward_as_tuple(macrotask_group_id),
                                     std::forward_as_tuple(0, 1));
      for( const auto &t : app.macrotask_exec_time_info_manager().info(macrotask_group_id).sorted_candidates() ){
        priority_list.emplace_back(t.first);
      }
    }
    ++(macrotask_info[app_id].at(macrotask_group_id).first);
  }

    
  // 割当計算機資源量を二分探索で求める．
  std::sort(priority_list.begin(), priority_list.end(), std::greater<double>());
  size_t l = 0, m = 0, r = std::distance(priority_list.begin(), std::unique(priority_list.begin(), priority_list.end()));
  size_t total_num_threads = 0;

  auto count_total_num_threads = [&](double th){
    size_t tnt = 0;
    for( auto &[app_id, info_of_app] : macrotask_info ){
      const Application &app = app_list_.applications(app_id);
      for( auto &[macrotask_group_id, info] : info_of_app ){
        info.second = app.macrotask_exec_time_info_manager().info(macrotask_group_id).lower_bound_of_sorted_candidates(th)->second;
        tnt += info.first * info.second;
      }
    }
    return tnt;
  };
    
  while( l + 1 < r ){
    m = (l+r) / 2;
    total_num_threads = count_total_num_threads(priority_list.at(m));
    (total_num_threads <= available_num_threads ? l : r) = m;
    if( total_num_threads == available_num_threads ){ break; }
  }
  // m=l+1において，total_num_threads(l)<available_num_threads<total_num_threads(m)が成り立つ場合が存在する．
  if( l != m ){
    total_num_threads = count_total_num_threads(priority_list.at(l));
  }
  assert(total_num_threads<=available_num_threads);
    
  // 次に実行するマクロタスクのキューに追加する．
  for( size_t i = 0; i < num_waiting_macrotasks; ++i ){
    const Application &app = queue_.at(i).app();
    AppID app_id = app.app_id();
    MacrotaskID macrotask_group_id = app.macrotask_graph().macrotask(queue_.at(i).macrotask_id()).macrotask_group();
    ComputationalResource cr(macrotask_info.at(app_id).at(macrotask_group_id).second);
    assert(macrotask_info.at(app_id).at(macrotask_group_id).second > 0);
    mq_.exec_next_queue_emplace(std::move(queue_.at(i)), cr);
  }

  return;
}
   
  
}

