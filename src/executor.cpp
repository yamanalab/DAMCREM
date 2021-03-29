#include"executor.h"

namespace DAMCREM{
template<class TimeManagerClass>
void Executor<TimeManagerClass>::init(){
  current_computational_resource_ = ComputationalResource();
  mq_.clear();
  jobs_.clear();
  return;
}

template<>
std::vector<Job> Executor<TimeManager>::update_executing_jobs(){
  static const std::chrono::seconds zero(0);
  std::vector<Job> result;

  auto finished_jobs = mq_.executing_queue_finished_emt<TimeManager>(time_manager_.get_current_time());
  
  for( auto &&x : finished_jobs ){
    auto [ft, emt, solved_dependency, is_end] = std::move(x);
    // 計算資源のアップデート
    current_computational_resource_ -= emt.computational_resource();
    
    // 依存関係が解決したマクロタスクを実行待ちマクロタスクキューへ追加する．
    util::for_each(solved_dependency,
                   [&](MacrotaskID macrotask_id){ mq_.waiting_queue_emplace(macrotask_id, option_, emt.job(), ft); });

    // ジョブの実行完了時の処理
    if( is_end ){
      jobs_.at(emt.job_id()).finish_time(ft);
      result.emplace_back(std::move(jobs_.at(emt.job_id())));
      jobs_.erase(emt.job_id());
    }
  }

  return result;
}

template<class TimeManagerClass>
void Executor<TimeManagerClass>::update_queries(std::deque<Query> &&queries){
  // 新規ジョブの作成と追加
  for( Query &query : queries ){
    JobID job_id = query.job_id();
    Time start_time = query.start_time();
    jobs_.emplace(std::piecewise_construct,
                  std::forward_as_tuple(job_id),
                  std::forward_as_tuple(app_list_, std::move(query)));
    util::for_each(std::get<1>(jobs_.at(job_id).finish_task(0, time_manager_.get_current_time())),
                   [&](MacrotaskID macrotask_id){ mq_.waiting_queue_emplace(macrotask_id, option_, jobs_.at(job_id), start_time); });
  }
  return;
}

template<class TimeManagerClass>
void Executor<TimeManagerClass>::execute_tasks(){
  calculate_allocated_computational_resource();
  while( !(mq_.exec_next_queue().empty()) ){
    // メインスレッドでの段階で，参照を取得しておく．
    Job& target_job = jobs_.at(mq_.exec_next_queue().front().query().job_id());
    current_computational_resource_ += mq_.exec_next_queue().front().computational_resource();

    auto lf = [emt = std::move(mq_.exec_next_queue_pop_front_move()), &jb = target_job, &time_manager = time_manager_]() mutable {
      jb.execute(emt.macrotask_id(), emt.computational_resource(), time_manager.get_current_time());
      auto [is_end, solved_dependency] = jb.finish_task(emt.macrotask_id(), time_manager.get_current_time());
      return std::make_tuple(jb.macrotask_exec_log(emt.macrotask_id()).second, std::move(emt), std::move(solved_dependency), is_end);
    };
      
    mq_.executing_queue_emplace(std::async(std::launch::async, std::move(lf)));
  }
  return;
}

////////////////////////////////////////
// for TimeManagerSimulator
////////////////////////////////////////
template<>
std::vector<Job> Executor<TimeManagerSimulator>::update_executing_jobs(){
  static const std::chrono::seconds zero(0);
  std::vector<Job> result;

  auto finished_jobs = mq_.executing_queue_finished_emt<TimeManagerSimulator>(time_manager_.get_current_time());
  
  for( auto &&x : finished_jobs ){
    auto [ft, emt, dummy1, dummy2] = std::move(x);
    // 計算資源のアップデート
    current_computational_resource_ -= emt.computational_resource();
    auto [is_end, solved_dependency] = jobs_.at(emt.job_id()).finish_task(emt.macrotask_id(), ft);
    
    // 依存関係が解決したマクロタスクを実行待ちマクロタスクキューへ追加する．
    util::for_each(solved_dependency,
                   [&](MacrotaskID macrotask_id){ mq_.waiting_queue_emplace(macrotask_id, option_, emt.job(), ft); });

    // ジョブの実行完了時の処理
    if( is_end ){
      jobs_.at(emt.job_id()).finish_time(ft);
      result.emplace_back(std::move(jobs_.at(emt.job_id())));
      jobs_.erase(emt.job_id());
    }
  }

  return result;
}

template<>
void Executor<TimeManagerSimulator>::execute_tasks(){
  calculate_allocated_computational_resource();
  while( !(mq_.exec_next_queue().empty()) ){
    // メインスレッドでの段階で，参照を取得しておく．
    Job& target_job = jobs_.at(mq_.exec_next_queue().front().query().job_id());
    current_computational_resource_ += mq_.exec_next_queue().front().computational_resource();

    auto lf = [emt = std::move(mq_.exec_next_queue_pop_front_move()), &jb = target_job, &time_manager = time_manager_]() mutable {
      MacrotaskID macrotask_id = emt.macrotask_id();
      jb.execute(macrotask_id, emt.computational_resource(), time_manager.get_current_time());
      Time expected_finish_time = time_manager.get_current_time()
        + jb.app().macrotask_exec_time_info_manager().info(macrotask_id).exec_time(emt.computational_resource().num_threads());
      std::vector<MacrotaskID> solved_dependency;
      bool is_end = false;
      time_manager.push_next_event_occuring_time(expected_finish_time);
      return std::make_tuple(expected_finish_time, std::move(emt), std::move(solved_dependency), is_end);
    };

    mq_.executing_queue_simulator_emplace(std::move(lf()));
  }
  return;
}


}


