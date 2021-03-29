#include"scheme_list.h"

namespace DAMCREM{
bool SchemeList::load_scheme(const DirPathConfig &dpc, AppID app_id, ClientID client_id, bool do_force){
  if( !do_force && do_exist(app_id, client_id) ){
    return true;
  }
    
  const std::filesystem::path dir_path = dpc.scheme_dir_path(app_id, client_id);
  const std::filesystem::path symlink_file_path = dir_path / Scheme::scheme_name() += ".link";
  if( !std::filesystem::exists(dir_path) ){
    if( client_id == 0 ){ return false; }
    if( load_scheme(dpc, app_id, 0, false) ){
      throw std::runtime_error("scheme data 0th is required but it does not exist.");
    }
    add_scheme(app_id, client_id, 0);
  }else if( std::filesystem::is_symlink(symlink_file_path) ){
    std::filesystem::path target_file_path = std::filesystem::absolute(std::filesystem::read_symlink(symlink_file_path));
    std::filesystem::path tmp = std::filesystem::relative(target_file_path, dir_path);
    const std::vector<std::string> ss = util::split_str(tmp.string(), "/");
    AppID target_app_id = util::convert_string<AppID>(ss.at(1));
    ClientID target_client_id = util::convert_string<ClientID>(ss.at(2));
    if( dpc.scheme_dir_path(target_app_id, target_client_id) != target_file_path){
      throw std::runtime_error("Invalid symlink file: "+symlink_file_path.string()+" -> "+target_file_path.string());
    }
    load_scheme(dpc, target_app_id, target_client_id, false);
    add_scheme(app_id, client_id, target_app_id, target_client_id);
  }else{
    add_scheme(app_id, client_id, dir_path / Scheme::scheme_name());
  }

  return true;
}

void SchemeList::load_schemes(const Option &option){
  for( const auto &[app_id, client_id_list] : option.valid_app_client_id_list() ){
    util::for_each(client_id_list, [&](ClientID client_id){ load_scheme(option.dir_path_config(), app_id, client_id, false); });
  }
  return;
}

}  // namespace DAMCREM

