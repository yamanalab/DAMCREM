#include"query_receiver.h"

namespace DAMCREM{

std::deque<Query> QueryReceiver::get_queries(const size_t n, const Time current_time){
  std::deque<Query> queries;
  {
    std::lock_guard<std::mutex> lg(mutex_);
    if( queue_.size() <= n ){
      std::swap(queries, queue_);
    }else{
      for( size_t i = 0; i < n; ++i ){
        queries.emplace_back(std::move(queue_.at(0)));
        queue_.pop_front();
      }
    }
  }
  util::for_each(queries, [&](Query &query){ query.start_time(current_time); });
  return queries;
}

Time QueryReceiver::next_query_received_time(){
  Time rt = std::numeric_limits<Time>::max();
  {
    std::lock_guard<std::mutex> lg(mutex_);
    if( queue_.size() ){
      rt = queue_.front().received_time();
    }
  }
  return rt;
}

void QueryReceiver::add_query(AppID app_id, ClientID client_id, QueryID query_id,
                              std::vector<Plaintext> &&pt, std::vector<Ciphertext> &&ct, Time received_time){
  std::lock_guard<std::mutex> lg(mutex_);
  queue_.emplace_back(app_id, client_id, query_id, next_jid_++, std::move(pt), std::move(ct), received_time);
  return;
}

void QueryReceiver::add_query(Query &&query){
  std::lock_guard<std::mutex> lg(mutex_);
  std::cout << util::makeIndentedMessageData(query) << std::endl;
  queue_.emplace_back(query.app_id(), query.client_id(), query.query_id(), next_jid_++,
                      std::move(query.pt()), std::move(query.ct()), query.received_time());
  return;
}

  
}  // namespace DAMCREM

