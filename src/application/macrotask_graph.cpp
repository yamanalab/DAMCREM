#include"macrotask_graph.h"

#include<regex>

#include"util.h"

namespace DAMCREM{
  MacrotaskGraph::MacrotaskGraph(size_t num_macrotasks, const std::unordered_map<std::string, std::string> &app_data)
    : num_macrotasks_(num_macrotasks){
    std::smatch m;
    std::regex r("(|\\s*\\d+\\s*->\\s*\\d+\\s*(,\\s*\\d+\\s*->\\s*\\d+\\s*)*)");
    if( !std::regex_match(app_data.at("macrotask_group="), m, r) ){
      throw std::runtime_error("app data of macrotask_group is invalid.");
    }

    macrotask_.reserve(num_macrotasks_+1);
    util::transform(util::sequence<size_t>(0, num_macrotasks_+1), std::back_inserter(macrotask_), [](size_t i){ return i; });

    for( const auto &x : util::split_str(app_data.at("macrotask_group="), ",", true, true) ){
      const auto y = util::split_str<size_t>(x, "->");
      macrotask_.at(y.at(0)).set_macrotask_group(y.at(1));
    }

    for( size_t src = 0; src <= num_macrotasks_; ++src ){
      std::string key = "dependency[" + std::to_string(src) + "]=";
      if( app_data.at(key).size() == 0 ){
        final_macrotask_id_.emplace(src);
      }else{
        util::for_each(util::split_str<size_t>(app_data.at(key), ",", true, true),
                       [&](size_t dst){
                         macrotask_.at(src).add_next(dst);
                         macrotask_.at(dst).add_previous(src);
                       });
      }
    }
    
    for( size_t macrotask_id = 1; macrotask_id <= num_macrotasks_; ++macrotask_id ){
      util::for_each(util::split_str(app_data.at("op["+std::to_string(macrotask_id)+"]="), ";", true, true),
                     [&](const std::string &x){ macrotask_.at(macrotask_id).add_operation(util::split_str(x, ",", true, true)); });
    }
    
    return;
  }

}

