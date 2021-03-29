#pragma once

#include<iomanip>
#include<mutex>
#include<set>
#include<unordered_set>

#include"server_info.h"
#include"file_path_config.h"
// #include"scheme_list.h"

namespace DAMCREM{
  class Option{
  public:
    static constexpr size_t digit_file_suffix = 10;

    Option(){}
    Option(const std::string &file_path, bool is_finalize=false);
    virtual ~Option() = default;
    Option(const Option &src) noexcept = delete;
    Option(Option &&src) noexcept = default;
    
    //! 設定が完了した際に実行する関数
    void finalize(){ is_finalized_ = true; }

    const DirPathConfig& dir_path_config() const noexcept { return file_path_config_.dir_path_config(); }

    ClientID get_new_client_id(const AppID app_id);
    
    /*! @name Getter */
    /*! @{ */
    bool is_finalized() const noexcept { return is_finalized_; }
    const FilePathConfig& file_path_config() const noexcept { return file_path_config_; }
    const ComputationalResource& max_computational_resource() const noexcept { return max_computational_resource_; }
    const std::unordered_map<size_t, double>& lowerbound_of_prirority() const noexcept { return lowerbound_of_priority_; }
    //! 設定済みであれば設定された値を返し，未指定であれば，デフォルト値を返す
    double get_lowerbound_of_priority(AppID i) const {
      if( lowerbound_of_priority_.count(i) ){ return lowerbound_of_priority_.at(i); }
      return def_lowerbound_of_priority_;
    }
    double def_lowerbound_of_prirority() const noexcept { return def_lowerbound_of_priority_; }
    bool do_use_max_threads_for_prediction() const noexcept { return do_use_max_threads_for_prediction_; }
    bool do_use_critical_path_for_prediction() const noexcept { return do_use_critical_path_for_prediction_; }
    std::unordered_set<size_t>& valid_app_client_id_list(AppID app_id) noexcept {
      return valid_app_client_id_list_.at(app_id);
    }
    const std::unordered_map<size_t, std::unordered_set<size_t>>& valid_app_client_id_list() const noexcept {
      return valid_app_client_id_list_;
    }
    size_t max_num_executing_jobs() const noexcept { return max_num_executing_jobs_; }
    const ServerInfo& server_info() const noexcept { return server_info_; }
    /*! @} */

    /*! @name Setter */
    /*! @{ */
    void max_computational_resource(const ComputationalResource &v) noexcept { set_param(max_computational_resource_, v); }
    void lowerbound_of_priority(AppID i, double v){ set_param(lowerbound_of_priority_, i, v); } 
    void def_lowerbound_of_priority(double v) noexcept { set_param(def_lowerbound_of_priority_, v); }
    void do_use_max_threads_for_prediction(bool v) noexcept { set_param(do_use_max_threads_for_prediction_, v); }
    void do_use_critical_path_for_prediction(bool v) noexcept { set_param(do_use_critical_path_for_prediction_, v); }
    void max_num_executing_jobs(size_t v) noexcept { set_param(max_num_executing_jobs_, v); }
    void server_info(ServerInfo &&v) noexcept { set_param(server_info_, v); }
    /*! @} */

    //! 計算機資源の最大値を設定する関数
    template<class... Args>
    void set_max_computational_resource(Args&&... args) noexcept {
      set_param(max_computational_resource_, ComputationalResource(std::forward<Args>(args)...));
    }
    
    //! 使用可能な最大スレッド数のみ設定する関数
    /*!
     * @param[in] v 使用可能な最大スレッド数
     * @note メモリ帯域幅などが追加された場合は，既存のmax_computational_resourceの対象以外の値を用いてcrを作成する．
     *       例えば，ComputationalResource cr(v, max_computational_resource.mem_width);といった感じ．
     *       コピーコストで無駄があるが，事前に実行する処理のため，影響は小さいと思う．
     */
    void max_num_threads(size_t v) noexcept {
      ComputationalResource cr(v);
      set_param(max_computational_resource_, cr);
    }
    
    std::string to_string() const;
    
  private:
    //! 変数を設定するための関数
    /*!
      @param[in] x 変更対象の変数
      @param[in] v 変更後の値
    */
    template<class T, class U>
    void set_param(T &x, U &&v) noexcept { if( !is_finalized_ ){ x = v; } }

    //! 変数を設定するための関数
    /*!
      @param[in] x 変更対象の変数
      @param[in] v 変更後の値
    */
    template<class T, class U>
    void set_param(std::unordered_map<size_t, T> &x, size_t i, U &&v) noexcept { if( !is_finalized_ ){ x.emplace(i, v); } }

    void set_param_from_str(const std::string &varname, const std::vector<std::string> &values);

    //! パラメータ設定が完了したらtrueにする．以降はメンバ変数(システムの基準時刻を除く)の値を変更不可．
    bool is_finalized_ = false;
    
    //! ディレクトリやファイルのパスを管理する
    FilePathConfig file_path_config_;
    
    //! 使用可能な最大計算機資源量
    ComputationalResource max_computational_resource_;
    
    //! スレッド数の候補を決める時の優先度の下限
    /*!
     * lowerbound_of_priority_[app_id] -> lowerbound of priority
     */
    std::unordered_map<AppID, double> lowerbound_of_priority_;
    
    //! スレッド数の候補を決める時の優先度のデフォルト(未指定時)の下限
    double def_lowerbound_of_priority_ = 0.0;
    
    //! 使用計算コストを推測する際に，最大スレッド数を用いるかどうかのフラグ
    bool do_use_max_threads_for_prediction_ = false;

    //! 使用計算コストを推測する際に，クリティカルパスを用いるかどうかのフラグ
    bool do_use_critical_path_for_prediction_ = false;
    
    //! 実行可能なアプリケーションIDとクライアントID
    std::unordered_map<AppID, std::unordered_set<ClientID>> valid_app_client_id_list_;
    
    //! 同時に実行可能なジョブ数 The maximum number of jobs which are in queue of Executor
    /*!
     * If this value is 0, any number of jobs can be added into queue of Executor.
     */
    size_t max_num_executing_jobs_ = 0;

    //! Info of server such as host and port number
    ServerInfo server_info_;

  };
  
}

