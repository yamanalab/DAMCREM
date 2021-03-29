#pragma once

#include"scheme_info.h"
#include"util.h"

namespace DAMCREM{
template<class T>
class OperandDataConst{
public:
  OperandDataConst(const std::unordered_map<std::string, std::string> &app_data);
  OperandDataConst(const OperandDataConst &src) noexcept = delete;
  OperandDataConst(OperandDataConst &&src) noexcept = default;

  size_t num_data() const noexcept { return num_data_; }
  size_t num_const_data() const noexcept { return num_const_data_; }
  size_t num_input_data() const noexcept { return num_input_data_; }
  size_t num_var_data() const noexcept { return num_var_data_; }
  const std::vector<size_t> result_indices() const noexcept { return result_indices_; }

  const std::vector<T>& const_data(ClientID client_id) const {
    if( const_data_.count(client_id) == 0 ){ client_id = 0; }
    return *const_data_.at(client_id);
  }
  const T& const_data(ClientID client_id, size_t i) const { return const_data(client_id).at(i); }

  void set_const_data(ClientID client_id, std::shared_ptr<std::vector<T>> src){ const_data_[client_id] = std::move(src); }
  void remove_const_data(ClientID client_id){ const_data_.erase(client_id); }
    
private:
  static const std::string KEY_NUM_CONST_DATA;
  static const std::string KEY_NUM_INPUT_DATA;
  static const std::string KEY_NUM_VAR_DATA;
  static const std::string KEY_RESULT_DATA_INDICES;
    
  //! データ総数
  size_t num_data_ = 0;
    
  //! 定数データの数
  size_t num_const_data_ = 0;

  //! 入力データの数
  size_t num_input_data_ = 0;

  //! 変数データの数
  size_t num_var_data_ = 0;
    
  //! 定数データ
  std::unordered_map<ClientID, std::shared_ptr<std::vector<T>>> const_data_;

  //! アプリケーションの結果として返すデータのインデックス
  std::vector<size_t> result_indices_;
    
};

template<class T>
OperandDataConst<T>::OperandDataConst(const std::unordered_map<std::string, std::string> &app_data){
  num_const_data_ = std::stoul(app_data.at(KEY_NUM_CONST_DATA));
  num_input_data_ = std::stoul(app_data.at(KEY_NUM_INPUT_DATA));
  num_var_data_ = std::stoul(app_data.at(KEY_NUM_VAR_DATA));
  num_data_ = num_const_data_ + num_input_data_ + num_var_data_;

  if( app_data.find(KEY_RESULT_DATA_INDICES) != app_data.end() ){
    util::transform(std::move(util::split_str<size_t>(app_data.at("result_ct_indices="), ",", true, true)),
                    std::back_inserter(result_indices_), [](size_t i){ return i; });
  }
  return;
}

template<>
inline const std::string OperandDataConst<Plaintext>::KEY_NUM_CONST_DATA = "num_const_pt=";
template<>
inline const std::string OperandDataConst<Plaintext>::KEY_NUM_INPUT_DATA = "num_input_pt=";
template<>
inline const std::string OperandDataConst<Plaintext>::KEY_NUM_VAR_DATA = "num_var_pt=";
template<>
inline const std::string OperandDataConst<Plaintext>::KEY_RESULT_DATA_INDICES = "result_pt_indices=";

template<>
inline const std::string OperandDataConst<Ciphertext>::KEY_NUM_CONST_DATA = "num_const_ct=";
template<>
inline const std::string OperandDataConst<Ciphertext>::KEY_NUM_INPUT_DATA = "num_input_ct=";
template<>
inline const std::string OperandDataConst<Ciphertext>::KEY_NUM_VAR_DATA = "num_var_ct=";
template<>
inline const std::string OperandDataConst<Ciphertext>::KEY_RESULT_DATA_INDICES = "result_ct_indices=";

}

