#pragma once

#include"application/operand_data_const.h"

namespace DAMCREM{
  template<class T>
  class OperandData{
  public:
    OperandData(const OperandDataConst<T> &const_data, std::vector<T> &input_data, ClientID client_id);
    OperandData(const OperandData<T> &src) noexcept = delete;
    OperandData(OperandData<T> &&src) noexcept = default;

    //! Return non-writable data as src operand.
    /*!
     * @param[in] i index. 0 <= i < const_data_.num_data()
     */
    const T& src(size_t i) const {
      if( i < const_data_.get().num_const_data() ){ return const_data_.get().const_data(client_id_, i); }
      return input_var_data_.at(i-const_data_.get().num_const_data());
    }

    //! Return writable data as dst operand. It can be used as src operand.
    /*! 
     * @param[in] i index. const_data_.num_const_data() <= i < const_data_.num_data()
     */
    T& dst(size_t i){
      return input_var_data_.at(i-const_data_.get().num_const_data());
    }

    const std::vector<std::reference_wrapper<T>>& result_data() const noexcept { return result_data_; }
    
  private:
    ClientID client_id_;
        
    std::reference_wrapper<const OperandDataConst<T>> const_data_;

    std::vector<T> var_data_;
    
    std::vector<std::reference_wrapper<T>> input_var_data_;

    std::vector<std::reference_wrapper<T>> result_data_;
    
  };

  template<class T>
  OperandData<T>::OperandData(const OperandDataConst<T> &const_data, std::vector<T> &input_data, ClientID client_id)
    : client_id_(client_id), const_data_(const_data), var_data_(const_data_.get().num_var_data()){
    var_data_.resize(const_data_.get().num_var_data());
    input_var_data_.reserve(const_data_.get().num_input_data() + const_data_.get().num_var_data());
    util::transform(input_data, std::back_inserter(input_var_data_), [](T &t){ return std::ref(t); });
    util::transform(var_data_, std::back_inserter(input_var_data_), [](T &t){ return std::ref(t); });
    util::transform(const_data_.get().result_indices(), std::back_inserter(result_data_),
                    [&](size_t i){ return std::ref(dst(i)); });
    return;
  }
  
}

