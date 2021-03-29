#pragma once

#include"communication/server/server_callback_param.h"
#include"executor.h"
#include"option.h"
#include"time_manager.h"
#include"time_manager_simulator.h"

#include"stdsc/stdsc_callback_function_container.hpp"
#include"stdsc/stdsc_server.hpp"
#include"stdsc/stdsc_state.hpp"
#include"communication/stdsc_all.h"

namespace DAMCREM{
namespace communication{
template<class TimeManagerClass=TimeManager>
class Server{
public:
  Server(Option &&option);
  virtual ~Server() = default;
  
  virtual void start();

  virtual void stop(){ server_->stop(); }

  virtual void wait(){ server_->wait(); }

  // protectedへ移動
  virtual bool is_continue(){ return true; }
  
  virtual std::deque<Query> get_queries(const size_t n, const Time current_time){
    return query_receiver_.get_queries(n, current_time);
  }
  
  virtual void add_result(Job &&job, const bool do_add_into_queue){
    result_sender_.add_result(std::move(job), do_add_into_queue);
  }

protected:
  virtual stdsc::CallbackFunctionContainer create_callback_function_container();

  void run();

  TimeManagerClass time_manager_;
  
  Option option_;

  SchemeList scheme_list_;

  ApplicationList app_list_;

  QueryReceiver query_receiver_;

  ResultSender result_sender_;

  Executor<TimeManagerClass> executor_;

  std::future<void> future_of_executor_;
  
  std::shared_ptr<ServerCallbackParam> sc_param_;

  std::shared_ptr<ServerSharedCallbackParam> ssc_param_;

  stdsc::StateContext state_;
  
  stdsc::CallbackFunctionContainer cf_container_;
  
  std::shared_ptr<stdsc::Server<>> server_;

};

}  // namespace communication
}  // namespace DAMCREM

