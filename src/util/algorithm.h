#pragma once

#include<algorithm>
#include<vector>

namespace DAMCREM{
namespace util{
//! [s,e)をstepずつインクリメントしながら数列を生成する関数
/*!
 * @param[in] s The first value
 * @param[in] e The next value of the last value
 * @param[in] step Size of increment
 */
template<class T>
inline std::vector<T> sequence(size_t s, size_t e, size_t step=1){
  std::vector<T> result;
  result.reserve((e-s+step-1)/step);
  for( size_t i = s; i < e; i+=step ){ result.emplace_back(i); }
  return result;
}

//! std::for_each()で，イテレータを実装するコンテナを引数として取れるようにした関数
template<class T, class Function>
inline Function for_each(T &&src, Function f){
  return std::move(std::for_each(src.begin(), src.end(), f));
}

//! std::transform()で，イテレータを実装するコンテナを引数(入力)として取れるようにした関数
template<class T, class OutputIterator, class UnaryOperation>
inline OutputIterator transform(T &&src, OutputIterator result, UnaryOperation op){
  return std::transform(src.begin(), src.end(), result, op);
}

//! std::transform()で，イテレータを実装するコンテナを引数(入力)として取れるようにした関数
template<class T, class U, class UnaryOperation>
inline typename U::iterator transform(T &&src, U &result, UnaryOperation op){
  return std::transform(src.begin(), src.end(), std::inserter(result, result.end()), op);
}

//! std::transform()で，イテレータを実装するコンテナを引数(入力)として取れるようにした関数
template<class T, class U, class V, class UnaryOperation>
inline typename U::iterator transform(T &&src0, U &&src1, V &result, UnaryOperation op){
  return std::transform(src0.begin(), src0.end(), src1.begin(), std::inserter(result, result.end()), op);
}

template<class T, class U, class UnaryOperation>
inline typename std::vector<U> transform_return(T &&src, std::vector<U> &&dst, UnaryOperation op){
  transform(std::forward<T>(src), std::back_inserter(dst), op);
  return std::move(dst);
}

template<class T, class U>
inline std::vector<T> get_sorted_id(const std::unordered_map<T, U> &src){
  std::vector<T> result;
  transform(src, std::back_inserter(result), [](const auto &s){ return s.first; });
  std::sort(result.begin(), result.end());
  return result;
}
  
    
}
}

