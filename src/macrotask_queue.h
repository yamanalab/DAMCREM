#pragma once

#include<cassert>
#include<future>
#include<queue>

#include"emt.h"
#include"time_manager.h"
#include"time_manager_simulator.h"

namespace DAMCREM{
/*!
 * @note priority_queueよりもsetの方が良い？ただ，setだとおそらくpriority_queueよりもかなり遅い．
 *       また，priority_queueだと，要素のmoveができないので，コピーが走ってしまう．
 *       Executorクラスのメインスレッドから呼び出される前提のため，非スレッドセーフ．
 */
using WaitingQueue = std::priority_queue<WMT, std::vector<WMT>, std::greater<WMT>>;

using ExecNextQueue = std::deque<EMT>;

//! 実行時は追加や削除の高速化のために，dequeを使う．
/*!
 * @note dequeだと，中間要素の削除が結局遅いのでは？
 *       独自クラスのリストで，要素へのポインタでアクセスするようにする？
 *       例えば，要素削除の際に，前後の要素を参照して繋ぎ直すようにするなど．
 */
using ExecutingMacrotask = std::tuple<Time, EMT, std::vector<MacrotaskID>, bool>;
using ExecutingQueue = std::deque<std::future<ExecutingMacrotask>>;

//! 終了予定時刻とEMTのペア．高速化のために，シミュレーション時は終了予定時刻でソートする
// using ExecutingMacrotaskSimulator = std::tuple<Time, EMT, std::vector<MacrotaskID>, bool>;
using ExecutingQueueSimulator = std::priority_queue<ExecutingMacrotask,
                                                    std::vector<ExecutingMacrotask>,
                                                    std::greater<ExecutingMacrotask>>;

class MacrotaskQueue{
public:
  MacrotaskQueue(){}
  MacrotaskQueue(const MacrotaskQueue &src) = delete;
  MacrotaskQueue(MacrotaskQueue &&src) = default;
    
  //! キューを空にする関数
  void clear(){
    while( !waiting_queue_.empty() ){ waiting_queue_.pop(); }
    exec_next_queue_.clear();
    executing_queue_.clear();
    while( !executing_queue_simulator_.empty() ){ executing_queue_simulator_.pop(); }
    return;
  }

  //! 各キューの要素数の合計を返す関数．
  size_t size() const noexcept { return waiting_queue_.size() + exec_next_queue_.size() + executing_queue_.size() + executing_queue_simulator_.size(); }

  //! 要素を追加する．
  template<class... Args>
  void waiting_queue_emplace(Args&&... args){ waiting_queue_.emplace(std::forward<Args>(args)...); }
    
  //! 先頭要素を削除する
  void waiting_queue_pop_front(){ waiting_queue_.pop(); }

  //! 要素を追加する．
  template<class... Args>
  void exec_next_queue_emplace(Args&&... args){ exec_next_queue_.emplace_back(std::forward<Args>(args)...); }
    
  //! 先頭要素を削除する
  void exec_next_queue_pop_front(){ exec_next_queue_.pop_front(); }

  //! 先頭要素のmoveで返し，キューを詰める．
  EMT exec_next_queue_pop_front_move(){
    EMT mt = std::move(exec_next_queue_.front());
    exec_next_queue_pop_front();
    return mt;
  }

  
  //! 要素を追加する．
  template<class... Args>
  void executing_queue_emplace(Args&&... args){
    executing_queue_.emplace_back(std::forward<Args>(args)...);
  }

  //! 先頭要素を削除する
  void executing_queue_pop_front(){
    executing_queue_.pop_front();
  }
  
  //! 添字で指定された要素を削除する
  void executing_queue_erase(size_t i){
    assert(i < executing_queue_.size());
    executing_queue_.erase(executing_queue_.begin()+i);
  }

  //! 実行が完了した要素を返し，キューを詰める．
  std::vector<ExecutingMacrotask> executing_queue_finished_emt();

    
  //! 要素を追加する．
  template<class... Args>
  void executing_queue_simulator_emplace(Args&&... args){
    executing_queue_simulator_.emplace(std::forward<Args>(args)...);
  }
  
  void executing_queue_simulator_pop(){
    executing_queue_simulator_.pop();
  }

  //! 実行が完了した要素を返し，キューを詰める．
  std::vector<ExecutingMacrotask> executing_queue_simulator_finished_emt(const Time current_time);


  template<class TimeManagerClass>
  std::vector<ExecutingMacrotask> executing_queue_finished_emt(const Time current_time);

    
  /*! @name Getter */
  /*! @{ */
  WaitingQueue& waiting_queue() noexcept { return waiting_queue_; }
  const WaitingQueue& waiting_queue() const noexcept { return waiting_queue_; }
  ExecNextQueue& exec_next_queue() noexcept { return exec_next_queue_; }
  const ExecNextQueue& exec_next_queue() const noexcept { return exec_next_queue_; }
  ExecutingQueue& executing_queue() noexcept { return executing_queue_; }
  const ExecutingQueue& executing_queue() const noexcept { return executing_queue_; }
  ExecutingQueueSimulator& executing_queue_simulator() noexcept { return executing_queue_simulator_; }
  const ExecutingQueueSimulator& executing_queue_simulator() const noexcept { return executing_queue_simulator_; }
  /*! @} */
    
private:
  //! 実行待ちマクロタスクのキュー
  WaitingQueue waiting_queue_;

  //! 割当計算機資源量が確定したマクロタスクとその割当計算機資源量のペアのキュー(リスト)
  ExecNextQueue exec_next_queue_;

  //! 実行中マクロタスクのキュー
  ExecutingQueue executing_queue_;

  ExecutingQueueSimulator executing_queue_simulator_;
  
};

template<>
inline std::vector<ExecutingMacrotask>
MacrotaskQueue::executing_queue_finished_emt<TimeManager>(const Time current_time){
  return executing_queue_finished_emt();
}

template<>
inline std::vector<ExecutingMacrotask> MacrotaskQueue::executing_queue_finished_emt<TimeManagerSimulator>(const Time current_time){
  return executing_queue_simulator_finished_emt(current_time);
}



}

