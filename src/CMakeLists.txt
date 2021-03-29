# Create a library for common parts of DAMCREM. ################################
# file(GLOB sources *.cpp application/*.cpp communication/*.cpp communication/*/*.cpp)
add_library(damcrem_core OBJECT
  dir_path_config.cpp executor.cpp job.cpp macrotask_queue.cpp
  option.cpp query.cpp query_receiver.cpp result_printer.cpp result_sender.cpp scheme_list.cpp
  application/macrotask_exec_time_info.cpp application/macrotask_exec_time_info_manager.cpp application/macrotask_graph.cpp
  communication/client/client.cpp
  communication/server/callback_function.cpp communication/server/server.cpp communication/server/state.cpp
  communication/server/offline_server.cpp)
my_set_property_for_library(damcrem_core)

# add_library(damcrem_core_online OBJECT
#   dir_path_config.cpp executor.cpp job.cpp macrotask_queue.cpp
#   option.cpp query.cpp query_receiver.cpp result_printer.cpp result_sender.cpp scheme_list.cpp
#   application/macrotask_exec_time_info.cpp application/macrotask_exec_time_info_manager.cpp application/macrotask_graph.cpp
#   communication/client/client.cpp
#   communication/server/callback_function.cpp communication/server/server.cpp communication/server/state.cpp
#   communication/server/server_type_server.cpp)
# my_set_property_for_library(damcrem_core_online)

# add_library(damcrem_core_offline OBJECT
#   dir_path_config.cpp executor.cpp job.cpp macrotask_queue.cpp
#   option.cpp query.cpp query_receiver.cpp result_printer.cpp result_sender.cpp scheme_list.cpp
#   application/macrotask_exec_time_info.cpp application/macrotask_exec_time_info_manager.cpp application/macrotask_graph.cpp
#   communication/client/client.cpp
#   communication/server/callback_function.cpp communication/server/server.cpp communication/server/state.cpp
#   communication/server/offline_server.cpp communication/server/server_type_offline_server.cpp)
# my_set_property_for_library(damcrem_core_offline)
################################################################################

# Create libraries for allocation method #######################################
add_library("damcrem_alloc_dummy" OBJECT allocate_method/dummy.cpp)
my_set_property_for_library("damcrem_alloc_dummy")
foreach(tam IN LISTS TARGET_ALLOCATE_METHODS)
  add_library("damcrem_alloc_${tam}" OBJECT allocate_method/${tam}.cpp)
  my_set_property_for_library("damcrem_alloc_${tam}")
endforeach()
################################################################################

# Create libraries for scheduling method #######################################
add_library("damcrem_sched_dummy" OBJECT schedule_method/dummy.cpp)
my_set_property_for_library("damcrem_sched_dummy")
foreach(tsm IN LISTS TARGET_SCHEDULE_METHODS)
  add_library("damcrem_sched_${tsm}" OBJECT schedule_method/${tsm}.cpp)
  my_set_property_for_library("damcrem_sched_${tsm}")
endforeach()
################################################################################

