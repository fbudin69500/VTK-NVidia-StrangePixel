cmake_minimum_required(VERSION 2.8)
 
PROJECT(renderer)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(main main.cxx)
 
if(VTK_LIBRARIES)
  target_link_libraries(main ${VTK_LIBRARIES})
endif()
