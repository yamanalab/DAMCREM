cmake_minimum_required(VERSION 3.13)

project(DAMCREM VERSION 1.0.0 LANGUAGES CXX)

find_package(Threads REQUIRED)

# Build type ###################################################################
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()
################################################################################

# Options for DAMCREM ##########################################################
list(APPEND DAMCREM_OPTIONS DAMCREM_DEBUG_MESSAGE)
################################################################################
message("${DAMCREM_OPTIONS}")

function(my_set_property_of_compile target)
  target_compile_definitions(${target} PRIVATE ${DAMCREM_OPTIONS})
  target_include_directories(${target} BEFORE PRIVATE ${PROJECT_SOURCE_DIR}/stdsc)
  target_include_directories(${target} BEFORE PRIVATE ${PROJECT_SOURCE_DIR}/src)
  target_compile_features(${target} PUBLIC cxx_std_17)
  target_compile_options(${target} BEFORE PUBLIC -Wall -fopenmp -march=native)
endfunction()

function(my_set_property_of_link target linked_libraries)
  target_link_options(${target} PUBLIC -Wall -fopenmp)
  # DAMCREM
  foreach(linked_library IN LISTS linked_libraries)
    message(${linked_library})
    target_link_libraries(${target} PUBLIC ${linked_library})
  endforeach()
  # SEAL
  if( "DO_USE_SEAL_CKKS" IN_LIST DAMCREM_OPTIONS )
    target_link_libraries(${target} PUBLIC SEAL::seal)
  endif()
endfunction()


function(my_set_property_for_library target)
  message("build target (obj): ${target}")
  my_set_property_of_compile("${target}")
endfunction()

function(my_set_property_for_binary target linked_libraries)
  message("build target (bin): ${target}")
  message("linked_libraries: ${linked_libraries}")
  my_set_property_of_compile("${target}")
  my_set_property_of_link("${target}" "${linked_libraries}")
endfunction()

set(TARGET_ALLOCATE_METHODS
  single max_parallel bits2020 minimum)

set(TARGET_SCHEDULE_METHODS
  deadline fifo deadline_fifo fifo_deadline deadline_deadline fifo_fifo)

add_subdirectory(src)
add_subdirectory(tool)
add_subdirectory(stdsc)
add_subdirectory(example)
