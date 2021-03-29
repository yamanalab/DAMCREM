#pragma once

#include<deque>
#include<map>
#include<mutex>

#include"query.h"
#include"application/application_list.h"
#include"time_manager.h"
#include"time_manager_simulator.h"

namespace DAMCREM{
class Job{
public:
  Job(const ApplicationList &app_list, Query &&query);
    
  Job(const Job &src) = delete;

  //! mutexがあるため，単純にmoveするのは危険？何らかのチェック機能を付ける？
  Job(Job &&src) noexcept
    : app_(src.app_), query_(std::move(src.query_)), pt_(std::move(src.pt_)), ct_(std::move(src.ct_)),
      input_dependency_(std::move(src.input_dependency_)), final_macrotask_id_(std::move(src.final_macrotask_id_)),
      macrotask_exec_log_(std::move(src.macrotask_exec_log_)){
    mutex_input_dependency_ = std::make_unique<std::mutex[]>(app_.macrotask_graph().num_macrotasks()+1);
  }

  //! デッドラインを設定する関数
  void set_deadline();

  //! マクロタスクを実行する関数
  /*!
   * @param[in] macrotask_id 実行するマクロタスクのID
   * @param[in] cr 使用する計算機資源量
   * @param[in] current_time 現在時刻
   */
  void execute(size_t macrotask_id, const ComputationalResource &cr, const Time current_time);

  template<class TimeManagerClass>
  std::tuple<bool, std::vector<MacrotaskID>> finish_task(size_t macrotask_id, const TimeManagerClass &time_manager){
    auto result = finish_task(macrotask_id);
    macrotask_exec_log_.at(macrotask_id).second = time_manager.get_current_time;
    return result;
  }

  //! タスクの実行完了時の処理を行う関数
  /*!
    @param[in] macrotask_id 実行完了したマクロタスクのID
    @param[in] マクロタスクの実行が完了した時刻
    @return (ジョブの実行が完了したかどうか, 依存関係が解決したマクロタスクのID)
  */
  std::tuple<bool, std::vector<MacrotaskID>> finish_task(MacrotaskID macrotask_id, const Time current_time){
    auto result = finish_task(macrotask_id);
    macrotask_exec_log_.at(macrotask_id).second = current_time;
    return result;
  }

    
  //! ジョブの種類のIDを返す関数
  AppID app_id() const noexcept { return query_.app_id(); }
    
  //! ジョブのIDを返す関数
  JobID job_id() const noexcept { return query_.job_id(); }

  //! ジョブのクエリの到着時刻を返す関数
  Time received_time() const noexcept { return query_.received_time(); }

  //! ジョブの実行完了時刻を返す関数
  Time finish_time() const noexcept { return query_.finish_time(); }

  const std::vector<std::reference_wrapper<Ciphertext>>& get_results() const { return ct_.result_data(); }

  /*! @name Getter */
  /*! @{ */
  const Application& app() const noexcept { return app_; }
  const Query& query() const noexcept { return query_; }
  const OperandData<Plaintext>& pt() const noexcept { return pt_; }
  const OperandData<Ciphertext>& ct() const noexcept { return ct_; }
  const auto& macrotask_exec_log() const noexcept { return macrotask_exec_log_; }
  /*! @} */
    
  /*! @name Setter */
  /*! @{ */
  void finish_time(Time v){ query_.finish_time(v); }
  /*! @} */

  const std::pair<Time, Time>& macrotask_exec_log(size_t i) const noexcept {
    return macrotask_exec_log_.at(i);
  }

  const Scheme scheme() const { return app().scheme(query_.client_id()); }
    
private:
  //! タスクの実行完了時の処理を行う関数
  /*!
    @param[in] macrotask_id 実行完了したマクロタスクのID
    @return (ジョブの実行が完了したかどうか, 依存関係が解決したマクロタスクのID)
  */
  std::tuple<bool, std::vector<MacrotaskID>> finish_task(MacrotaskID macrotask_id);

  //! ジョブのテンプレート
  const Application &app_;

  //! 入力されたクエリ
  Query query_;

  OperandData<Plaintext> pt_;
    
  OperandData<Ciphertext> ct_;
    
  //! 直前の入力依存のマクロタスク(サイズが0になったら実行可能)
  std::vector<std::unordered_set<MacrotaskID>> input_dependency_;

  //! 依存関係解消時のロック用のmutex
  std::unique_ptr<std::mutex[]> mutex_input_dependency_;
    
  //! 出力依存が存在しないマクロタスクの集合(サイズが0になったらジョブの実行が完了)
  std::unordered_set<MacrotaskID> final_macrotask_id_;

  //! 最終マクロタスクの実行完了状況を更新・確認する際に使用するmutex
  std::mutex mutex_final_macrotask_id_;

  // std::vector<std::pair<util::ClockTimePoint, util::ClockTimePoint>> macrotask_exec_log_;
  std::vector<std::pair<Time, Time>> macrotask_exec_log_;
    
};

}
