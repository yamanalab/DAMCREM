#include"communication/server/callback_function.h"

#include"communication/data.h"
#include"communication/server/server_callback_param.h"
#include"communication/shared_info.h"


namespace DAMCREM{
namespace communication{

DEFUN_DATA(CallbackFunctionForUpdateScheme){
  std::cerr << "update scheme" << std::endl;
  STDSC_LOG_INFO("Received scheme");
  DEF_CDATA_ON_ALL(ServerSharedCallbackParam);
  stdsc::BufferStream rbuffstream(buffer);
  std::iostream rstream(&rbuffstream);

  UpdateSchemeC2S data{};
  data.read_from_stream(rstream);
  if( *(data.client_id) == 0 ){ return; }
  Scheme scheme(std::move(*(data.scheme_info)));
  cdata_a->scheme_list().add_scheme(*(data.app_id), *(data.client_id), std::move(scheme));
  
  return;
}

DEFUN_DATA(CallbackFunctionForDeleteScheme){
  std::clog << "delete scheme " << std::endl;
  STDSC_LOG_INFO("Received scheme info for deletion");
  DEF_CDATA_ON_ALL(ServerSharedCallbackParam);
  stdsc::BufferStream rbuffstream(buffer);
  std::iostream rstream(&rbuffstream);

  DeleteDataC2S data{};
  data.read_from_stream(rstream);
  std::cerr << util::makeIndentedMessageData(data, 2) << std::endl;
  
  return;
}

DEFUN_DATA(CallbackFunctionForUpdateConstData){
  std::clog << "update const data " << std::endl;
  STDSC_LOG_INFO("Received const data");
  DEF_CDATA_ON_ALL(ServerSharedCallbackParam);
  stdsc::BufferStream rbuffstream(buffer);
  std::iostream rstream(&rbuffstream);

  UpdateConstDataC2S data{cdata_a->scheme_list()};
  data.read_from_stream(rstream);
  std::cerr << util::makeIndentedMessageData(data, 2) << std::endl;

  if( data.client_id == 0 ){ return; }
  cdata_a->app_list().applications(data.app_id).updateConstData(data.client_id,
                                                                std::make_shared<std::vector<Plaintext>>(std::move(data.pt)));
  cdata_a->app_list().applications(data.app_id).updateConstData(data.client_id,
                                                                std::make_shared<std::vector<Ciphertext>>(std::move(data.ct)));
  std::clog << "done." << std::endl;
  return;
}

DEFUN_DATA(CallbackFunctionForDeleteConstData){
  std::clog << "delete const data " << std::endl;
  STDSC_LOG_INFO("Received info of deleting const data");
  DEF_CDATA_ON_ALL(ServerSharedCallbackParam);
  stdsc::BufferStream rbuffstream(buffer);
  std::iostream rstream(&rbuffstream);

  DeleteDataC2S data;
  data.read_from_stream(rstream);
  std::cerr << util::makeIndentedMessageData(data, 2) << std::endl;
  cdata_a->app_list().applications(data.app_id).deleteConstData(data.client_id);

  return;
}


DEFUN_UPDOWNLOAD(CallbackFunctionForNewClientID){
  std::clog << "new client ID requested" << std::endl;
  STDSC_LOG_INFO("Received new client ID request");
  DEF_CDATA_ON_ALL(ServerSharedCallbackParam);
  stdsc::BufferStream rbuffstream(buffer);
  std::iostream rstream(&rbuffstream);

  NewClientIDC2S nci{};
  nci.read_from_stream(rstream);

  const ClientID client_id = cdata_a->option().get_new_client_id(*(nci.app_id));
  std::cerr << *nci.app_id << ", " << client_id << std::endl;
  
  std::ostringstream oss;
  NewClientIDS2C::write_to_stream(oss, nci.app_id, &client_id);
  stdsc::BufferStream sbuffstream(oss.tellp());
  std::iostream sstream(&sbuffstream);
  NewClientIDS2C::write_to_stream(sstream, nci.app_id, &client_id);
  stdsc::Buffer *bsbuff = &sbuffstream;
  sock.send_packet(stdsc::make_data_packet(static_cast<uint64_t>(ControlCode::kDataNewClientID), oss.tellp()));
  sock.send_buffer(*bsbuff);
  return;
}

DEFUN_UPDOWNLOAD(CallbackFunctionForQuery){
  std::clog << "Query received" << std::endl;
  STDSC_LOG_INFO("Received query");
  DEF_CDATA_ON_ALL(ServerSharedCallbackParam);
  stdsc::BufferStream rbuffstream(buffer);
  std::iostream rstream(&rbuffstream);

  QueryC2S data{cdata_a->scheme_list()};
  data.read_from_stream(rstream);
  std::cerr << util::makeIndentedMessageData(data, 2) << std::endl;
  
  cdata_a->query_receiver().add_query(data.app_id, data.client_id, data.query_id,
                                      std::move(data.pt), std::move(data.ct), cdata_a->time_manager().get_current_time());
  
  return;
}

DEFUN_UPDOWNLOAD(CallbackFunctionForResult){
  std::clog << "Result requested" << std::endl;
  STDSC_LOG_INFO("Received result request");
  DEF_CDATA_ON_ALL(ServerSharedCallbackParam);
  stdsc::BufferStream rbuffstream(buffer);
  std::iostream rstream(&rbuffstream);

  ResultC2S data{};
  data.read_from_stream(rstream);
  std::cerr << util::makeIndentedMessageData(data, 2) << std::endl;

  std::shared_ptr<Job> job = cdata_a->result_sender().get_result(*(data.app_id), *(data.client_id), *(data.query_id));
  AppID app_id = 0;
  ClientID client_id = 0;
  QueryID query_id = 0;
  std::vector<std::reference_wrapper<Plaintext>> dummy_pt;
  std::vector<std::reference_wrapper<Ciphertext>> dummy_ct;
  std::reference_wrapper<const std::vector<std::reference_wrapper<Plaintext>>> pt = dummy_pt;
  std::reference_wrapper<const std::vector<std::reference_wrapper<Ciphertext>>> ct = dummy_ct;
  const Scheme &scheme = cdata_a->scheme_list().scheme(*(data.app_id), *(data.client_id));
  if( job != nullptr ){
    app_id = job->query().app_id();
    client_id = job->query().client_id();
    query_id = job->query().query_id();
    ct = job->get_results();
  }

  std::ostringstream oss;
  ResultS2C::write_to_stream(oss, app_id, client_id, query_id, pt.get(), ct.get(), scheme);
  stdsc::BufferStream sbuffstream(oss.tellp());
  std::iostream sstream(&sbuffstream);
  ResultS2C::write_to_stream(sstream, app_id, client_id, query_id, pt.get(), ct.get(), scheme);
  stdsc::Buffer *bsbuff = &sbuffstream;
  sock.send_packet(stdsc::make_data_packet(static_cast<uint64_t>(ControlCode::kDataResult), oss.tellp()));
  sock.send_buffer(*bsbuff);
  
  return;
}



}  // namespace communication
}  // namespace DAMCREM
