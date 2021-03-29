#pragma once

#include<mutex>
#include<random>

namespace DAMCREM{
namespace util{

inline std::mt19937_64 create_random_generator(){
  static std::random_device seed_gen;
  static std::mutex mutex;
  std::lock_guard<std::mutex> lg{mutex};
  std::mt19937_64 engine(seed_gen());
  return engine;
}


}  // namespace util
}  // namespace DAMCREM

