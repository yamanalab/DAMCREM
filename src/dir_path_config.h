#pragma once

#include"util/alias.h"
#include"util/stream.h"

namespace DAMCREM{
  class DirPathConfig{
  public:
    DirPathConfig(const std::filesystem::path &parent_dir="./",
                  const std::filesystem::path &app_dir="app",
                  const std::filesystem::path &scheme_dir="scheme",
                  const std::filesystem::path &const_data_dir="const_data",
                  const std::filesystem::path &input_data_dir="input_data")
      : parent_dir_(std::filesystem::absolute(parent_dir)),
        app_dir_(std::filesystem::absolute(app_dir)),
        scheme_dir_(std::filesystem::absolute(scheme_dir)),
        const_data_dir_(std::filesystem::absolute(const_data_dir)),
        input_data_dir_(std::filesystem::absolute(input_data_dir)){
      update();
    }

    DirPathConfig(const DirPathConfig &src) = delete;
    DirPathConfig(DirPathConfig &&src) = default;
    
    std::filesystem::path app_dir_path(size_t app_id) const noexcept {
      return util::join_path(app_dir_path_, app_id);
    }
    
    std::filesystem::path scheme_dir_path(size_t app_id, size_t client_id) const noexcept {
      return util::join_path(scheme_dir_path_, app_id, client_id);
    }
    
    std::filesystem::path const_data_dir_path(uint64_t app_id, uint64_t client_id) const {
      return util::join_path(const_data_dir_path_, app_id, client_id);
    }

    std::filesystem::path input_data_dir_path(uint64_t app_id, uint64_t client_id) const {
      return util::join_path(input_data_dir_path_, app_id, client_id);
    }

    
  private:
    void update();
    
    std::filesystem::path parent_dir_;

    std::filesystem::path app_dir_;

    std::filesystem::path app_dir_path_;
    
    std::filesystem::path scheme_dir_;

    std::filesystem::path scheme_dir_path_;
    
    std::filesystem::path const_data_dir_;

    std::filesystem::path const_data_dir_path_;

    std::filesystem::path input_data_dir_;

    std::filesystem::path input_data_dir_path_;

  };
  
}
