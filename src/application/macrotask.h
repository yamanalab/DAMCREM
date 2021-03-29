#pragma once

#include<string>
#include<unordered_set>

#include"operation.h"

namespace DAMCREM{
  class Macrotask{
  public:
    Macrotask(size_t macrotask_id) : macrotask_id_(macrotask_id), macrotask_group_(macrotask_id_){}
    Macrotask(const Macrotask &src) noexcept = delete;
    Macrotask(Macrotask &&src) noexcept = default;

    size_t macrotask_group() const noexcept { return macrotask_group_; }
    const std::vector<size_t>& next() const noexcept { return next_; }
    const std::unordered_set<size_t>& previous() const noexcept { return previous_; }
    const std::vector<Operation>& op() const noexcept { return op_; }

    void set_macrotask_group(size_t i) noexcept { macrotask_group_ = i; }
    void add_next(size_t i){ next_.emplace_back(i); }
    void add_previous(size_t i){ previous_.emplace(i); }
    void add_operation(const std::vector<std::string> &src){ op_.emplace_back(src); }
    
  private:
    size_t macrotask_id_;
    
    size_t macrotask_group_;
    
    std::vector<size_t> next_;

    std::unordered_set<size_t> previous_;
    
    //! 演算内容
    /*!
     * @note 1つのマクロタスク内での準同型演算間の並列性はどうやって表現する？
     *       exec_orderみたいなメンバ変数をOperationクラスに追加して，小さい筋から実行，同じ数字は並列実行可能，とする？
     *       また，準同型演算間を並列に実行できるとすると，使用するスレッド数は配分方法をどのように指定する？
     */
    std::vector<Operation> op_;
    

  };
  
  
}

