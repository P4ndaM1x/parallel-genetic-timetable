cmake_minimum_required(VERSION 3.13)
project(ParallelGeneticTimetable)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/ext)

add_custom_target(clear
    COMMAND ${CMAKE_COMMAND} -E rm -r ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}
)

option(UPCXX_ENABLE "Use UPC instead of MPI" ON)
if(UPCXX_ENABLE)

  add_custom_target(test
      COMMAND upcxx-run -n 8 $$\(upcxx-nodes ../nodes\) ./main --config ${PROJECT_SOURCE_DIR}/test-config.ini
  )

  add_compile_definitions(UPCXX)
  find_package(UPCXX REQUIRED)
  set(CMAKE_CXX_COMPILER upcxx)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
  set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
  
else()

  add_custom_target(test
      COMMAND mpiexec -n 8 ./main --config ${PROJECT_SOURCE_DIR}/test-config.ini
  )

  set(CMAKE_CXX_COMPILER "/home/nfs/mpich-4.2.1/bin/mpicxx")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
  set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")

endif(UPCXX_ENABLE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES} main.cpp)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)

include_directories("/home/nfs/._off_mpich-4.0.1/include")
target_link_libraries(main PUBLIC m ext)
