cmake_minimum_required(VERSION 3.15)
project(Extended1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

include(sanitizers.cmake)
include(doctest.cmake)
include(benchmark.cmake)

enable_testing()

add_executable(main main.cpp number_series.cpp number_series.h)

add_executable(test_number_series test_number_series.cpp number_series.cpp number_series.h)
target_link_libraries(test_number_series PRIVATE doctest::doctest)
add_test(NAME test_number_series COMMAND test_number_series)

add_executable(bm_sort bm_sort.cpp)
target_link_libraries(bm_sort PRIVATE benchmark::benchmark)
add_test(NAME bm_sort COMMAND bm_sort)

add_executable(bm_number_series bm_number_series.cpp number_series.cpp number_series.h)
target_link_libraries(bm_number_series PRIVATE benchmark::benchmark)
add_test(NAME bm_number_series COMMAND bm_number_series)