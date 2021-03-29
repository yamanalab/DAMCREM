#pragma once

#include"job.h"
#include"communication/data/query_c2s.h"

namespace DAMCREM{
  class QueryReceiver{
  public:
    QueryReceiver(const Option &option) : option_(option){}

    QueryReceiver(const QueryReceiver &src) = delete;
    QueryReceiver(QueryReceiver &&src) noexcept = delete;

    //! 指定された個数の実行待ちクエリを返す関数
    /*!
     * @param[in] n 取得するクエリの数
     * @param[in] current_time 現在時刻
     */
    std::deque<Query> get_queries(size_t n, const Time current_time);

    //! @note 非スレッドセーフ
    size_t num_queries() const { return queue_.size(); }

    [[deprecated]]
    Time next_query_received_time();
    
    void add_query(AppID app_id, ClientID client_id, QueryID query_id,
                   std::vector<Plaintext> &&pt, std::vector<Ciphertext> &&ct, Time received_time);

    void add_query(Query &&query);
    
  private:
    //! オプション
    const Option &option_;
    
    //! 現在受信し，実行待ちとなっているジョブのクエリ
    std::deque<Query> queue_;

    std::mutex mutex_;

    //! 次のクエリのジョブ番号
    JobID next_jid_ = 0;
    
  };



}

