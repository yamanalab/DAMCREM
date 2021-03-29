#pragma once

#include"scheme.h"
#include"util/alias.h"
#include"util/stream.h"

namespace DAMCREM{
namespace communication{

struct ResultC2S{
  static bool read_from_stream(std::istream &stream, AppID *ai, ClientID *ci, QueryID *qi);
  static bool write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci, const QueryID *qi);

  ResultC2S() : need_to_delete(true), app_id(new AppID{}), client_id(new ClientID{}), query_id(new QueryID{}){}
  ResultC2S(AppID *ai, ClientID *ci, QueryID *qi) noexcept : app_id(ai), client_id(ci), query_id(qi){}
  virtual ~ResultC2S(){
    if( !need_to_delete ){ return; }
    delete app_id;
    delete client_id;
    delete query_id;
  }
  ResultC2S(const ResultC2S &src) noexcept = delete;
  ResultC2S(ResultC2S &&src) noexcept = delete;

  const bool need_to_delete = false;
  AppID *app_id = nullptr;
  ClientID *client_id = nullptr;
  QueryID *query_id = nullptr;

  bool read_from_stream(std::istream &stream){ return read_from_stream(stream, app_id, client_id, query_id); }

  bool write_to_stream(std::ostream &stream){ return write_to_stream(stream, app_id, client_id, query_id); }
};


inline bool ResultC2S::read_from_stream(std::istream &stream, AppID *ai, ClientID *ci, QueryID *qi){
  if( !stream ){ return false; }
  util::read(stream, ai);
  util::read(stream, ci);
  util::read(stream, qi);
  return true;
}

inline bool ResultC2S::write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci, const QueryID *qi){
  if( !stream ){ return false; }
  util::write(stream, ai);
  util::write(stream, ci);
  util::write(stream, qi);
  return true;
}

}  // namespace communication
}  // namespace DAMCREM

inline std::ostream& operator<<(std::ostream &stream,
                                DAMCREM::util::IndentedMessageData<DAMCREM::communication::ResultC2S> &&src){
  std::ostringstream oss;
  const std::string prefix_space(src.first, ' ');
  const auto &x = src.second.get();
  oss << prefix_space << "ResultC2S:" << std::endl;
  const std::string prefix_space_element(src.first+2, ' ');
  oss << prefix_space_element << "app_id: " << *(x.app_id) << std::endl;
  oss << prefix_space_element << "client_id: " << *(x.client_id) << std::endl;
  oss << prefix_space_element << "query_id: " << *(x.query_id) << std::endl;
  stream << oss.str();
  return stream;
}


