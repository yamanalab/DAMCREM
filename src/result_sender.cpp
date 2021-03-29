#include"result_sender.h"

namespace DAMCREM{

void ResultSender::clear(){
  {
    std::lock_guard<std::mutex> lg(mutex_);
    queue_.clear();
    results_.erase(results_.begin(), results_.end());
  }
  return;
}

void ResultSender::add_result(Job &&job, bool do_add_into_queue){
  const AppID app_id = job.app_id();
  const ClientID client_id = job.query().client_id();
  const QueryID query_id = job.query().query_id();
  {
    std::lock_guard<std::mutex> lg(mutex_);
    if( do_add_into_queue ){
      queue_.emplace_back(std::make_shared<Job>(std::move(job)));
    }else{
      results_[app_id][client_id][query_id] = std::make_shared<Job>(std::move(job));
    }
  }
  return;
}

std::shared_ptr<Job> ResultSender::get_result(const AppID app_id, const ClientID client_id, const QueryID query_id){
  std::shared_ptr<Job> result;
  {
    std::lock_guard<std::mutex> lg(mutex_);
    auto itr0 = results_.find(app_id);
    if( itr0 == results_.end() ){ return nullptr; }
    auto itr1 = itr0->second.find(client_id);
    if( itr1 == itr0->second.end() ){ return nullptr; }
    auto itr2 = itr1->second.find(query_id);
    if( itr2 == itr1->second.end() ){ return nullptr; }
    result = itr2->second;
    itr1->second.erase(itr2);
  }
  return result;
}


}  // namespace DAMCREM

