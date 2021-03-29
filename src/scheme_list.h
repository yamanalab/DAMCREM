#pragma once

#include<mutex>
#include<unordered_map>
#include<unordered_set>

#include"dir_path_config.h"
#include"option.h"
#include"scheme.h"

namespace DAMCREM{
  class SchemeList{
  public:
    SchemeList() : mutex_(std::make_unique<std::mutex>()){}
    SchemeList(const Option &option) : mutex_(std::make_unique<std::mutex>()){ load_schemes(option); }
    virtual ~SchemeList() = default;
    SchemeList(const SchemeList &src) noexcept = delete;
    SchemeList(SchemeList &&src) noexcept = default;
    
    //! 既存のスキームを使用するように設定する関数
    void add_scheme(AppID app_id, ClientID client_id, ClientID target_client_id){
      std::lock_guard<std::mutex> lg(*mutex_);
      scheme_.at(app_id)[client_id] = scheme_.at(app_id).at(target_client_id);
    }

    //! 既存のスキームを使用するように設定する関数
    void add_scheme(AppID app_id, ClientID client_id, AppID target_app_id, ClientID target_client_id){
      std::lock_guard<std::mutex> lg(*mutex_);
      scheme_.at(app_id)[client_id] = scheme_.at(target_app_id).at(target_client_id);
    }
    
    //! スキームを追加する関数
    void add_scheme(AppID app_id, ClientID client_id, Scheme &&scheme){
      std::lock_guard<std::mutex> lg(*mutex_);
      scheme_[app_id][client_id] = std::make_shared<Scheme>(std::move(scheme));
    }

    void delete_scheme(const AppID app_id, const ClientID client_id){
      std::lock_guard<std::mutex> lg(*mutex_);
      if( do_exist(app_id, client_id) ){ scheme_.at(app_id).erase(client_id); }
    }
    
    bool load_scheme(const DirPathConfig &dpc, AppID app_id, ClientID clientID, bool do_force);

    void load_schemes(const Option &option);
    
    bool do_exist(const AppID app_id) const {
      return scheme_.find(app_id) != scheme_.end();
    }

    bool do_exist(const AppID app_id, const ClientID client_id) const {
      auto itr = scheme_.find(app_id);
      return ((itr!=scheme_.end()) && (itr->second.find(client_id)!=itr->second.end()));
    }
    
    /*! @name Getter */
    /*! @{ */
    const auto& scheme() const { return scheme_; }
    /*! @} */

    const auto& scheme(AppID app_id) const { return scheme_.at(app_id); }
    
    const Scheme& scheme(AppID app_id, ClientID client_id) const { return *(scheme(app_id).at(client_id)); }

  private:
    std::unique_ptr<std::mutex> mutex_;

    //! スキーム
    std::unordered_map<AppID, std::unordered_map<ClientID, std::shared_ptr<Scheme>>> scheme_;
    
  };
  
}

