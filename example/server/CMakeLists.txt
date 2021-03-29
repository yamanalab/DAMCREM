set(TARGET_SERVER_PROGRAMS
  offline_evaluation simulation_evaluation test_online)

foreach(tam IN LISTS TARGET_ALLOCATE_METHODS)
  foreach(tsm IN LISTS TARGET_SCHEDULE_METHODS)
    foreach(target_source IN LISTS TARGET_SERVER_PROGRAMS)
      set(target "${target_source}_${tam}_${tsm}")
      add_executable("${target}" "${target_source}.cpp")
      my_set_property_for_binary("${target}" "stdsc;damcrem_alloc_${tam};damcrem_sched_${tsm};damcrem_core")
    endforeach()
  endforeach()
endforeach()

add_executable("test_server" "test_server.cpp")
my_set_property_for_binary("test_server" "stdsc;damcrem_alloc_dummy;damcrem_sched_dummy;damcrem_core")
