#pragma once

#include"query_receiver.h"
#include"result_sender.h"
#include"scheme_list.h"
#include"time_manager.h"

namespace DAMCREM{
namespace communication{

class ServerCallbackParam{
public:
  ServerCallbackParam() noexcept = default;
  ~ServerCallbackParam() = default;
private:
  
};



class ServerSharedCallbackParam{
public:
  ServerSharedCallbackParam(Option &option, SchemeList &scheme_list, ApplicationList &app_list,
                            QueryReceiver &query_receiver, ResultSender &result_sender, TimeManager &time_manager)
    : option_(option), scheme_list_(scheme_list), app_list_(app_list),
      query_receiver_(query_receiver), result_sender_(result_sender), time_manager_(time_manager){}
  ~ServerSharedCallbackParam() = default;
  ServerSharedCallbackParam(const ServerCallbackParam &src) noexcept = delete;
  ServerSharedCallbackParam(ServerCallbackParam &&src) noexcept = delete;

  Option& option() noexcept { return option_; }
  SchemeList& scheme_list() noexcept { return scheme_list_; }
  ApplicationList& app_list() noexcept { return app_list_; }
  QueryReceiver& query_receiver() noexcept { return query_receiver_; }
  ResultSender& result_sender() noexcept { return result_sender_; }
  TimeManager& time_manager() noexcept { return time_manager_; }
  
private:
  Option &option_;
  
  SchemeList &scheme_list_;

  ApplicationList &app_list_;

  QueryReceiver &query_receiver_;
  
  ResultSender &result_sender_;

  TimeManager &time_manager_;
};






}  // namespace communication
}  // namespace DAMCREM



