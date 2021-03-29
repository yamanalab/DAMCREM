#pragma once

#include"macrotask_queue.h"
#include"query_receiver.h"
#include"result_printer.h"
#include"result_sender.h"
#include"time_manager.h"
#include"time_manager_simulator.h"

namespace DAMCREM{
template<class TimeManagerClass>
class Executor{
public:
  Executor(TimeManagerClass &time_manager, const Option &option, const ApplicationList &app_list)
    : time_manager_(time_manager), option_(option), app_list_(app_list){}
  virtual ~Executor() = default;
  Executor(const Executor &src) = delete;
  Executor(Executor &&src) = delete;

  //! 初期化を行う関数
  void init();

  //! 処理を継続する必要があるかどうかを判定する関数
  bool is_continue() const { return static_cast<bool>(mq_.size()); }
  
  //! 実行中のジョブに対する更新処理を行う関数
  std::vector<Job> update_executing_jobs();

  //! 新規に実行可能なジョブ数を返す関数
  size_t num_acceptable_queries(){ return option_.max_num_executing_jobs() - jobs_.size(); }
  
  //! 新規クエリに対する更新処理を行う関数
  void update_queries(std::deque<Query> &&queries);
    
  //! 実行待ちマクロタスクの一部もしくは全てを実行する関数
  void execute_tasks();

  const MacrotaskQueue& macrotask_queue() const { return mq_; }
  
private:
  //! 次に実行するマクロタスクへ割り当てる計算機資源量を決定する処理
  /*!
   * @note 別のソースファイルで実装し，リンクする．
   */
  void calculate_allocated_computational_resource();

  TimeManagerClass &time_manager_;
  
  const Option &option_;

  const ApplicationList &app_list_;
  
  ComputationalResource current_computational_resource_;

  //! 実行中のジョブ
  std::unordered_map<JobID, Job> jobs_;

  //! 各種マクロタスクのキュー
  MacrotaskQueue mq_;
};

template class Executor<TimeManager>;
template class Executor<TimeManagerSimulator>;

}  // namespace DAMCREM

