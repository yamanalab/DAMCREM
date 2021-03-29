#pragma once

#include"application/macrotask_exec_time_info_manager.h"
#include"application/operand_data_const.h"
#include"scheme_list.h"

namespace DAMCREM{
class Application{
public:
  //! コンストラクタ
  /*!
   * @param[in] app_id application ID
   * @param[in] option option for DAMCREM
   */
  Application(AppID app_id, const Option &option, const SchemeList &scheme_list)
    : app_id_(app_id), option_(option),
      app_data_(util::parse_data_from_file(util::load_text_from_file(option_.file_path_config().app_data_file_path(app_id_)))),
      scheme_(scheme_list.scheme(app_id_)),
      macrotask_graph_(std::stoul(app_data_.at("num_macrotasks=")), app_data_),
      macrotask_exec_time_info_manager_(option_, app_id_, macrotask_graph_),
      pt_(app_data_), ct_(app_data_){
    set_const_data<Plaintext>(pt_);
    set_const_data<Ciphertext>(ct_);
  }
    
  virtual ~Application() = default;
  Application(const Application &src) = delete;
  Application(Application &&src) = default;

  void updateConstData(const ClientID client_id, std::shared_ptr<std::vector<Plaintext>> pt){
    pt_.set_const_data(client_id, pt);
    return;
  }

  void updateConstData(const ClientID client_id, std::shared_ptr<std::vector<Ciphertext>> ct){
    ct_.set_const_data(client_id, ct);
    return;
  }

  void deleteConstData(const ClientID client_id){
    pt_.remove_const_data(client_id);
    ct_.remove_const_data(client_id);
  }
  
  /*! @name Getter */
  /*! @{ */
  uint64_t app_id() const noexcept { return app_id_; }
  const auto& option() const noexcept { return option_; }
  const auto& scheme(uint64_t client_id) const { return *(scheme_.at(client_id)); }
  const auto& macrotask_graph() const noexcept { return macrotask_graph_; }
  const auto& macrotask_exec_time_info_manager() const noexcept { return macrotask_exec_time_info_manager_; }
  const auto& pt() const noexcept { return pt_; }
  const auto& ct() const noexcept { return ct_; }
  /*! @} */
    
private:
  //! Set constant data for each client ID
  /*!
   * @param[in] odc Target to be set
   */
  template<class T>
  void set_const_data(OperandDataConst<T> &odc);
    
  //! アプリケーションID．
  AppID app_id_;

  //! オプション情報
  const Option &option_;
    
  //! ファイルから読み込んだアプリケーションデータ
  const std::unordered_map<std::string, std::string> app_data_;
    
  //! 平文や暗号文を読み込むための鍵情報など．
  const std::unordered_map<ClientID, std::shared_ptr<Scheme>> &scheme_;

  //! Info of macrotask graph
  MacrotaskGraph macrotask_graph_;

  //! Info of execution time of whole and each of macrotasks
  MacrotaskExecTimeInfoManager macrotask_exec_time_info_manager_;

  //! Plaintexts info and constant plaintexts
  OperandDataConst<Plaintext> pt_;

  //! Ciphertexts info and constant ciphertexts
  OperandDataConst<Ciphertext> ct_;
    
};

template<class T>
void Application::set_const_data(OperandDataConst<T> &odc){
  for( size_t client_id : option_.valid_app_client_id_list().at(app_id_) ){
    if( !std::filesystem::exists(option_.dir_path_config().const_data_dir_path(app_id_, client_id)) ){ continue; }
    std::shared_ptr<std::vector<T>> const_data = std::make_shared<std::vector<T>>();
    util::transform(option_.file_path_config().const_data_file_path_list<T>(app_id_, client_id, odc.num_const_data()),
                    std::back_inserter(*const_data),
                    [&](const std::string &path){ return scheme(client_id).load_data<T>(path); });
    odc.set_const_data(client_id, const_data);
  }
  return;
}
  

  
}

