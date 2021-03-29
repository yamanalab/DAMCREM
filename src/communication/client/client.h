#pragma once

#include<cstring>
#include<sstream>
#include<string>
#include<unordered_set>
#include<vector>

#include"communication/shared_info.h"
#include"communication/data.h"
#include"query.h"
#include"scheme.h"
#include"util/alias.h"
#include"util/random.h"

#include"stdsc/stdsc_buffer.hpp"
#include"stdsc/stdsc_client.hpp"
#include"communication/stdsc_all.h"

namespace DAMCREM{
namespace communication{

class Client{
public:
  Client(const std::string &host="localhost", const std::string &port="45678") noexcept : host_(host), port_(port){}
  ~Client() noexcept = default;
  Client(const Client &src) noexcept = default;
  Client(Client &&src) noexcept = default;

  QueryID get_new_query_id();
  
  //! Connect to server
  bool connect();

  //! Disconnect from server
  bool disconnect();

  bool send_data(std::ostringstream &stream, const ControlCode cc);

  template<class T>
  bool send_recv_data(std::ostringstream &stream, const ControlCode cc, T &data_s2c);

  
  bool update_scheme(const AppID app_id, const ClientID client_id, const SchemeInfo &scheme_info);

  bool delete_scheme(const AppID app_id, const ClientID client_id);

  bool update_const_data(const AppID app_id, const ClientID client_id,
                         const std::vector<Plaintext> &pt, const std::vector<Ciphertext> &ct,
                         const SchemeList &scheme_list);

  bool delete_const_data(const AppID app_id, const ClientID client_id);

  bool request_client_id(ClientID &client_id, const AppID app_id);

  //! 
  /*!
   * @param[in] query query which will be sent to the server
   */
  bool send_query(const QueryC2S &query);
  
  //! 
  /*!
   * @param[in] pt Output plaintexts
   * @param[in] ct Output ciphertexts
   */
  bool request_result(std::vector<Plaintext> &pt, std::vector<Ciphertext> &ct,
                      const AppID app_id, const ClientID client_id, const QueryID query_id, const Scheme &scheme);

  // bool change_server_info(const std::string &host, const std::string &port);
  
private:
  std::string host_;
  std::string port_;

  bool is_connected_ = false;

  std::unique_ptr<std::mutex> mutex = std::make_unique<std::mutex>();
  std::unordered_set<QueryID> generated_query_id_list_;
  std::unordered_set<QueryID> using_query_id_list_;

  stdsc::Client client_;

  std::mt19937_64 engine = util::create_random_generator();
};

template<class T>
bool Client::send_recv_data(std::ostringstream &stream, const ControlCode cc, T &data_s2c){
  if( !stream ){ return false; }
  stdsc::BufferStream bs(stream.tellp());
  stdsc::Buffer rbuffer;
  std::memcpy(bs.data(), stream.str().data(), stream.tellp());
  client_.send_recv_data_blocking(static_cast<uint64_t>(cc), *(static_cast<stdsc::Buffer*>(&bs)), rbuffer);

  stdsc::BufferStream rbuffstream(rbuffer);
  std::iostream rstream(&rbuffstream);
  data_s2c.read_from_stream(rstream);

  return true;
}



}
}

