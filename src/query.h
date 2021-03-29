#pragma once

#include"scheme_list.h"

namespace DAMCREM{
  class Query{
  public:
    Query(){}
    Query(AppID app_id, ClientID client_id, QueryID query_id, JobID job_id,
          std::vector<Plaintext> &&pt, std::vector<Ciphertext> &&ct, Time received_time)
      : app_id_(app_id), client_id_(client_id), query_id_(query_id), job_id_(job_id),
        pt_(std::move(pt)), ct_(std::move(ct)), received_time_(received_time){}

    Query(const Query &src) = delete;
    Query(Query &&src) = default;

    bool read_from_stream(std::istream &stream);
    
    bool write_to_stream(std::ostream &stream) const;

    size_t stream_size() const;
    
    /*! @name Getter */
    /*! @{ */
    AppID app_id() const noexcept { return app_id_; }
    ClientID client_id() const noexcept { return client_id_; }
    QueryID query_id() const noexcept { return query_id_; }
    JobID job_id() const noexcept { return job_id_; }
    std::vector<Plaintext>& pt() noexcept { return pt_; }
    const std::vector<Plaintext>& pt() const noexcept { return pt_; }
    std::vector<Ciphertext>& ct() noexcept { return ct_; }
    const std::vector<Ciphertext>& ct() const noexcept { return ct_; }
    Time received_time() const noexcept { return received_time_; }
    Time deadline() const noexcept { return deadline_; }
    Time start_time() const noexcept { return start_time_; }
    Time finish_time() const noexcept { return finish_time_; }
    /*! @} */

    /*! @name Setter */
    /*! @{ */
    void received_time(Time v) noexcept { received_time_ = v; }
    void deadline(Time v) noexcept { deadline_ = v; }
    void start_time(Time v) noexcept { start_time_ = v; }
    void finish_time(Time v) noexcept { finish_time_ = v; }
    /*! @} */
  private:
    //! ジョブの種類ID
    AppID app_id_ = 0;
    
    //! クライアントID
    ClientID client_id_ = 0;

    //! クエリID
    QueryID query_id_ = 0;

    //! ジョブのID
    JobID job_id_ = 0;

    //! 入力平文
    std::vector<Plaintext> pt_;

    //! 入力暗号文
    std::vector<Ciphertext> ct_;

    //! クエリの受信時刻
    Time received_time_ = 0;

    //! ジョブのデッドライン
    Time deadline_ = 0;

    //! ジョブの実行開始時刻
    Time start_time_ = 0;

    //! ジョブの実行完了時刻
    Time finish_time_ = 0;
  };
  
  
}

inline std::ostream& operator<<(std::ostream &stream, std::pair<size_t, std::reference_wrapper<const DAMCREM::Query>> &&src){
  std::ostringstream oss;
  const std::string prefix_space(src.first, ' ');
  const auto &x = src.second.get();
  oss << prefix_space << "app_id: " << x.app_id() << std::endl;
  oss << prefix_space << "client_id: " << x.client_id() << std::endl;
  oss << prefix_space << "query_id: " << x.query_id() << std::endl;
  oss << prefix_space << "job_id: " << x.job_id() << std::endl;
  oss << prefix_space << "# of plaintexts: " << x.pt().size() << std::endl;
  oss << prefix_space << "# of ciphertexts: " << x.ct().size();
  stream << oss.str();
  return stream;
}


