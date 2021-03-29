#include"dir_path_config.h"

namespace DAMCREM{
  void DirPathConfig::update(){
    app_dir_path_ = util::join_path(parent_dir_, app_dir_);
    scheme_dir_path_ = util::join_path(parent_dir_, scheme_dir_);
    const_data_dir_path_ = util::join_path(parent_dir_, const_data_dir_);
    input_data_dir_path_ = util::join_path(parent_dir_, input_data_dir_);
    return;
  }

  
}

