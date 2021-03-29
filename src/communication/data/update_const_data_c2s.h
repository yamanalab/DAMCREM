#pragma once

#include"query.h"
#include"scheme_list.h"
#include"util/alias.h"
#include"util/stream.h"

namespace DAMCREM{
namespace communication{

struct UpdateConstDataC2S{
  static bool read_from_stream(std::istream &stream, AppID &ai, ClientID &ci,
                               std::vector<Plaintext> &p, std::vector<Ciphertext> &c, const SchemeList &schm_lst);
  static std::tuple<bool, UpdateConstDataC2S> read_from_stream(std::istream &stream, const SchemeList &schm_lst);
  static bool write_to_stream(std::ostream &stream, const AppID &ai, const ClientID &ci,
                              const std::vector<Plaintext> &p, const std::vector<Ciphertext> &c, const SchemeList &schm_lst);

  UpdateConstDataC2S(const AppID ai, const ClientID ci, 
                     std::vector<Plaintext> &&p, std::vector<Ciphertext> &&c, const SchemeList &schm_lst)
    : app_id(ai), client_id(ci), pt(std::move(p)), ct(std::move(c)), scheme_list(schm_lst){}
  UpdateConstDataC2S(const SchemeList &schm_lst) : scheme_list(schm_lst){}
  virtual ~UpdateConstDataC2S() = default;
  UpdateConstDataC2S(const UpdateConstDataC2S &src) noexcept = delete;
  UpdateConstDataC2S(UpdateConstDataC2S &&src) noexcept = default;

  AppID app_id;
  ClientID client_id;
  std::vector<Plaintext> pt;
  std::vector<Ciphertext> ct;
  const SchemeList &scheme_list;

  bool read_from_stream(std::istream &stream){ return read_from_stream(stream, app_id, client_id, pt, ct, scheme_list); }
  bool write_to_stream(std::ostream &stream) const {
    return write_to_stream(stream, app_id, client_id, pt, ct, scheme_list);
  }
};

inline bool UpdateConstDataC2S::read_from_stream(std::istream &stream, AppID &ai, ClientID &ci,
                                                 std::vector<Plaintext> &p, std::vector<Ciphertext> &c, const SchemeList &schm_lst){
  if( !stream ){ return false; }
  util::read(stream, ai);
  util::read(stream, ci);
  const auto &schm = schm_lst.scheme(ai, ci);
  auto read_data = [&](auto &data){
    data.resize(util::read<size_t>(stream));
    std::for_each(data.begin(), data.end(), [&](auto &t){ schm.load_data(t, stream); });
  };
  read_data(p);
  read_data(c);
  return true;
}

inline std::tuple<bool, UpdateConstDataC2S> UpdateConstDataC2S::read_from_stream(std::istream &stream, const SchemeList &schm_lst){
  UpdateConstDataC2S result(schm_lst);
  if( !stream ){ std::make_tuple(false, std::move(result)); }
  util::read(stream, result.app_id);
  util::read(stream, result.client_id);
  const auto &schm = schm_lst.scheme(result.app_id, result.client_id);
  auto read_data = [&](auto &data){
    data.resize(util::read<size_t>(stream));
    std::for_each(data.begin(), data.end(), [&](auto &t){ schm.load_data(t, stream); });
  };
  read_data(result.pt);
  read_data(result.ct);
  return std::make_tuple(true, std::move(result));
}

inline bool UpdateConstDataC2S::write_to_stream(std::ostream &stream, const AppID &ai, const ClientID &ci,
                                                const std::vector<Plaintext> &p, const std::vector<Ciphertext> &c, const SchemeList &schm_lst){
  if( !stream ){ return false; }
  util::write(stream, ai);
  util::write(stream, ci);
  const auto &schm = schm_lst.scheme(ai, ci);
  auto write_data = [&](const auto &data){
    util::write(stream, data.size());
    std::for_each(data.begin(), data.end(), [&](const auto &t){ schm.save_data(t, stream); });
  };
  write_data(p);
  write_data(c);
  return true;
}

}  // namespace communication
}  // namespace DAMCREM

inline std::ostream& operator<<(std::ostream &stream,
                                DAMCREM::util::IndentedMessageData<DAMCREM::communication::UpdateConstDataC2S> &&src){
  std::ostringstream oss;
  const std::string prefix_space(src.first, ' ');
  const auto &x = src.second.get();
  oss << prefix_space << "UpdateConstDataC2S: " << std::endl;
  const std::string prefix_space_element(src.first+2, ' ');
  oss << prefix_space << "AppID: " << x.app_id << std::endl;
  oss << prefix_space << "ClientID: " << x.client_id << std::endl;
  oss << prefix_space << "# of plaintexts: " << x.pt.size() << std::endl;
  oss << prefix_space << "# of ciphertexts: " << x.ct.size() << std::endl;
  stream << oss.str();
  return stream;
}


