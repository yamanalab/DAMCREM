#pragma once

#include<functional>

#include"scheme.h"
#include"util/alias.h"
#include"util/stream.h"

namespace DAMCREM{
namespace communication{

struct ResultS2C{
  static bool read_from_stream(std::istream &stream, AppID *ai, ClientID *ci, QueryID *qi,
                               std::vector<Plaintext> *p, std::vector<Ciphertext> *c, const Scheme &schm);
  static bool write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci, const QueryID *qi,
                              const std::vector<Plaintext> *p, const std::vector<Ciphertext> *c, const Scheme &schm);
  static bool write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci, const QueryID *qi,
                              const std::vector<std::reference_wrapper<Plaintext>> *p,
                              const std::vector<std::reference_wrapper<Ciphertext>> *c,
                              const Scheme &schm);
  static bool write_to_stream(std::ostream &stream, const AppID &ai, const ClientID &ci, const QueryID &qi,
                              const std::vector<std::reference_wrapper<Plaintext>> &p,
                              const std::vector<std::reference_wrapper<Ciphertext>> &c,
                              const Scheme &schm);

  ResultS2C(const Scheme &schm) : need_to_delete(true), app_id(new AppID{}), client_id(new ClientID{}), query_id(new QueryID{}),
                                  pt(new std::vector<Plaintext>{}), ct(new std::vector<Ciphertext>{}), scheme(schm){}
  ResultS2C(AppID *ai, ClientID *ci, QueryID *qi,
            std::vector<Plaintext> *p, std::vector<Ciphertext> *c, const Scheme &schm) noexcept
    : app_id(ai), client_id(ci), query_id(qi), pt(p), ct(c), scheme(schm){}
  virtual ~ResultS2C(){
    if( !need_to_delete ){ return; }
    delete app_id;
    delete client_id;
    delete query_id;
    delete pt;
    delete ct;
  }
  ResultS2C(const ResultS2C &src) noexcept = delete;
  ResultS2C(ResultS2C &&src) noexcept = delete;

  const bool need_to_delete = false;
  // アプリケーションIDとかは要らない？
  AppID *app_id = nullptr;
  ClientID *client_id = nullptr;
  QueryID *query_id = nullptr;
  std::vector<Plaintext> *pt = nullptr;
  std::vector<Ciphertext> *ct = nullptr;
  const Scheme &scheme;

  bool read_from_stream(std::istream &stream){ return read_from_stream(stream, app_id, client_id, query_id, pt, ct, scheme); }

  bool write_to_stream(std::ostream &stream){ return write_to_stream(stream, app_id, client_id, query_id, pt, ct, scheme); }
};


inline bool ResultS2C::read_from_stream(std::istream &stream, AppID *ai, ClientID *ci, QueryID *qi,
                                        std::vector<Plaintext> *p, std::vector<Ciphertext> *c, const Scheme &schm){
  if( !stream ){ return false; }
  util::read(stream, ai);
  util::read(stream, ci);
  util::read(stream, qi);
  size_t n = 0;
  util::read(stream, &n);
  p->resize(n);
  std::for_each(p->begin(), p->end(), [&](Plaintext &t){ schm.load_data(t, stream); });
  util::read(stream, &n);
  c->resize(n);
  std::for_each(c->begin(), c->end(), [&](Ciphertext &t){ schm.load_data(t, stream); });
  return true;
}

inline bool ResultS2C::write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci, const QueryID *qi,
                                       const std::vector<Plaintext> *p, const std::vector<Ciphertext> *c, const Scheme &schm){
  if( !stream ){ return false; }
  util::write(stream, ai);
  util::write(stream, ci);
  util::write(stream, qi);
  size_t n = p->size();
  util::write(stream, &n);
  std::for_each(p->begin(), p->end(), [&](const Plaintext &t){ schm.save_data(t, stream); });
  n = c->size();
  util::write(stream, &n);
  std::for_each(c->begin(), c->end(), [&](const Ciphertext &t){ schm.save_data(t, stream); });
  return true;
}

inline bool ResultS2C::write_to_stream(std::ostream &stream, const AppID *ai, const ClientID *ci, const QueryID *qi,
                                       const std::vector<std::reference_wrapper<Plaintext>> *p,
                                       const std::vector<std::reference_wrapper<Ciphertext>> *c,
                                       const Scheme &schm){
  if( !stream ){ return false; }
  util::write(stream, ai);
  util::write(stream, ci);
  util::write(stream, qi);
  size_t n = p->size();
  util::write(stream, &n);
  std::for_each(p->begin(), p->end(), [&](const std::reference_wrapper<Plaintext> &t){ schm.save_data(t.get(), stream); });
  n = c->size();
  util::write(stream, &n);
  std::for_each(c->begin(), c->end(), [&](const std::reference_wrapper<Ciphertext> &t){ schm.save_data(t.get(), stream); });
  return true;
}

inline bool ResultS2C::write_to_stream(std::ostream &stream, const AppID &ai, const ClientID &ci, const QueryID &qi,
                                       const std::vector<std::reference_wrapper<Plaintext>> &p,
                                       const std::vector<std::reference_wrapper<Ciphertext>> &c,
                                       const Scheme &schm){
  if( !stream ){ return false; }
  util::write(stream, ai);
  util::write(stream, ci);
  util::write(stream, qi);
  auto write_data = [&](const auto &data){
    util::write(stream, data.size());
    std::for_each(data.begin(), data.end(), [&](const auto &t){ schm.save_data(t.get(), stream); });
  };
  write_data(p);
  write_data(c);
  return true;
}


}  // namespace communication
}  // namespace DAMCREM

inline std::ostream& operator<<(std::ostream &stream,
                                DAMCREM::util::IndentedMessageData<DAMCREM::communication::ResultS2C> &&src){
  std::ostringstream oss;
  const std::string prefix_space(src.first, ' ');
  const auto &x = src.second.get();
  oss << prefix_space << "app_id: " << *(x.app_id) << std::endl;
  oss << prefix_space << "client_id: " << *(x.client_id) << std::endl;
  oss << prefix_space << "query_id: " << *(x.query_id) << std::endl;
  stream << oss.str();
  return stream;
}


