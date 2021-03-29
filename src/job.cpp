#include"job.h"

namespace DAMCREM{
Job::Job(const ApplicationList &app_list, Query &&query)
  : app_(app_list.applications(query.app_id())), query_(std::move(query)), 
    pt_(app_.pt(), query_.pt(), query_.client_id()), ct_(app_.ct(), query_.ct(), query_.client_id()){

  mutex_input_dependency_ = std::make_unique<std::mutex[]>(app_.macrotask_graph().num_macrotasks()+1);
  macrotask_exec_log_.resize(app_.macrotask_graph().num_macrotasks()+1);

  // 要素を削除するため，参照ではなくコピーする．
  input_dependency_.reserve(app().macrotask_graph().num_macrotasks()+1);
  for( const auto &macrotask : app().macrotask_graph().macrotask() ){
    input_dependency_.emplace_back(macrotask.previous());
  }
  final_macrotask_id_ = app().macrotask_graph().final_macrotask_id();

  set_deadline();
  
  return;
}
  
void Job::set_deadline(){
  if( query_.deadline() == 0 ){
    query_.deadline(query_.received_time() + app().macrotask_exec_time_info_manager().total_exec_time_for_prediction());
  }
  return;
}
  
void Job::execute(MacrotaskID macrotask_id, const ComputationalResource &cr, const Time current_time){
  macrotask_exec_log_.at(macrotask_id).first = current_time;
  for( const auto &op : app().macrotask_graph().macrotask(macrotask_id).op() ){
    scheme().execute(op, pt_, ct_, cr);
  }
  return;
}

std::tuple<bool, std::vector<MacrotaskID>> Job::finish_task(size_t macrotask_id){
  // 依存関係の解決
  std::vector<MacrotaskID> solved_dependency;
  for( MacrotaskID dst : app().macrotask_graph().macrotask(macrotask_id).next() ){
    std::lock_guard<std::mutex> lg(mutex_input_dependency_[dst]);
    input_dependency_.at(dst).erase(macrotask_id);
    if( input_dependency_.at(dst).size() == 0 ){
      solved_dependency.emplace_back(dst);
    }
  }

  // ジョブの実行完了チェック
  bool is_end_of_job = false;
  {
    std::lock_guard<std:: mutex> lg(mutex_final_macrotask_id_);
    final_macrotask_id_.erase(macrotask_id);
    is_end_of_job = (final_macrotask_id_.size()==0);
  }

  return std::forward_as_tuple(is_end_of_job, std::move(solved_dependency));
}

  
}

