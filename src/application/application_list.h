#pragma once

#include"application/application.h"

namespace DAMCREM{
  class ApplicationList{
  public:
    ApplicationList(const Option &option, const SchemeList &scheme_list){
      for( const auto &[app_id, client_id_list] : option.valid_app_client_id_list() ){
        applications_.emplace(std::piecewise_construct,
                              std::forward_as_tuple(app_id),
                              std::forward_as_tuple(app_id, option, scheme_list));
      }
    }
      
    /*! @name Getter */
    /*! @{ */
    const std::unordered_map<size_t, Application>& applications() const noexcept { return applications_; }
    Application& applications(size_t app_id){ return applications_.at(app_id); }
    const Application& applications(size_t app_id) const { return applications_.at(app_id); }
    /*! @} */
    
  private:
    //! アプリケーション
    std::unordered_map<size_t, Application> applications_;

  };
}

