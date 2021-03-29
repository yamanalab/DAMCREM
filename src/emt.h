#pragma once

#include"wmt.h"

/*!
 * @note Jobクラスなどのインスタンスへの参照を追加する．
 */

namespace DAMCREM{
  class EMT{
  public:
    /*!
     * @param[in] wmt マクロタスクの情報
     * @param[in] cr 割当計算機資源量
     * @param[in] ft 終了時刻
     */
    EMT(WMT &&wmt, ComputationalResource &cr)
      : wmt_(std::move(wmt)), computational_resource_(cr){}
    ~EMT(){};
    EMT(const EMT &src) noexcept = default;
    EMT(EMT &&src) noexcept = default;

    //! @note std::priority_queueの都合上，deleteにできない．
    EMT& operator=(const EMT &src) noexcept = default;
  
    bool operator<(const EMT &other) const {
      return this->wmt_ < other.wmt_;
    }

    bool operator>(const EMT &other) const {
      return this->wmt_ > other.wmt_;
    }

    /*! @name Getter */
    /*! @{ */
    const WMT& wmt() const noexcept { return wmt_; }
    const ComputationalResource& computational_resource() const noexcept { return computational_resource_; }
    /*! @} */

    const Query& query() const noexcept { return wmt_.query(); }
    const Job& job() const noexcept { return wmt_.job(); }
    JobID job_id() const noexcept { return wmt_.job_id(); }
    MacrotaskID macrotask_id() const noexcept { return wmt_.macrotask_id(); }
    
    //! 割当スレッド数を取得する関数
    size_t num_threads() const noexcept { return computational_resource_.num_threads(); }
    
  private:
    //! マクロタスクの情報
    WMT wmt_;
    
    //! 割り当てる計算資源量
    ComputationalResource computational_resource_;
    
  };

}

