#include"option.h"

#include"util/random.h"

namespace DAMCREM{
Option::Option(const std::string &file_path, bool is_finalize){
  std::ifstream ifs(file_path);
  std::string buf;
  while( std::getline(ifs, buf) ){
    if( buf.size() == 0 ){ continue; }
    if( buf[0] == '#' ){ continue; }
    std::vector<std::string> x = util::split_str(buf, "=");
    if( x.size() != 2 ){
      throw std::runtime_error(buf+" is invalid option.");
    }
    set_param_from_str(x.at(0), util::split_str(x.at(1), ","));
  }

  if( is_finalize ){
    finalize();
  }
    
  util::dbg_msg << "Loading option from " << file_path << " has been done." << std::endl;
  util::dbg_msg << to_string();
    
  return;
}

ClientID Option::get_new_client_id(const AppID app_id){
  static std::mutex mutex;
  static std::mt19937_64 engine = util::create_random_generator();
  std::lock_guard<std::mutex> lg(mutex);
  if( valid_app_client_id_list_.count(app_id) == 0 ){
    return 0;
  }
  
  ClientID client_id = 0;
  do{
    client_id = engine();
  }while( (valid_app_client_id_list_.at(app_id).count(client_id)) || (client_id == 0));
  valid_app_client_id_list_.at(app_id).emplace(client_id);
  return client_id;
}


std::string Option::to_string() const {
  std::ostringstream oss;
  oss << "is_finalized=" << is_finalized_ << std::endl;
  oss << "max_computational_resource=" << max_computational_resource_.to_string() << std::endl;
  for( auto [app_id, lop] : lowerbound_of_priority_ ){
    oss << "lowerbound_of_priority[" << app_id << "]=" << lop << std::endl;
  }
  oss << "def_lowerbound_of_priority=" << def_lowerbound_of_priority_ << std::endl;
  oss << "do_use_max_threads_for_prediction=" << do_use_max_threads_for_prediction_ << std::endl;
  for( const auto &[app_id, client_id_list] : valid_app_client_id_list_ ){
    std::ostringstream tmp;
    tmp << "valid_app_client_id_list[" << app_id << "]=";
    for( size_t client_id : client_id_list ){
      tmp << client_id << ",";
    }
    oss << tmp.str().substr(0, tmp.str().size()-1) << std::endl;
  }
  for( const auto &[app_id, file_info] : file_path_config_.query_arrival_time_file_info() ){
    oss << "query_arrival_time_file_info[" << app_id << "]=" << file_info.first << "," << file_info.second << std::endl;
  }
  return oss.str();
}

void Option::set_param_from_str(const std::string &varname, const std::vector<std::string> &values){
  if( is_finalized_ ){
    throw std::logic_error("The option has already been finalized.");
  }
    
  // 速度は重視していない．
  std::string varname_without_index, index;
  std::vector<std::string> tmp_varname = util::split_str(varname, "[");
  if( tmp_varname.size() == 2 ){
    varname_without_index = tmp_varname.at(0);
    index = util::split_str(tmp_varname.at(1), "]").at(0);
  }

  auto check_size_of_values = [&](size_t n, bool do_throw_error=true) -> bool{
    if( values.size() == n ){ return true; }
    if( do_throw_error ){ throw std::runtime_error(util::join_str(values, ",").str()+" is invalid values for "+varname); }
    return false;
  };

  auto check_valid_index = [&](bool do_throw_error=true) -> bool{
    if( index.size() > 0 ){ return true; }
    if( do_throw_error ){ throw std::runtime_error("The corrent number of index should be required for "+varname); };
    return false;
  };

  auto range_assign = [&](std::unordered_map<size_t, std::unordered_set<size_t>> &dst){
    check_valid_index();
    size_t i = std::stoul(index);
    dst.try_emplace(i);
    for( const auto &value : values ){
      const auto x = util::split_str(value, "--");
      if( x.size() == 1 ){
        dst.at(i).emplace(std::stoul(x.at(0)));
      }else if( x.size() == 2 ){
        for( size_t j = std::stoul(x.at(0)), e = std::stoul(x.at(1)); j < e; ++j ){
          dst.at(i).emplace(j);
        }
      }else{
        throw std::runtime_error("Invalid range value "+value);
      }
    }

    return;
  };
        
  if( varname == "max_computational_resource" ){
    max_computational_resource_ = ComputationalResource(values);
  }else if( varname_without_index == "lowerbound_of_priority" ){
    check_size_of_values(1);
    check_valid_index();
    lowerbound_of_priority_[std::stoul(index)] = std::stoul(values.at(0));
  }else if( varname == "def_lowerbound_of_priority" ){
    check_size_of_values(1);
    def_lowerbound_of_priority_ = util::convert_string<double>(values.at(0));
  }else if( varname == "do_use_max_threads_for_prediction" ){
    check_size_of_values(1);
    do_use_max_threads_for_prediction_ = util::convert_string<bool>(values.at(0));
  }else if( varname == "do_use_critical_path_for_prediction" ){
    check_size_of_values(1);
    do_use_critical_path_for_prediction_ = util::convert_string<bool>(values.at(0));
  }else if( varname_without_index == "valid_app_client_id_list" ){
    range_assign(valid_app_client_id_list_);
  }else if( varname_without_index == "query_arrival_time_file_info" ){
    check_size_of_values(2);
    check_valid_index();
    file_path_config_.add_query_arrival_time_file_info(
      util::convert_string<AppID>(index), values.at(0), util::convert_string<size_t>(values.at(1)));
  }else if( varname == "max_num_executing_jobs" ){
    check_size_of_values(1);
    max_num_executing_jobs_ = util::convert_string<size_t>(values.at(0));
  }else{
    throw std::invalid_argument(varname+" is invalid variable name.");
  }

    
  return;
}
  
  
}

