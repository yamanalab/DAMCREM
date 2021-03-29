#pragma once

#include<algorithm>
#include<filesystem>
#include<fstream>
#include<iostream>
#include<stdexcept>
#include<unordered_map>

#include"util/string.h"

namespace DAMCREM{
namespace util{

template<class T>
using IndentedMessageData = std::pair<size_t, std::reference_wrapper<const T>>;

template<class T>
inline auto makeIndentedMessageData(const T &src, size_t n=0){
  return std::make_pair<size_t, std::reference_wrapper<const T>>(std::move(n), src);
}

inline std::vector<std::string> load_text_from_file(const std::filesystem::path &file_path, bool &is_success){
  std::vector<std::string> data;
  std::string buf;
  std::ifstream ifs(file_path);

  is_success = static_cast<bool>(ifs);
  if( is_success ){
    while( std::getline(ifs, buf) ){
      data.emplace_back(std::move(buf));
    }
  }
      
  return data;
}

inline std::vector<std::string> load_text_from_file(const std::filesystem::path &file_path){
  bool is_success = true;
  auto result = load_text_from_file(file_path, is_success);
  if( !is_success ){
    throw std::runtime_error(file_path.string()+" does not exist.");
  }
  return result;
}

inline std::unordered_map<std::string, std::string> parse_data_from_file(const std::vector<std::string> &input){
  std::unordered_map<std::string, std::string> data;
  for( const std::string &x : input ){
    const std::vector<std::string> ss = util::split_str(x, "=");
    if( (ss.size() == 0) || ss.at(0).at(0) == '#' ){ continue; }
    if( ss.size() != 2 ){ throw std::runtime_error("Invalid data: " + x); }
    if( data.try_emplace(ss.at(0)+"=", ss.at(1)).second ){ continue; }
    throw std::runtime_error("Invalid data: " + x + " has already been existed.");
  }
  return data;
}

    
inline std::unordered_map<std::string, std::string> parse_arguments(const std::vector<std::string> &argv,
                                                                    const std::unordered_map<std::string, std::string> &args_keywords,
                                                                    const std::unordered_map<std::string, std::string> &default_args){
  std::unordered_map<std::string, std::string> args = default_args;
  for( const auto &x : argv ){
    std::vector<std::string> arg = util::split_str(x, "=");
    if( arg.size() == 2 ){
      arg[0] += "=";
    }
    if( args_keywords.find(arg[0]) == args_keywords.end() ){
      throw std::invalid_argument(arg[0]+" is not keyword");
    }
    args[arg[0]] = (arg.size()==2 ? arg[1] : "true");
  }
    
  return args;
}
    
inline std::unordered_map<std::string, std::string> parse_arguments(int argc, char *argv[],
                                                                    const std::unordered_map<std::string, std::string> &args_keywords,
                                                                    const std::unordered_map<std::string, std::string> &default_args){
  std::vector<std::string> tmp;
  for( int i = 1; i < argc; ++i ){
    tmp.emplace_back(argv[i]);
  }
  return parse_arguments(tmp, args_keywords, default_args);
}

inline std::vector<std::string> get_indices(const std::string &str){
  std::vector<std::string> indices = split_str(str, "]");
  indices.pop_back();
  std::transform(indices.begin(), indices.end(), indices.begin(),
                 [](const std::string s){ return split_str(s, "[").at(1); });
  return indices;
}

template<class T>
inline void read(std::istream &stream, T &t){
  stream.read(reinterpret_cast<char*>(&t), sizeof(t));
  return;
}

template<class T>
inline void read(std::istream &stream, T *t){
  stream.read(reinterpret_cast<char*>(t), sizeof(*t));
  return;
}

template<class T>
inline T read(std::istream &stream){
  T t;
  stream.read(reinterpret_cast<char*>(&t), sizeof(t));
  return t;
}

template<class T>
inline void write(std::ostream &stream, const T &t){
  stream.write(reinterpret_cast<const char*>(&t), sizeof(t));
  return;
}

template<class T>
inline void write(std::ostream &stream, const T *t){
  stream.write(reinterpret_cast<const char*>(t), sizeof(*t));
  return;
}



}  // namespace util
}  // namespace DAMCREM

