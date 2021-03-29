add_executable("gen_data" gen_data.cpp)
my_set_property_for_binary("gen_data" "stdsc;damcrem_alloc_dummy;damcrem_sched_dummy;damcrem_core")

# add_executable("static_analysis" static_analysis.cpp)
# my_set_property_for_binary("static_analysis" "stdsc;damcrem_sched_fifo;damcrem_core")

