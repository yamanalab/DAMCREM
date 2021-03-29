#pragma once

#include"util/alias.h"
#include"util/stream.h"

namespace DAMCREM{
namespace communication{

struct NewClientIDC2S{
  static bool read_from_stream(std::istream &stream, AppID *ai);
  static bool write_to_stream(std::ostream &stream, const AppID *ai);
  NewClientIDC2S() : need_to_delete(true), app_id(new AppID{}){}
  NewClientIDC2S(AppID *ai) noexcept : app_id(ai){}
  virtual ~NewClientIDC2S(){
    if( !need_to_delete ){ return; }
    delete app_id;
  }
  NewClientIDC2S(const NewClientIDC2S &src) noexcept = delete;
  NewClientIDC2S(NewClientIDC2S &&src) noexcept = delete;

  const bool need_to_delete = false;
  AppID *app_id = nullptr;

  bool read_from_stream(std::istream &stream){ return read_from_stream(stream, app_id); }

  bool write_to_stream(std::ostream &stream) const { return write_to_stream(stream, app_id); }
  
};

inline bool NewClientIDC2S::read_from_stream(std::istream &stream, AppID *ai){
  if( !stream ){ return false; }
  util::read(stream, ai);
  return true;
}

inline bool NewClientIDC2S::write_to_stream(std::ostream &stream, const AppID *ai){
  if( !stream ){ return false; }
  util::write(stream, ai);
  return true;
}


}  // namespace communication
}  // namespace DAMCREM

inline std::ostream& operator<<(std::ostream &stream,
                                std::pair<size_t, std::reference_wrapper<const DAMCREM::communication::NewClientIDC2S>> &&src){
  std::ostringstream oss;
  const std::string prefix_space(src.first, ' ');
  const auto &x = src.second.get();
  oss << prefix_space << "app_id: " << *(x.app_id) << std::endl;
  stream << oss.str();
  return stream;
}

