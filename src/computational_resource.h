#pragma once

#include"util.h"

namespace DAMCREM{
  class ComputationalResource{
  public:
    ComputationalResource() noexcept {}
    ComputationalResource(size_t num_threads) noexcept : num_threads_(num_threads){}
    ComputationalResource(const std::vector<std::string> &values){
      if( values.size() != 1 ){
        throw std::runtime_error(util::join_str(values, ",").str()+" is invalid values for constructor of ComputationalResource.");
      }
      num_threads_ = std::stoul(values.at(0));
      return;
    }

    ComputationalResource(const ComputationalResource &src) noexcept = default;
    ComputationalResource(ComputationalResource &&src) noexcept = default;

    ComputationalResource& operator=(const ComputationalResource &src) noexcept = default;

    ComputationalResource& operator+=(const ComputationalResource &src) noexcept {
      this->num_threads_ += src.num_threads_;
      return *this;
    }

    ComputationalResource& operator-=(const ComputationalResource &src) noexcept {
      this->num_threads_ -= src.num_threads_;
      return *this;
    }

    ComputationalResource operator+(const ComputationalResource &src) const noexcept {
      ComputationalResource result = *this;
      result += src;
      return result;
    }
    
    ComputationalResource operator-(const ComputationalResource &src) const noexcept {
      ComputationalResource result = *this;
      result -= src;
      return result;
    }

    //! 引数の要素が有効な計算機資源量であるかの判定を行う
    /*!
     * @retval true 全ての要素において，targetの方が小さいか等しい
     * @retval false 1つ以上の要素において，targetの方が大きい
     */
    bool is_valid(const ComputationalResource &target) const noexcept {
      if( num_threads_ < target.num_threads_ ){ return false; }
      return true;
    }
    
    /*! @name Getter */
    /*! @{ */
    size_t num_threads() const noexcept { return num_threads_; }
    /*! *} */

    std::string to_string() const {
      std::ostringstream oss;
      oss << num_threads_;
      return oss.str();
    }
    
  private:
    //! 割当スレッド数
    size_t num_threads_ = 0;
    
  };

  
}

