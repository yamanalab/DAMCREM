#pragma once

#include"scheme.h"
#include"util/alias.h"

namespace DAMCREM{
namespace communication{

struct NewClientIDS2C{
  static bool read_from_stream(std::istream &stream, AppID *ai, ClientID *ci);
  static bool write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci);

  NewClientIDS2C() : need_to_delete(true), app_id(new AppID{}), client_id(new ClientID{}){}
  NewClientIDS2C(AppID *ai, ClientID *ci) noexcept : app_id(ai), client_id(ci){}
  virtual ~NewClientIDS2C(){
    if( !need_to_delete ){ return; }
    delete app_id;
    delete client_id;
  }
  NewClientIDS2C(const NewClientIDS2C &src) noexcept = delete;
  NewClientIDS2C(NewClientIDS2C &&src) noexcept = delete;

  const bool need_to_delete = false;
  AppID *app_id = nullptr;
  ClientID *client_id = nullptr;

  bool read_from_stream(std::istream &stream){ return read_from_stream(stream, app_id, client_id); }

  bool write_to_stream(std::ostream &stream){ return write_to_stream(stream, app_id, client_id); }
};


inline bool NewClientIDS2C::read_from_stream(std::istream &stream, AppID *ai, ClientID *ci){
  if( !stream ){ return false; }
  util::read(stream, ai);
  util::read(stream, ci);
  return true;
}

inline bool NewClientIDS2C::write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci){
  if( !stream ){ return false; }
  util::write(stream, ai);
  util::write(stream, ci);
  return true;
}

}  // namespace communication
}  // namespace DAMCREM

inline std::ostream& operator<<(std::ostream &stream,
                                std::pair<size_t, std::reference_wrapper<const DAMCREM::communication::NewClientIDS2C>> &&src){
  std::ostringstream oss;
  const std::string prefix_space(src.first, ' ');
  const auto &x = src.second.get();
  oss << prefix_space << "app_id: " << *(x.app_id) << std::endl;
  oss << prefix_space << "client_id: " << *(x.client_id) << std::endl;
  stream << oss.str();
  return stream;
}


