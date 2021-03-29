#pragma once

#include"job.h"

namespace DAMCREM{
class ResultSender{
public:
  ResultSender(const Option &option) : option_(option){};
  ResultSender(const ResultSender &src) = delete;
  ResultSender(ResultSender &&src) = delete;
    
  void clear();
  
  void add_result(Job &&job, bool do_add_into_queue);
  
  /*!
   * @return The pointer of the target job in the queue. Or nullptr if it does not exist.
   */
  std::shared_ptr<Job> get_result(const AppID app_id, const ClientID client_id, const QueryID query_id);

  //! @note 非スレッドセーフ
  const std::deque<std::shared_ptr<Job>> queue() const { return queue_; }
  //! @note 非スレッドセーフ
  const NestedUnorderedMap<AppID, ClientID, QueryID, std::shared_ptr<Job>> results() const { return results_; }
  
private:
  const Option &option_;
    
  //! Queue of jobs that will be sent to client without result request from clients
  std::deque<std::shared_ptr<Job>> queue_;

  //! A set of jobs that will be sent to client after receiving result request from clients
  NestedUnorderedMap<AppID, ClientID, QueryID, std::shared_ptr<Job>> results_;

  std::mutex mutex_;
};

}

