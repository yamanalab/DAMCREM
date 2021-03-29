#pragma once

#include<filesystem>
#include<sstream>
#include<stdexcept>
#include<string>
#include<vector>

namespace DAMCREM{
namespace util{
template<class T>
inline T convert_string(const std::string &src){
  throw std::logic_error("convert_string<"+std::string(typeid(T).name())+"> is not implemented.");
  return T();
}

template<>
inline bool convert_string<bool>(const std::string &src){ return ((src!="false") && (src!="0")); }
    
template<>
inline int convert_string<int>(const std::string &src){ return std::stoi(src); }

template<>
inline long convert_string<long>(const std::string &src){ return std::stol(src); }

template<>
inline unsigned long convert_string<unsigned long>(const std::string &src){ return std::stoul(src); }

template<>
inline uint32_t convert_string<uint32_t>(const std::string &src){ return static_cast<uint32_t>(convert_string<uint64_t>(src)); }

template<>
inline float convert_string<float>(const std::string &src){ return std::stof(src); }

template<>
inline double convert_string<double>(const std::string &src){ return std::stod(src); }

template<class T=std::string>
inline std::vector<T> split_str(const std::string &str, const std::string &dlmt,
                                bool do_strip=true, bool do_remove_empty=false){
  std::vector<std::string> splitted_str;
  if( str.size() == 0 ){ return std::vector<T>(); }
  size_t l_str = str.size(), l_dlmt = dlmt.size();
  size_t s = 0;
  for( size_t i = 0; i < l_str; ++i ){
    if( str.at(i) != dlmt.at(0) ){ continue; }
    bool is_matched = true;
    if( l_str-i < l_dlmt ){ break; }
    for( size_t j = 1; j < l_dlmt; ++j ){
      is_matched &= (str.at(i+j)==dlmt.at(j));
    }
    if( is_matched ){
      splitted_str.emplace_back(str.substr(s, i-s));
      s = i + l_dlmt;
      i = s - 1;  // 直後に実行されるforの繰り返し処理で1を足すため1を引く
    }
  }
  splitted_str.emplace_back(str.substr(s, std::string::npos));

  if( do_strip ){
    for( auto &x : splitted_str ){
      size_t s = 0, e = 0, l = x.size();
      for( e = 0; (e < l) && (x.at(l-e-1) == ' '); ++e ){}
      l -= e;
      for( s = 0; (s < l) && (x.at(s) == ' '); ++s ){}
      x = x.substr(s, l-s);
    }
  }

  if( do_remove_empty ){
    std::remove_if(splitted_str.begin(), splitted_str.end(), [](const std::string &x){ return x.size()==0; });
  }

  if constexpr( std::is_same<std::string, T>::value ){
    return splitted_str;
  }
  std::vector<T> result;
  std::transform(splitted_str.begin(), splitted_str.end(), std::back_inserter(result), convert_string<T>);
  return result;
}

inline std::ostringstream join_str(const std::vector<std::string> &str, const std::string &dlmt){
  std::ostringstream joined_oss;
  if( str.size() == 0 ){ return joined_oss; }
  joined_oss << str.at(0);
  std::for_each(str.begin()+1, str.end(), [&](const std::string &s){ joined_oss << dlmt << s; });
  return joined_oss;
}
    
inline std::string replace_str(const std::string &str, const std::string &old_word, const std::string &new_word){
  return join_str(split_str(str, old_word, false, false), new_word).str();
}

inline std::filesystem::path join_path_ex(std::filesystem::path &&result){
  return std::move(result);
}
    
template<class Head, class... Tail>
inline std::filesystem::path join_path_ex(std::filesystem::path &&result, Head &&head, Tail&&... tail){
  if constexpr( std::is_same<Head, std::filesystem::path>::value ||
                std::is_same<Head, std::filesystem::path&>::value ||
                std::is_same<Head, const std::filesystem::path&>::value ){
    result /= head;
  }else{
    std::ostringstream oss;
    oss << head;
    result /= std::filesystem::path(oss.str());
  }
  return join_path_ex(std::move(result), std::forward<Tail>(tail)...);
}

template<class... Args>
inline std::filesystem::path join_path(Args&&... args){
  return join_path_ex(std::filesystem::path(), std::forward<Args>(args)...);
}

    
}  // namespace util
}  // namespace DAMCREM

