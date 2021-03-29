#pragma once

#include"result_sender.h"

namespace DAMCREM{
class ResultPrinter{
public:
  ResultPrinter(const Option &option, const SchemeList &scheme_list) : option_(option), scheme_list_(scheme_list){}
  virtual ~ResultPrinter() = default;
  ResultPrinter(const ResultPrinter &src) = delete;
  ResultPrinter(ResultPrinter &&src) = delete;

  void show(const std::vector<std::shared_ptr<Job>> &src, std::ostream &stream=std::cout,
            bool do_show_result_ciphertext=false, bool do_convert_seconds=false,
            bool do_show_macrotask_exec_log=false);

  std::vector<std::shared_ptr<Job>> get_sorted_all(const ResultSender &src);
  
  std::vector<std::shared_ptr<Job>> get_sorted_queue(const ResultSender &src);
  
  std::vector<std::shared_ptr<Job>> get_sorted_results(const ResultSender &src);
  
private:
  std::vector<std::shared_ptr<Job>> flatten(const std::deque<std::shared_ptr<Job>> &src) const;

  std::vector<std::shared_ptr<Job>> flatten(const NestedUnorderedMap<AppID, ClientID, QueryID, std::shared_ptr<Job>> &src) const;

  void show_header_of_result_of_job(std::ostream &stream, bool do_show_result_ciphertext,
                                    bool do_convert_seconds, bool do_show_macrotask_exec_log);
    
  void show_result_of_job(std::ostream &stream, const Job &job, bool do_show_result_ciphertext,
                          bool do_convert_seconds, bool do_show_macrotask_exec_log);

  const Option &option_;

  const SchemeList &scheme_list_;
  
};


}  // namespace DAMCREM

