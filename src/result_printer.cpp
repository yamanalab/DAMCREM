#include"result_printer.h"

namespace DAMCREM{

void ResultPrinter::show(const std::vector<std::shared_ptr<Job>> &src, std::ostream &stream,
                         bool do_show_result_ciphertext, bool do_convert_seconds,
                         bool do_show_macrotask_exec_log){
  show_header_of_result_of_job(stream, do_show_result_ciphertext, do_convert_seconds, do_show_macrotask_exec_log);
  for( const std::shared_ptr<Job> &job : src ){
    show_result_of_job(stream, *job, do_show_result_ciphertext, do_convert_seconds, do_show_macrotask_exec_log);
  }
  return;
}

std::vector<std::shared_ptr<Job>> ResultPrinter::get_sorted_all(const ResultSender &src){
  std::vector<std::shared_ptr<Job>> result;
  result = get_sorted_queue(src);
  auto r = get_sorted_results(src);
  std::move(r.begin(), r.end(), std::back_inserter(result));
  return result;
}

std::vector<std::shared_ptr<Job>> ResultPrinter::get_sorted_queue(const ResultSender &src){
  std::vector<std::shared_ptr<Job>> queue_list = flatten(src.queue());
  std::sort(queue_list.begin(), queue_list.end(), [](const auto &a, const auto &b){ return a->job_id() < b->job_id(); });
  return queue_list;
}

std::vector<std::shared_ptr<Job>> ResultPrinter::get_sorted_results(const ResultSender &src){
  std::vector<std::shared_ptr<Job>> results_list = flatten(src.results());
  std::sort(results_list.begin(), results_list.end(), [](const auto &a, const auto &b){ return a->job_id() < b->job_id(); });
  return results_list;
}

std::vector<std::shared_ptr<Job>> ResultPrinter::flatten(const std::deque<std::shared_ptr<Job>> &src) const {
  std::vector<std::shared_ptr<Job>> vec;
  util::transform(src, std::back_inserter(vec), [&](const auto &x){ return x; });
  return vec;
}

std::vector<std::shared_ptr<Job>> ResultPrinter::flatten(const NestedUnorderedMap<AppID, ClientID, QueryID, std::shared_ptr<Job>> &src) const {
  std::vector<std::shared_ptr<Job>> vec;
  util::for_each(src, [&](const auto &ai_x){
    util::for_each(ai_x.second, [&](const auto &ci_y){
      util::transform(ci_y.second, std::back_inserter(vec), [](const auto &qi_z){ return qi_z.second; });
    });
  });
  return vec;
}

void ResultPrinter::show_header_of_result_of_job(std::ostream &stream, bool do_show_result_ciphertext,
                                                 bool do_convert_seconds, bool do_show_macrotask_exec_log){
  const std::string unit_of_time = (do_convert_seconds ? "[s]" : "[ms]");
  stream << util::join_str({
      util::join_str({"JobID", "AppID", "ClientID", "QueryID"}, ", ").str(),
      "finish-receive (received -> finish)" + unit_of_time,
      "finish-start (start -> finish)" + unit_of_time
    }, ": ").str() << std::endl;
  return;
}

void ResultPrinter::show_result_of_job(std::ostream &stream, const Job &job, bool do_show_result_ciphertext,
                                       bool do_convert_seconds, bool do_show_macrotask_exec_log){
  const std::vector<std::reference_wrapper<Ciphertext>> &results_cts = job.get_results();
  std::vector<double> result;
  Plaintext pt;
  const Scheme &scheme = scheme_list_.scheme(job.app_id(), job.query().client_id());
  scheme.decrypt(pt, results_cts.at(0));
  scheme.decode(result, pt);
  stream << job.job_id() << ", " << job.query().app_id() << ", "
         << job.query().client_id() << ", " << job.query().query_id();
    
  if( do_convert_seconds ){
    stream << ": " << std::fixed << std::setprecision(6) << (job.query().finish_time()-job.query().received_time())*0.001*0.001
           << " (" << job.query().received_time()*0.001*0.001 << " -> " << job.query().finish_time()*0.001*0.001 << "): "
           << (job.query().finish_time()-job.query().start_time())*0.001*0.001
           << " (" << job.query().start_time()*0.001*0.001 << " -> " << job.query().finish_time()*0.001*0.001 << "): ";
  }else{
    stream << ": " << (job.query().finish_time()-job.query().received_time())
           << " (" << job.query().received_time() << " -> " << job.query().finish_time() << "): "
           << (job.query().finish_time()-job.query().start_time())
           << " (" << job.query().start_time() << " -> " << job.query().finish_time() << ")";
  }
  if( do_show_result_ciphertext ){
    stream << ": " << std::fixed << std::setprecision(6) << result.at(0);
  }
  stream << std::endl;
  
  if( do_show_macrotask_exec_log ){
    const auto &met = job.macrotask_exec_log();
    for( size_t i = 1, n = job.app().macrotask_graph().num_macrotasks(); i <= n; ++i ){
      stream << "  " << i << ": " << (met.at(i).second - met.at(i).first)
             << " (" << met.at(i).first << " -> " << met.at(i).second << ")" << std::endl;
    }
  }
  return;
}


}  // namespace DAMCREM

