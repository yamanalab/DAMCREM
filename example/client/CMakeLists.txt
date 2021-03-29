set(TARGET_CLIENT_PROGRAMS
  test_online)

foreach(target IN LISTS TARGET_CLIENT_PROGRAMS)
  add_executable("${target}" "${target}.cpp")
  my_set_property_for_binary("${target}" "stdsc;damcrem_alloc_dummy;damcrem_sched_dummy;damcrem_core")
endforeach()
