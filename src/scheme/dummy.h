#pragma once

#include<iomanip>

#include"util.h"

namespace DAMCREM{
inline const std::string& Scheme::scheme_name() noexcept {
  static const std::string name = "dummy";
  return name;
}
  
inline void Scheme::encode(Plaintext &v0, const std::vector<double> &v1, size_t nt) const {
  v0.scheme_info = &scheme_info_;
  v0.level = v0.scheme_info->L();
  v0.data.clear();
  v0.data.resize(v0.level+1, v1);
  return;
}

inline void Scheme::decode(std::vector<double> &v0, const Plaintext &v1, size_t nt) const {
  v0 = v1.data.at(0);
  return;
}

inline void Scheme::encrypt(Ciphertext &v0, const Plaintext &v1, size_t nt) const {
  v0.scheme_info = &scheme_info_;
  v0.level = v1.level;
  v0.length = 2;
  v0.data.clear();
  v0.data.resize(2);
  for( size_t l = 0; l < v0.length; ++l ){
    v0.data.at(l) = v1.data;
  }
  return;
}

inline void Scheme::encrypt(Ciphertext &v0, const std::vector<double> &v1, size_t nt) const {
  v0.scheme_info = &scheme_info_;
  v0.level = v0.scheme_info->L();
  v0.length = 2;
  v0.data.clear();
  v0.data.resize(v0.length, {v0.level+1, v1});
  return;
}

inline void Scheme::decrypt(Plaintext &v0, const Ciphertext &v1, size_t nt) const {
  // v1のチェックを入れる．
  v0.scheme_info = &scheme_info_;
  v0.data = v1.data.at(0);
  v0.level = v1.level;
  return;
}


template<>
inline bool Scheme::load_data<Plaintext>(Plaintext &pt, std::istream &stream) const {
  if( !stream ){ return false; }
  pt.data.clear();
  stream.read(reinterpret_cast<char*>(&pt.level), sizeof(pt.level));
  pt.data.resize(pt.level+1);
  for( size_t i = 0; i <= pt.level; ++i ){
    pt.data.at(i).resize(scheme_info_.N());
    for( size_t j = 0, n = scheme_info_.N(); j < n; ++j ){
      stream.read(reinterpret_cast<char*>(&pt.data.at(i).at(j)), sizeof(pt.data.at(i).at(j)));
    }
  }
  pt.scheme_info = &scheme_info_;
  return true;
}

template<>
inline bool Scheme::load_data<Ciphertext>(Ciphertext &ct, std::istream &stream) const {
  if( !stream ){ return false; }
  ct.data.clear();
  stream.read(reinterpret_cast<char*>(&ct.length), sizeof(ct.length));
  stream.read(reinterpret_cast<char*>(&ct.level), sizeof(ct.level));
  ct.data.resize(ct.length);
  for( size_t k = 0; k < ct.length; ++k ){
    ct.data.at(k).resize(ct.level+1);
    for( size_t i = 0; i <= ct.level; ++i ){
      ct.data.at(k).at(i).resize(scheme_info_.N());
      for( size_t j = 0, n = scheme_info_.N(); j < n; ++j ){
        stream.read(reinterpret_cast<char*>(&ct.data.at(k).at(i).at(j)), sizeof(ct.data.at(k).at(i).at(j)));
      }
    }
  }
  ct.scheme_info = &scheme_info_;
  return true;
}

template<class T>
inline bool Scheme::load_data(T &t, const std::filesystem::path &file_path) const {
  std::ifstream ifs(file_path, std::ios::binary);
  if( !load_data(t, ifs) ){
    throw std::invalid_argument(file_path.string()+" does not exist.");
  }
  return true;
}

template<class T>
inline T Scheme::load_data(const std::filesystem::path &file_path) const {
  T t{};
  load_data(t, file_path);
  return t;
}

template<>
inline bool Scheme::save_data<Plaintext>(const Plaintext &pt, std::ostream &stream) const {
  if( !stream ){ return false; }
  stream.write(reinterpret_cast<const char*>(&pt.level), sizeof(pt.level));
  for( size_t i = 0; i <= pt.level; ++i ){
    for( size_t j = 0, n = scheme_info_.N(); j < n; ++j ){
      stream.write(reinterpret_cast<const char*>(&pt.data.at(i).at(j)), sizeof(SchemeInfo::DataType));
    }
  }
  return true;
}

template<>
inline bool Scheme::save_data<Ciphertext>(const Ciphertext &ct, std::ostream &stream) const {
  if( !stream ){ return false; }
  stream.write(reinterpret_cast<const char*>(&ct.length), sizeof(ct.length));
  stream.write(reinterpret_cast<const char*>(&ct.level), sizeof(ct.level));
  for( size_t k = 0; k < ct.length; ++k ){
    for( size_t i = 0; i <= ct.level; ++i ){
      for( size_t j = 0, n = scheme_info_.N(); j < n; ++j ){
        stream.write(reinterpret_cast<const char*>(&ct.data.at(k).at(i).at(j)), sizeof(SchemeInfo::DataType));
      }
    }
  }
  return true;
}

template<class T>
inline bool Scheme::save_data(const T &t, const std::filesystem::path &file_path) const {
  std::ofstream ofs(file_path, std::ios::binary);
  if( !save_data<T>(t, ofs) ){
    throw std::invalid_argument(file_path.string()+" does not exist.");
  }
  return true;
}
  
inline void Scheme::copy(Plaintext &v0, const Plaintext &v1, size_t nt) const {
  v0 = v1;
}

inline void Scheme::copy(Ciphertext &v0, const Ciphertext &v1, size_t nt) const {
  v0 = v1;
}
  
inline void Scheme::add(Ciphertext &v0, const Plaintext &v1, size_t nt) const {
  if( (v0.scheme_info == nullptr) || (v0.scheme_info->N() != v1.scheme_info->N()) ||
      (v0.scheme_info->num_slot() != v1.scheme_info->num_slot()) || (v0.level != v1.level) ){
    util::dbg_msg << v0 << std::endl;
    util::dbg_msg << v1 << std::endl;
    throw std::invalid_argument("invalid input @ add(Ciphertext&, const Plaintext&, size_nt)");
  }
// #pragma omp parallel for num_threads(nt)
  for( size_t i = 0; i <= v0.level; ++i ){
    for( size_t j = 0; j < v0.scheme_info->N(); ++j ){
      v0.data.at(0).at(i).at(j) += v1.data.at(i).at(j);
    }
  }
}

inline void Scheme::add(Ciphertext &v0, const Ciphertext &v1, size_t nt) const {
  if( (v0.scheme_info == nullptr) || (v0.scheme_info != v1.scheme_info) || (v0.level != v1.level) ){
    util::dbg_msg << v0 << std::endl;
    util::dbg_msg << v1 << std::endl;
    throw std::invalid_argument("invalid input @ add(Ciphertext&, const Ciphertext&, size_nt)");
  }
  v0.length = std::max(v0.length, v1.length);
  v0.data.resize(v0.length, {v0.level+1, std::vector<double>(v0.scheme_info->N())});
// #pragma omp parallel for num_threads(nt) collapse(2)
  for( size_t l = 0; l < v1.length; ++l ){
    for( size_t i = 0; i <= v0.level; ++i ){
      for( size_t j = 0; j < v0.scheme_info->N(); ++j ){
        v0.data.at(l).at(i).at(j) += v1.data.at(l).at(i).at(j);
      }
    }
  }
}

inline void Scheme::sub(Ciphertext &v0, const Plaintext &v1, size_t nt) const {
  if( (v0.scheme_info == nullptr) || (v0.scheme_info->N() != v1.scheme_info->N()) ||
      (v0.scheme_info->num_slot() != v1.scheme_info->num_slot()) || (v0.level != v1.level) ){
    util::dbg_msg << v0 << std::endl;
    util::dbg_msg << v1 << std::endl;
    throw std::invalid_argument("invalid input @ sub(Ciphertext&, const Plaintext&, size_nt)");
  }
// #pragma omp parallel for num_threads(nt)
  for( size_t i = 0; i <= v0.level; ++i ){
    for( size_t j = 0; j < v0.scheme_info->N(); ++j ){
      v0.data.at(0).at(i).at(j) += v1.data.at(i).at(j);
    }
  }
}

inline void Scheme::sub(Ciphertext &v0, const Ciphertext &v1, size_t nt) const {
  if( (v0.scheme_info == nullptr) || (v0.scheme_info != v1.scheme_info) || (v0.level != v1.level) ){
    util::dbg_msg << v0 << std::endl;
    util::dbg_msg << v1 << std::endl;
    throw std::invalid_argument("invalid input @ sub(Ciphertext&, const Ciphertext&, size_nt)");
  }
  v0.length = std::max(v0.length, v1.length);
  v0.data.resize(v0.length, {v0.level+1, std::vector<double>(v0.scheme_info->N())});
// #pragma omp parallel for num_threads(nt) collapse(2)
  for( size_t l = 0; l < v1.length; ++l ){
    for( size_t i = 0; i <= v0.level; ++i ){
      for( size_t j = 0; j < v0.scheme_info->N(); ++j ){
        v0.data.at(l).at(i).at(j) -= v1.data.at(l).at(i).at(j);
      }
    }
  }
}

inline void Scheme::mul(Ciphertext &v0, const Plaintext &v1, size_t nt) const {
  if( (v0.scheme_info == nullptr) || (v0.scheme_info->N() != v1.scheme_info->N()) ||
      (v0.scheme_info->num_slot() != v1.scheme_info->num_slot()) || (v0.level != v1.level) ){
    util::dbg_msg << v0 << std::endl;
    util::dbg_msg << v1 << std::endl;
    throw std::invalid_argument("invalid input @ mul(Ciphertext&, const Plaintext&, size_nt)");
  }
// #pragma omp parallel for num_threads(nt) collapse(2)
  for( size_t l0 = 0; l0 < v0.length; ++l0 ){
    for( size_t i = 0; i <= v0.level; ++i ){
      for( size_t j = 0; j < v0.scheme_info->N(); ++j ){
        v0.data.at(l0).at(i).at(j) *= v1.data.at(i).at(j);
      }
    }
  }
}

inline void Scheme::mul(Ciphertext &v0, const Ciphertext &v1, size_t nt) const {
  if( (v0.scheme_info == nullptr) || (v0.scheme_info != v1.scheme_info) || (v0.level != v1.level) ){
    util::dbg_msg << v0 << std::endl;
    util::dbg_msg << v1 << std::endl;
    throw std::invalid_argument("invalid input @ mul(Ciphertext&, const Ciphertext&, size_nt)");
  }
  size_t new_length = v0.length + v1.length - 1;
  std::vector<std::vector<std::vector<double>>> t(new_length, {v0.level+1, std::vector<double>(v0.scheme_info->N())});
// #pragma omp parallel for num_threads(nt) collapse(2)
  for( size_t l = 0; l < new_length; ++l ){
    for( size_t i = 0; i <= v0.level; ++i ){
      for( size_t l0 = 0; l0 < v0.length; ++l0 ){
        if( l0 > l ){ continue; }
        size_t l1 = l - l0;
        if( l1 >= v1.length ){ continue; }
        for( size_t j = 0; j < v0.scheme_info->N(); ++j ){
          t.at(l).at(i).at(j) = v0.data.at(l0).at(i).at(j) * v1.data.at(l1).at(i).at(j);
        }
      }
    }
  }
  std::swap(v0.data, t);
  v0.length = new_length;
}

inline void Scheme::square(Ciphertext &v0, size_t nt) const {
  if( v0.scheme_info == nullptr ){
    util::dbg_msg << v0 << std::endl;
    throw std::invalid_argument("invalid input @ square(Ciphertext&, size_nt)");
  }

  size_t new_length = v0.length + v0.length - 1;
  std::vector<std::vector<std::vector<double>>> t(new_length, {v0.level+1, std::vector<double>(v0.scheme_info->N())});
// #pragma omp parallel for num_threads(nt) collapse(2)
  for( size_t l = 0; l < new_length; ++l ){
    for( size_t i = 0; i <= v0.level; ++i ){
      for( size_t l0 = 0; l0 < v0.length; ++l0 ){
        if( l0 > l ){ continue; }
        size_t l1 = l - l0;
        if( l1 >= v0.length ){ continue; }
        for( size_t j = 0; j < v0.scheme_info->N(); ++j ){
          t.at(l).at(i).at(j) = v0.data.at(l0).at(i).at(j) * v0.data.at(l1).at(i).at(j);
        }
      }
    }
  }
  std::swap(v0.data, t);
  v0.length = new_length;
}
  
inline void Scheme::relinearize(Ciphertext &v0, size_t nt) const {
  if( v0.length > 2 ){
    --(v0.length);
    v0.data.resize(v0.length);
  }else{
    throw std::invalid_argument("Error @ relinearization");
  }
}

inline void Scheme::rescale(Ciphertext &v0, size_t nt) const {
  if( v0.level > 0 ){
    --(v0.level);
    for( auto &x : v0.data ){
      x.resize(v0.level+1);
    }
  }else{
    throw std::invalid_argument("Error @ rescale");
  }
}

inline void Scheme::simple_modulus_reduction(Plaintext &v0, size_t nt) const {
  if( v0.level > 0 ){
    --(v0.level);
    v0.data.resize(v0.level+1);
  }else{
    throw std::invalid_argument("Error @ simple_modulus_reduction");
  }
}

inline void Scheme::simple_modulus_reduction(Ciphertext &v0, size_t nt) const {
  if( v0.level > 0 ){
    --(v0.level);
    std::for_each(v0.data.begin(), v0.data.end(), [&](auto &x){ x.resize(v0.level+1); });
  }else{
    throw std::invalid_argument("Error @ simple_modulus_reduction");
  }
}

inline void Scheme::bootstrap(Ciphertext &v0, size_t nt) const {
  if( v0.scheme_info == nullptr ){
    util::dbg_msg << v0 << std::endl;
    throw std::invalid_argument("invalid input @ bootstrap(Ciphertext&, size_nt)");
  }
  v0.level = v0.scheme_info->L();
  for( auto &x : v0.data ){
    x.resize(v0.level+1, x.at(0));
  }
}
  
}

