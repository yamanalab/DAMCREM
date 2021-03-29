#pragma once

#include"application/application_list.h"
#include"communication/server/server.h"
#include"result_printer.h"

namespace DAMCREM{
namespace communication{
template<class TimeManagerClass=TimeManager>
class OfflineServer : public Server<TimeManagerClass> {
public:
  OfflineServer(Option &&option) : Server<TimeManagerClass>(std::move(option)), result_printer_(this->option_, this->scheme_list_){}
  virtual ~OfflineServer() = default;
  
  void start() override final;

  void stop() override final { return; }

  void wait() override final { return; }

  bool is_continue() override final { return (this->query_receiver_.num_queries() + queue_.size() > 0); }

  std::deque<Query> get_queries(const size_t n, const Time current_time=0) override final;
  
  void add_result(Job &&job, const bool do_add_into_queue) override final;

  // client_idが0なら常にclient_idを使い続ける．
  ClientID get_client_id(AppID app_id){
    ClientID result = next_client_id_.at(app_id).front();
    next_client_id_.at(app_id).pop_front();
    return result;
  }

  template<class... Args>
  void show_results(Args&&... args){
    result_printer_.show(result_printer_.get_sorted_all(this->result_sender_), std::forward<Args>(args)...);
  }
  
  const auto& queue() const { return queue_; }
  
private:
  void init_offline_queue();
  
  void init_for_simulation();
  
  std::vector<Time> load_received_time(const std::filesystem::path &file_path, size_t n) const;
  
  void receive_query();

  ResultPrinter result_printer_;
  
  //! オフライン評価時のキュー
  /*!
   * 要素は(クエリ到着時刻, app_id)
   */
  std::deque<std::pair<Time, AppID>> queue_;

  //! オフライン評価時における次のクエリのクライアント番号
  /*!
   * @note 異なるジョブ間で同じ鍵を使用したくない時に使用する．
   *       要素数はOptionクラスで指定する．
   */
  std::unordered_map<AppID, std::deque<ClientID>> next_client_id_;

  QueryID next_query_id_ = 0;
  
  //! app_id, client_idに対応する入力平文
  /*!
   * @note 1つのclient_idに1通りで問題ない？
   */
  std::unordered_map<AppID, std::unordered_map<ClientID, std::vector<Plaintext>>> input_pt_;

  //! app_id, client_idに対応する入力暗号文
  /*!
   * @note 1つのclient_idに1通りで問題ない？
   */
  std::unordered_map<AppID, std::unordered_map<ClientID, std::vector<Ciphertext>>> input_ct_;
};

}  // namespace communication
}  // namespace DAMCREM

