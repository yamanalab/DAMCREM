#pragma once

#include"scheme.h"
#include"util/alias.h"

namespace DAMCREM{
namespace communication{

struct UpdateSchemeC2S{
  static bool read_from_stream(std::istream &stream, AppID *ai, ClientID *ci, SchemeInfo *si);
  static bool write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci, const SchemeInfo *si);
  
  UpdateSchemeC2S() : need_to_delete(true), app_id(new AppID{}), client_id(new ClientID{}),
                              scheme_info(new SchemeInfo{}){}
  UpdateSchemeC2S(AppID *ai, ClientID *ci, SchemeInfo *si) : app_id(ai), client_id(ci), scheme_info(si){}
  virtual ~UpdateSchemeC2S(){
    if( !need_to_delete ){ return; }
    delete app_id;
    delete client_id;
    delete scheme_info;
  }
  UpdateSchemeC2S(const UpdateSchemeC2S &src) noexcept = delete;
  UpdateSchemeC2S(UpdateSchemeC2S &&src) noexcept = delete;
    
  const bool need_to_delete = false;
  AppID *app_id = nullptr;
  ClientID *client_id = nullptr;
  SchemeInfo *scheme_info = nullptr;
  
  bool read_from_stream(std::istream &stream){ return read_from_stream(stream, app_id, client_id, scheme_info); }

  bool write_to_stream(std::ostream &stream) const { return write_to_stream(stream, app_id, client_id, scheme_info); }
};

inline bool UpdateSchemeC2S::read_from_stream(std::istream &stream,
                                                      AppID *ai, ClientID *ci, SchemeInfo *si){
  if( !stream ){ return false; }
  util::read(stream, ai);
  util::read(stream, ci);
  si->read_from_stream(stream);
  return true;
}

inline bool UpdateSchemeC2S::write_to_stream(std::ostream &stream,
                                                     const AppID *ai, const ClientID *ci, const SchemeInfo *si){
  if( !stream ){ return false; }
  util::write(stream, ai);
  util::write(stream, ci);
  si->write_to_stream(stream);
  return true;
}

}  // namespace communication
}  // namespace DAMCREM

inline std::ostream& operator<<(std::ostream &stream,
                         std::pair<size_t, std::reference_wrapper<const DAMCREM::communication::UpdateSchemeC2S>> &&src){
  std::ostringstream oss;
  const std::string prefix_space(src.first, ' ');
  const auto &x = src.second.get();
  oss << prefix_space << "app_id: " << *(x.app_id) << std::endl;
  oss << prefix_space << "client_id: " << *(x.client_id) << std::endl;
  stream << oss.str();
  return stream;
}

