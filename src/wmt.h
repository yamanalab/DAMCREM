#pragma once

#include"job.h"

namespace DAMCREM{
  class WMT{
  public:
    WMT(MacrotaskID macrotask_id, const Option& option, const Job &job, Time ready_time)
      : macrotask_id_(macrotask_id), option_(option), job_(job), ready_time_(ready_time){}
    ~WMT(){};
    WMT(const WMT &src) noexcept = default;
    WMT(WMT &&src) noexcept = default;

    //! @note std::priority_queueの都合上，deleteにできない．
    WMT& operator=(const WMT &src) noexcept = default;

    //! スケジューリング方法を決定するoperator
    /*!
      schedule_method/xxx.cpp で実装する．使用するソースファイルによって，スケジューリング方法が決まる．
     */
    bool operator<(const WMT &other) const;

    //! スケジューリング方法を決定するoperator
    /*!
     * schedule_method/xxx.cpp で実装する．使用するソースファイルによって，スケジューリング方法が決まる．
     */
    bool operator>(const WMT &other) const;

    
    //! ジョブのテンプレートの参照を返す関数
    const Application& app() const noexcept { return job().app(); }

    //! ジョブのデッドライン
    size_t deadline_job() const noexcept { return query().deadline(); }

    const MacrotaskExecTimeInfo& macrotask_exec_time_info() const noexcept {
      return app().macrotask_exec_time_info_manager().info(macrotask_id_);
    }

    //! ジョブのクエリの参照を返す関数
    const Query& query() const noexcept { return job().query(); }

    //! ジョブのIDを返す関数
    JobID job_id() const noexcept { return query().job_id(); }
    
    /*! @name Getter */
    /*! @{ */
    MacrotaskID macrotask_id() const noexcept { return macrotask_id_; }
    const Option& option() const noexcept { return option_.get(); }
    const Job& job() const noexcept { return job_.get(); }
    Time ready_time() const noexcept { return ready_time_; }
    /*! @} */
  
  private:
    //! マクロタスクのID
    MacrotaskID macrotask_id_;

    //! 使用しているオプションの情報
    std::reference_wrapper<const Option> option_;

    //! ジョブへの参照
    std::reference_wrapper<const Job> job_;

    //! Time when this macrotask is inputed into waiting queue
    Time ready_time_;
  };

}

inline std::ostream& operator<<(std::ostream &stream, std::pair<size_t, std::reference_wrapper<const DAMCREM::WMT>> &&src){
  const std::string prefix_space(src.first, ' ');
  const DAMCREM::WMT &x = src.second.get();
  stream << prefix_space << "query: " << std::endl;
  stream << std::make_pair<size_t, std::reference_wrapper<const DAMCREM::Query>>(src.first+2, x.query()) << std::endl;
  stream << prefix_space << "macrotask_id: " << x.macrotask_id() << std::endl;
  for( const DAMCREM::Operation &op : x.app().macrotask_graph().macrotask(x.macrotask_id()).op() ){
    stream << prefix_space << op << std::endl;
  }
  stream << x.ready_time();
  return stream;
}

inline std::ostream& operator<<(std::ostream &stream, const DAMCREM::WMT &src){
  stream << std::make_pair<size_t, std::reference_wrapper<const DAMCREM::WMT>>(0, src);
  return stream;
}

