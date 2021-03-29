#include<filesystem>

#include"executor.h"

namespace DAMCREM{
namespace static_analysis{
size_t NUM_THREADS = 1;
}

// 割当計算機資源量決定アルゴリズム
void Executor::calculate_allocated_computational_resource(){
  size_t l = mq_.waiting_queue().size();
  ComputationalResource tmp_cr = current_computational_resource_;
  for( size_t i = 0; i < l; ++i ){
    auto x = mq_.waiting_queue().top();
    ComputationalResource cr(DAMCREM::static_analysis::NUM_THREADS);
    tmp_cr += cr;
    if( !(option_.max_computational_resource().is_valid(tmp_cr)) ){
      break;
    }
    mq_.exec_next_queue_emplace(std::move(x), cr);
    mq_.waiting_queue_pop_front();
  }
  return;
}
}

std::unordered_map<std::string, std::string> args_keywords = {
  {"--log-file=", "Measurement log file path"},
  {"--data-file=", "Generated execution time data file path"},
  {"--measure", "Execute some jobs and create execution result file"},
  {"--generate", "Generate execution time data file from the log file"},
  {"--option-file=", "Option file path"},
  {"--max-num-threads=", "The maximum number of threads to be used"},
  {"--ignore-top-jobs=", "The number of ignored jobs from the earliest jobs"},
  {"--ignore-end-jobs=", "The number of ignored jobs from the latest jobs"},
  {"--force-override", "The log file and execution time data file will be override. Default is false."},
};

std::unordered_map<std::string, std::string> default_args = {
  {"--measure", "false"},
  {"--generate", "false"},
  {"--ignore-top-jobs=", "0"},
  {"--ignore-end-jobs=", "0"},
  {"--force-override", "false"},
};

void measure_once(const std::unordered_map<std::string, std::string> &args, std::ostream &stream){
  DAMCREM::Option option(args.at("--option-file="));
  option.max_num_threads(std::stoul(args.at("--max-num-threads=")));
  option.finalize();
  
  DAMCREM::Executor executor(std::move(option));

  executor.run();

  executor.show_results(stream, false, true, true);
  
  return;
}

std::tuple<std::vector<std::vector<std::vector<size_t>>>, std::vector<size_t>>
load_log_data(const std::unordered_map<std::string, std::string> &args){
  std::vector<std::vector<std::vector<size_t>>> log_data;
  std::vector<size_t> app_id_list;

  // 全てのデータを読み込む．フィルタリングはしない．
  std::vector<std::string> raw_data = DAMCREM::util::load_text_from_file(args.at("--log-file="));

  size_t num_macrotasks = 0;
  size_t num_threads = 0;
  size_t job_id = 0;

  for( const auto &x : raw_data ){
    if( x.substr(0, 2) == "  " ){
      if( num_macrotasks >= std::stoul(DAMCREM::util::split_str(x, ":").at(0)) ){ continue; }
      num_macrotasks = std::stoul(DAMCREM::util::split_str(x, ":").at(0));
    }
  }

  // add data for allocating 0 threads
  log_data.resize(num_macrotasks+1, std::vector<std::vector<size_t>>(1));
  
  for( const auto &x : raw_data ){
    if( x.size() == 0 ){
      continue;
    }else if( x.substr(0, 12) == "NUM_THREADS=" ){
      num_threads = std::stoul(DAMCREM::util::split_str(x, "=").at(1));
      for( auto &y : log_data ){ y.emplace_back(); }
    }else if( x.substr(0, 2) == "  " ){
      std::vector<std::string> ss = DAMCREM::util::split_str(x, ":");
      log_data.at(std::stoul(ss.at(0))).at(num_threads).at(job_id) = std::stoul(DAMCREM::util::split_str(ss.at(1), "(").at(0));
    }else{
      std::vector<std::string> ss = DAMCREM::util::split_str(DAMCREM::util::split_str(x, ":").at(0), ",");
      job_id = std::stoul(ss.at(0));
      if( num_threads == 1 ){
        app_id_list.emplace_back(std::stoul(ss.at(1)));
      }
      for( auto &y : log_data ){ y.at(num_threads).emplace_back(); }
    }
  }

  return std::make_tuple(log_data, app_id_list);
}

void measure(const std::unordered_map<std::string, std::string> &args){
  const std::string log_file_path = args.at("--log-file=");
  if( !DAMCREM::util::convert_string<bool>(args.at("--force-override")) && std::filesystem::exists(log_file_path) ){
    throw std::runtime_error(log_file_path+" exists.");
  }
  std::ofstream log_msg_out(log_file_path);
  DAMCREM::util::DebugMessage log_msg(log_msg_out);
    
  size_t max_num_threads = std::stoul(args.at("--max-num-threads="));
  for( size_t i = 1; i <= max_num_threads; ++i ){
    DAMCREM::static_analysis::NUM_THREADS = i;
    log_msg << "NUM_THREADS=" << DAMCREM::static_analysis::NUM_THREADS << std::endl;
    measure_once(args, log_msg_out);
  }
  
  return;
}

void generate(const std::unordered_map<std::string, std::string> &args){
  const std::string data_file_path = args.at("--data-file=");
  if( !DAMCREM::util::convert_string<bool>(args.at("--force-override")) && std::filesystem::exists(data_file_path) ){
    throw std::runtime_error(data_file_path+" exists.");
  }
  std::ofstream ofs(data_file_path);

  DAMCREM::Executor executor(args.at("--option-file="));
  const DAMCREM::ApplicationList &app_list = executor.app_list();

  auto [log_data, app_id_list] = load_log_data(args);

  size_t max_num_threads = std::stoul(args.at("--max-num-threads="));
  size_t min_job_id = std::stoul(args.at("--ignore-top-jobs="));
  size_t max_job_id = app_id_list.size() - std::stoul(args.at("--ignore-end-jobs="));
  
  // app_idは全て同じである前提を置く．
  std::set<size_t> app_id_set;
  for( size_t app_id : app_id_list ){
    app_id_set.emplace(app_id);
  }
  if( app_id_set.size() != 1 ){
    throw std::runtime_error("All app_id of the measured jobs should be the same.");
  }  
  
  // 合計を算出する．total_exec_time.at(app_id).at(macrotask_id).at(num_threads) -> (total execution time, the number of jobs)
  std::unordered_map<size_t, std::vector<std::vector<std::pair<size_t, size_t>>>> total_exec_time;
  for( size_t app_id : app_id_set ){
    total_exec_time.try_emplace(app_id);
    for( size_t macrotask_id = 0; macrotask_id <= app_list.applications(app_id).macrotask_graph().num_macrotasks(); ++macrotask_id ){
      total_exec_time.at(app_id).emplace_back(max_num_threads+1);
    }
  }
  
  for( size_t macrotask_id = 1; macrotask_id < log_data.size(); ++macrotask_id ){
    for( size_t num_threads = 1; num_threads <= max_num_threads; ++num_threads ){
      for( size_t job_id = min_job_id; job_id < max_job_id; ++job_id ){
        auto &tet = total_exec_time.at(app_id_list.at(job_id)).at(macrotask_id).at(num_threads);
        tet.first += log_data.at(macrotask_id).at(num_threads).at(job_id);
        ++(tet.second);
      }
    }
  }

  for( size_t app_id : app_id_set ){
    const DAMCREM::Application &app = app_list.applications(app_id);
    for( size_t macrotask_id = 1; macrotask_id <= app.macrotask_graph().num_macrotasks(); ++macrotask_id ){
      ofs << macrotask_id << "=";
      if( app.macrotask_graph().macrotask(macrotask_id).macrotask_group() == macrotask_id ){
        ofs << "0";
        for( size_t num_threads = 1; num_threads <= max_num_threads; ++num_threads ){
          size_t total = 0;
          size_t count = 0;
          for( size_t i = 1; i <= app.macrotask_graph().num_macrotasks(); ++i ){
            if( app.macrotask_graph().macrotask(i).macrotask_group() != macrotask_id ){ continue; }
            total += total_exec_time.at(app_id).at(i).at(num_threads).first;
            count += total_exec_time.at(app_id).at(i).at(num_threads).second;
          }
          ofs << ", " << total/count;
        }
      }
      ofs << std::endl;
    }
    
    ofs << std::endl << std::endl;
  }
    
  
  return;
}

int main(int argc, char *argv[]){
  std::unordered_map<std::string, std::string> args = DAMCREM::util::parse_arguments(argc, argv, args_keywords, default_args);

  if( DAMCREM::util::convert_string<bool>(args.at("--measure")) ){
    measure(args);
  }

  if( DAMCREM::util::convert_string<bool>(args.at("--generate")) ){
    generate(args);
  }
    
  return 0;
}

