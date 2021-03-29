#pragma once

/*!
 * @file scheme_info/dummy.h
 * @brief This is just dummy. It is for test. 
 *        So, this includes some bad code such as raw pointer, but it is no problem.
 * @author Takuya Suzuki
 * @date 2020/12/08
 */

#include<vector>
#include<fstream>

#include"util/stream.h"

namespace DAMCREM{
class SchemeInfo{
public:
  using DataType = double;
    
  SchemeInfo() : N_(0), num_slot_(0), L_(0){}
    
  SchemeInfo(size_t N, size_t num_slot, size_t L) : N_(N), num_slot_(num_slot), L_(L){}

  SchemeInfo(const SchemeInfo &src) noexcept = default;

  SchemeInfo(SchemeInfo &&src) noexcept = default;

  SchemeInfo(std::filesystem::path file_path_prefix){
    std::ifstream ifs((file_path_prefix+="_info.txt").string());
    if( !ifs ){
      throw std::invalid_argument(file_path_prefix.string()+" does not exist.");
    }
    ifs >> N_;
    ifs >> num_slot_;
    ifs >> L_;
  }

  bool operator==(const SchemeInfo &src){
    if( N() != src.N() ){ return false; }
    if( num_slot() != src.num_slot() ){ return false; }
    if( L() != src.L() ){ return false; }
    return true;
  }

  bool operator!=(const SchemeInfo &src){
    return !(*this==src);
  }
    
  size_t N() const noexcept { return N_; }
  size_t num_slot() const noexcept { return num_slot_; }
  size_t L() const noexcept { return L_; }
    
  bool read_from_stream(std::istream &stream){
    if( !stream ){ return false; }
    util::read(stream, &N_);
    util::read(stream, &num_slot_);
    util::read(stream, &L_);
    return true;
  }

  bool write_to_stream(std::ostream &stream) const {
    if( !stream ){ return false; }
    util::write(stream, &N_);
    util::write(stream, &num_slot_);
    util::write(stream, &L_);
    return true;
  }
    
    
private:
  size_t N_;
  size_t num_slot_;
  size_t L_;
};
  
class Plaintext{
public:
  Plaintext(){}
  Plaintext(const Plaintext &src) noexcept
    : data(src.data), level(src.level), scheme_info(src.scheme_info){}
  Plaintext(Plaintext &&src) noexcept
    : data(std::move(src.data)), level(src.level), length(src.length), scheme_info(src.scheme_info){}
  Plaintext& operator=(const Plaintext &src) noexcept {
    this->data = src.data;
    this->level = src.level;
    this->scheme_info = src.scheme_info;
    return *this;
  }
  std::vector<std::vector<SchemeInfo::DataType>> data = {};
  size_t level = 0;
  const size_t length = 1;
  const SchemeInfo *scheme_info = nullptr;
};
  
class Ciphertext{
public:
  Ciphertext(){}
  Ciphertext(const Ciphertext &src) noexcept
    : data(src.data), level(src.level), length(src.length), scheme_info(src.scheme_info){}
  Ciphertext(Ciphertext &&src) noexcept 
    : data(std::move(src.data)), level(src.level), length(src.length), scheme_info(src.scheme_info){}
  Ciphertext& operator=(const Ciphertext &src) noexcept {
    this->data = src.data;
    this->level = src.level;
    this->length = src.length;
    scheme_info = src.scheme_info;
    return *this;
  }
  std::vector<std::vector<std::vector<SchemeInfo::DataType>>> data;
  size_t level = 0;
  size_t length = 2;
  const SchemeInfo *scheme_info = nullptr;
};
  
  
}

inline std::ostream& operator<<(std::ostream &stream, const DAMCREM::Plaintext &src){
  std::ostringstream oss;
  oss << "pt: " << src.level << ", " << src.length;
  stream << oss.str();
  return stream;
}

inline std::ostream& operator<<(std::ostream &stream, const DAMCREM::Ciphertext &src){
  std::ostringstream oss;
  oss << "ct: " << src.level << ", " << src.length;
  size_t n = src.data.size();
  oss << ", (" << n;
  if( n > 0 ){
    n = src.data.at(0).size();
    oss << ", " << n;
  }
  if( n > 0 ){
    n = src.data.at(0).at(0).size();
    oss << ", " << n;
  }
  oss << ")";
  stream << oss.str();
  return stream;
}


