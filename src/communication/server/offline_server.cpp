#include"communication/server/offline_server.h"

#include"communication/server/callback_function.h"
#include"communication/server/state.h"
#include"communication/shared_info.h"

namespace DAMCREM{
namespace communication{

template<class TimeManagerClass>
void OfflineServer<TimeManagerClass>::start(){
  init_offline_queue();
  this->time_manager_.init_system_start_time();
  this->run();
  return;
}

template<class TimeManagerClass>
std::deque<Query> OfflineServer<TimeManagerClass>::get_queries(const size_t n, const Time current_time){
  for( size_t i = 0; (i < n) && (queue_.size() > 0) && (current_time >= queue_.front().first); ++i ){
    receive_query();
  }
  return this->query_receiver_.get_queries(n, current_time);
}

template<class TimeManagerClass>
void OfflineServer<TimeManagerClass>::add_result(Job &&job, const bool do_add_into_queue){
  next_client_id_.at(job.query().app_id()).emplace_back(job.query().client_id());
  this->result_sender_.add_result(std::move(job), do_add_into_queue);
  return;
}

template<class TimeManagerClass>
void OfflineServer<TimeManagerClass>::init_offline_queue(){
  for( const auto &[app_id, file_info] : this->option_.file_path_config().query_arrival_time_file_info() ){
    util::for_each(load_received_time(file_info.first, file_info.second), [&](Time rt){ queue_.emplace_back(rt, app_id); });
  }
  std::sort(queue_.begin(), queue_.end(), std::less<std::pair<Time, AppID>>());
  
  // 入力データの作成
  const auto &fpc = this->option_.file_path_config();
  for( const auto &[app_id, client_id_list] : this->option_.valid_app_client_id_list() ){
    const auto &app = this->app_list_.applications(app_id);
    auto &ncid_list = next_client_id_[app_id];
    for( ClientID client_id : client_id_list ){
      next_client_id_[app_id].emplace_back(client_id);
      const auto &scheme = app.scheme(client_id);
      util::transform((fpc.template input_data_file_path_list<Plaintext>)(app_id, client_id, app.pt().num_input_data(), false),
                      std::back_inserter(input_pt_[app_id][client_id]),
                      [&](const auto &path){ return (scheme.template load_data<Plaintext>)(path); });
      util::transform((fpc.template input_data_file_path_list<Ciphertext>)(app_id, client_id, app.ct().num_input_data(), false),
                      std::back_inserter(input_ct_[app_id][client_id]),
                      [&](const auto &path){ return (scheme.template load_data<Ciphertext>)(path); });
    }
    for( size_t num_client = client_id_list.size(); ncid_list.size() < this->option_.max_num_executing_jobs(); ){
      std::copy(ncid_list.begin(), ncid_list.begin()+num_client, std::back_inserter(ncid_list));
    }
  }

  init_for_simulation();
  return;
}

template<>
void OfflineServer<TimeManager>::init_for_simulation(){ return; }

template<>
void OfflineServer<TimeManagerSimulator>::init_for_simulation(){
  util::for_each(queue_, [&](const auto &x){ time_manager_.push_next_event_occuring_time(x.first); });
}

template<class TimeManagerClass>
std::vector<Time> OfflineServer<TimeManagerClass>::load_received_time(const std::filesystem::path &file_path, size_t n) const {
  std::vector<Time> vec;
  std::vector<std::string> data = util::load_text_from_file(file_path);
  for( size_t i = 0; vec.size() < n; ++i ){
    if( (data.at(i).size() == 0) || (data.at(i)[0] == '#') ){ continue; }
    vec.emplace_back(util::convert_string<Time>(data.at(i)));
  }
  return vec;
}

template<class TimeManagerClass>
void OfflineServer<TimeManagerClass>::receive_query(){
  auto [received_time, app_id] = queue_.front();
  ClientID client_id = get_client_id(app_id);
  std::vector<Plaintext> pt = input_pt_.at(app_id).at((input_pt_.at(app_id).count(client_id) ? client_id : 0));
  std::vector<Ciphertext> ct = input_ct_.at(app_id).at((input_ct_.at(app_id).count(client_id) ? client_id : 0));
  this->query_receiver_.add_query(app_id, client_id, next_query_id_++, std::move(pt), std::move(ct), received_time);
  queue_.pop_front();
  return;
}

template class OfflineServer<TimeManager>;
template class OfflineServer<TimeManagerSimulator>;


}  // namespace communication
}  // namespace DAMCREM

