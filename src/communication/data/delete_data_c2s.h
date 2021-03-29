#pragma once

#include"scheme.h"
#include"util/alias.h"
#include"util/stream.h"

namespace DAMCREM{
namespace communication{

struct DeleteDataC2S{
  static bool read_from_stream(std::istream &stream, AppID &ai, ClientID &ci);
  static bool write_to_stream(std::ostream &stream, const AppID ai, const ClientID ci);

  DeleteDataC2S(){}
  DeleteDataC2S(AppID ai, ClientID ci) noexcept : app_id(ai), client_id(ci){}
  virtual ~DeleteDataC2S() = default;
  DeleteDataC2S(const DeleteDataC2S &src) noexcept = delete;
  DeleteDataC2S(DeleteDataC2S &&src) noexcept = delete;

  const bool need_to_delete = false;
  AppID app_id;
  ClientID client_id;

  bool read_from_stream(std::istream &stream){ return read_from_stream(stream, app_id, client_id); }

  bool write_to_stream(std::ostream &stream){ return write_to_stream(stream, app_id, client_id); }
};


inline bool DeleteDataC2S::read_from_stream(std::istream &stream, AppID &ai, ClientID &ci){
  if( !stream ){ return false; }
  util::read(stream, &ai);
  util::read(stream, &ci);
  return true;
}

inline bool DeleteDataC2S::write_to_stream(std::ostream &stream, const AppID ai, const ClientID ci){
  if( !stream ){ return false; }
  util::write(stream, ai);
  util::write(stream, ci);
  return true;
}

}  // namespace communication
}  // namespace DAMCREM

inline std::ostream& operator<<(std::ostream &stream,
                                DAMCREM::util::IndentedMessageData<DAMCREM::communication::DeleteDataC2S> &&src){
  std::ostringstream oss;
  const std::string prefix_space(src.first, ' ');
  const auto &x = src.second.get();
  oss << prefix_space << "app_id: " << x.app_id << std::endl;
  oss << prefix_space << "client_id: " << x.client_id << std::endl;
  stream << oss.str();
  return stream;
}


