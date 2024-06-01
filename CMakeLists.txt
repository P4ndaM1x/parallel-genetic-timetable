cmake_minimum_required(VERSION 3.13)
project(ParallelGeneticTimetable)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/ext)

add_custom_target(clear
    COMMAND ${CMAKE_COMMAND} -E rm -r ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}
)

add_custom_target(test
    COMMAND mpiexec -n 2 ./main --config ${PROJECT_SOURCE_DIR}/test-config.ini
)

find_package(UPCXX REQUIRED)
set(CMAKE_CXX_COMPILER upcxx)
# include_directories(/opt/nfs/mpich-3.2/include)
# include_directories(/opt/nfs/mpe2-2.4.9b/include)
# link_directories(/opt/nfs/mpich-3.2/lib)
# link_directories(/opt/nfs/mpe2-2.4.9b/lib)
# link_directories(/opt/nfs/mpich-3.2/bin/lib)
# list( APPEND CMAKE_INSTALL_RPATH /opt/nfs/mpich-3.2/lib )
# add_link_options("-Wl,-rpath-link,/opt/nfs/mpich-3.2/lib" "-Wl,--enable-new-dtags")

set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
add_compile_options(${CMAKE_CXX_FLAGS})

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
