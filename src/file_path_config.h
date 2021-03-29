#pragma once

#include<cmath>

#include"dir_path_config.h"
#include"scheme.h"

namespace DAMCREM{
  class FilePathConfig{
  public:
    FilePathConfig(size_t digit_file_suffix=10) : digit_file_suffix_(digit_file_suffix){}
    FilePathConfig(DirPathConfig &&dpc, size_t digit_file_suffix=10) : dpc_(std::move(dpc)), digit_file_suffix_(digit_file_suffix){}
    FilePathConfig(const FilePathConfig &src) = delete;
    FilePathConfig(FilePathConfig &&src) = default;

    template<class T>
    const std::filesystem::path& data_file_name_prefix() const;
    
    template<class T>
    std::filesystem::path const_data_file_path_prefix(AppID app_id, ClientID client_id) const {
      return util::join_path(dpc_.const_data_dir_path(app_id, client_id), data_file_name_prefix<T>());
    }

    template<class T>
    std::filesystem::path input_data_file_path_prefix(AppID app_id, ClientID client_id) const {
      return util::join_path(dpc_.input_data_dir_path(app_id, client_id), data_file_name_prefix<T>());
    }
    
    std::filesystem::path app_data_file_path(AppID app_id) const {
      return util::join_path(dpc_.app_dir_path(app_id), app_data_file_name_);
    }

    std::filesystem::path app_exec_time_data_file_path(AppID app_id) const {
      return util::join_path(dpc_.app_dir_path(app_id), app_exec_time_data_file_name_);
    }

    template<class T>
    std::filesystem::path const_data_file_path(AppID app_id, ClientID client_id, size_t i) const {
      return const_data_file_path_prefix<T>(app_id, client_id) += file_suffix_id(i);
    }

    template<class T>
    std::filesystem::path input_data_file_path(AppID app_id, ClientID client_id, size_t i, bool do_force=true) const {
      const std::filesystem::path file_path = input_data_file_path_prefix<T>(app_id, client_id) += file_suffix_id(i);
      if( do_force || std::filesystem::exists(file_path) ){ return file_path; }
      return input_data_file_path_prefix<T>(app_id, 0) += file_suffix_id(i);
    }
    
    template<class T>
    std::vector<std::filesystem::path> input_data_file_path_list(AppID app_id, ClientID client_id,
                                                                 size_t n, bool do_force=true) const {
      std::vector<std::filesystem::path> result;
      util::transform(util::sequence<size_t>(0, n), std::back_inserter(result),
                      [&](size_t i){ return input_data_file_path<T>(app_id, client_id, i, do_force); });
      return result;
    }

    
    template<class T>
    std::vector<std::filesystem::path> const_data_file_path_list(AppID app_id, ClientID client_id, size_t n) const {
      std::vector<std::filesystem::path> result;
      util::transform(util::sequence<size_t>(0, n), std::back_inserter(result),
                      [&](size_t i){ return const_data_file_path<T>(app_id, client_id, i); });
      return result;
    }

    void add_query_arrival_time_file_info(AppID app_id, const std::filesystem::path &file_path, size_t n){
      query_arrival_time_file_info_.emplace(app_id, std::make_pair(file_path, n));
    }

    /*! @ Getter */
    /*! @{ */
    const DirPathConfig& dir_path_config() const noexcept { return dpc_; }
    const auto& query_arrival_time_file_info() const noexcept { return query_arrival_time_file_info_; }
    const std::pair<std::filesystem::path, size_t>& query_arrival_time_file_info(AppID app_id) const {
      return query_arrival_time_file_info_.at(app_id);
    }
    /*! @} */

    
  private:
    std::string file_suffix_id(uint64_t i) const {
      std::ostringstream oss;
      oss << std::setw(digit_file_suffix_) << std::setfill('0') << i;
      return oss.str();
    }

    
    DirPathConfig dpc_;

    size_t digit_file_suffix_;
    
    std::filesystem::path app_data_file_name_ = "app_data.txt";
    std::filesystem::path app_exec_time_data_file_name_ = "app_exec_time_data.txt";
    std::filesystem::path pt_file_name_prefix_ = Scheme::scheme_name()+"_plaintext_";
    std::filesystem::path ct_file_name_prefix_ = Scheme::scheme_name()+"_ciphertext_";

    //! クエリ情報のファイル．query_arrival_time_file_path[app_id] => (file_path, the number of queries from the file)
    std::unordered_map<uint64_t, std::pair<std::filesystem::path, size_t>> query_arrival_time_file_info_;
    
  };

  template<>
  inline const std::filesystem::path& FilePathConfig::data_file_name_prefix<Plaintext>() const {
    return pt_file_name_prefix_;
  }

  template<>
  inline const std::filesystem::path& FilePathConfig::data_file_name_prefix<Ciphertext>() const {
    return ct_file_name_prefix_;
  }

  
  
}
