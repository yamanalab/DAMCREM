#include"communication/server/server.h"

#include"communication/server/callback_function.h"
#include"communication/server/state.h"
#include"communication/shared_info.h"

namespace DAMCREM{
namespace communication{

template<class TimeManagerClass>
Server<TimeManagerClass>::Server(Option &&option)
  : option_(std::move(option)), scheme_list_(option_), app_list_(option_, scheme_list_), 
    query_receiver_(option_), result_sender_(option_), executor_(time_manager_, option_, app_list_),
    sc_param_(std::make_shared<ServerCallbackParam>()),
    ssc_param_(std::make_shared<ServerSharedCallbackParam>(option_, scheme_list_, app_list_,
                                                           query_receiver_, result_sender_, time_manager_)),
    state_(std::make_shared<ServerStateReady>()),
    cf_container_(create_callback_function_container()){
  server_ = std::make_shared<stdsc::Server<>>(option_.server_info().port().c_str(), state_, cf_container_);
  return;
}

template<class TimeManagerClass>
void Server<TimeManagerClass>::start(){
  const bool enable_async_mode = true;
  server_->start(enable_async_mode);
  time_manager_.init_system_start_time();
  auto f = [this](){
    this->run();
    return;
  };
  future_of_executor_ = std::async(std::launch::async, f);
  return;
}

template<class TimeManagerClass>
stdsc::CallbackFunctionContainer Server<TimeManagerClass>::create_callback_function_container(){
  std::cerr << "create callback function container" << std::endl;
  stdsc::CallbackFunctionContainer cf_container;
  auto cfc_set = [&cf_container](ControlCode cc, auto *func){
    std::shared_ptr<stdsc::CallbackFunction> cf(func);  // Temporary variable, but it is OK, because cf_container copy it.
    cf_container.set(static_cast<uint64_t>(cc), cf);
  };
    
  // Configure callback functions
  cfc_set(ControlCode::kDataUpdateScheme, new CallbackFunctionForUpdateScheme());
  cfc_set(ControlCode::kDataDeleteScheme, new CallbackFunctionForDeleteScheme());
  cfc_set(ControlCode::kDataUpdateConstData, new CallbackFunctionForUpdateConstData());
  cfc_set(ControlCode::kDataDeleteConstData, new CallbackFunctionForDeleteConstData());
  cfc_set(ControlCode::kUpDownloadNewClientID, new CallbackFunctionForNewClientID());
  cfc_set(ControlCode::kUpDownloadQuery, new CallbackFunctionForQuery());
  cfc_set(ControlCode::kUpDownloadResult, new CallbackFunctionForResult());

  cf_container_.set_commondata(static_cast<void*>(sc_param_.get()), sizeof(*sc_param_));
  cf_container_.set_commondata(static_cast<void*>(ssc_param_.get()), sizeof(*ssc_param_),
                               stdsc::CommonDataKind_t::kCommonDataOnAllConnection);

  return cf_container;
}

template<class TimeManagerClass>
void Server<TimeManagerClass>::run(){
  executor_.init();
  while( executor_.is_continue() || is_continue() ){
    time_manager_.update_time();
    util::for_each(executor_.update_executing_jobs(),
                   [&]( auto &finished_job){ result_sender_.add_result(std::move(finished_job), false); });
    executor_.update_queries(get_queries(executor_.num_acceptable_queries(), time_manager_.get_current_time()));
    executor_.execute_tasks();
  }
  return;
}

template class Server<TimeManager>;
template class Server<TimeManagerSimulator>;

}  // namespace communication
}  // namespace DAMCREM
