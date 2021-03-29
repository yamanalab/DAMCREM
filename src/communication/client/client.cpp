#include"communication/client/client.h"

#include"util/stream.h"

namespace DAMCREM{
namespace communication{

QueryID Client::get_new_query_id(){
  static std::mutex mutex;
  std::lock_guard<std::mutex> lg{mutex};
  QueryID query_id = 0;
  do{
    query_id = engine();
  }while( generated_query_id_list_.count(query_id) );
  generated_query_id_list_.emplace(query_id);
  return query_id;
}

bool Client::connect(){
  if( is_connected_ ){ return false; }

  client_.connect(host_.c_str(), port_.c_str());

  is_connected_ = true;
  std::clog << "Connected to the server" << std::endl;
  
  return true;
}

bool Client::disconnect(){
  if( !is_connected_ ){ return false; }
  if( using_query_id_list_.size() > 0 ){ return false; }

  client_.close();
  is_connected_ = false;
  std::clog << "Disconnected from the server" << std::endl;
  
  return true;
}

bool Client::send_data(std::ostringstream &stream, const ControlCode cc){
  if( !stream ){ return false; }
  stdsc::BufferStream bs(stream.tellp());
  std::memcpy(bs.data(), stream.str().data(), stream.tellp());
  client_.send_data_blocking(static_cast<uint64_t>(cc), *(static_cast<stdsc::Buffer*>(&bs)));
  return true;
}

bool Client::update_scheme(const AppID app_id, const ClientID client_id, const SchemeInfo &scheme_info){
  util::dbg_msg << "update_scheme()" << std::endl;
  std::ostringstream oss;
  UpdateSchemeC2S::write_to_stream(oss, &app_id, &client_id, &scheme_info);
  send_data(oss, ControlCode::kDataUpdateScheme);
  return true;
}

bool Client::delete_scheme(const AppID app_id, const ClientID client_id){
  util::dbg_msg << "delete_scheme()" << std::endl;
  std::ostringstream oss;
  DeleteDataC2S::write_to_stream(oss, app_id, client_id);
  send_data(oss, ControlCode::kDataDeleteScheme);
  return true;
}

bool Client::update_const_data(const AppID app_id, const ClientID client_id,
                               const std::vector<Plaintext> &pt, const std::vector<Ciphertext> &ct,
                               const SchemeList &scheme_list){
  util::dbg_msg << "update_const_data()" << std::endl;
  std::ostringstream oss;
  UpdateConstDataC2S::write_to_stream(oss, app_id, client_id, pt, ct, scheme_list);
  send_data(oss, ControlCode::kDataUpdateConstData);
  return true;
}

bool Client::delete_const_data(AppID app_id, ClientID client_id){
  util::dbg_msg << "update_const_data()" << std::endl;
  std::ostringstream oss;
  DeleteDataC2S::write_to_stream(oss, app_id, client_id);
  send_data(oss, ControlCode::kDataDeleteConstData);
  return true;
}

bool Client::request_client_id(ClientID &client_id, AppID app_id){
  util::dbg_msg << "request_client_id()" << std::endl;
  std::ostringstream oss;
  NewClientIDC2S::write_to_stream(oss, &app_id);
  AppID dummy_ai = 0;
  NewClientIDS2C result(&dummy_ai, &client_id);
  send_recv_data(oss, ControlCode::kUpDownloadNewClientID, result);
  return true;
}

bool Client::send_query(const QueryC2S &query){
  util::dbg_msg << "send_query()" << std::endl;
  std::ostringstream oss;
  query.write_to_stream(oss);
  using_query_id_list_.emplace(query.query_id);
  send_data(oss, ControlCode::kUpDownloadQuery);
  return true;
}

bool Client::request_result(std::vector<Plaintext> &pt, std::vector<Ciphertext> &ct,
                            const AppID app_id, const ClientID client_id, const QueryID query_id, const Scheme &scheme){
  std::ostringstream oss;
  ResultC2S::write_to_stream(oss, &app_id, &client_id, &query_id);
  AppID dummy_ai;
  ClientID dummy_ci;
  QueryID dummy_qi;
  ResultS2C result(&dummy_ai, &dummy_ci, &dummy_qi, &pt, &ct, scheme);
  send_recv_data(oss, ControlCode::kUpDownloadResult, result);
  using_query_id_list_.erase(query_id);
  if( (dummy_ai != app_id) || (dummy_ci != client_id) || (dummy_qi != query_id) ){
    return false;
  }
  return true;
}

                    
}
}


