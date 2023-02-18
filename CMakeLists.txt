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

add_executable(bm_make_random bm_make_random.cpp number_series.cpp number_series.h)
target_link_libraries(bm_make_random PRIVATE benchmark::benchmark)
add_test(NAME bm_make_random COMMAND bm_make_random)

# Benchmark comparisons:
add_test(NAME bm_ns_vs_wrap COMMAND ${benchmark_cmp} filters $<TARGET_FILE:bm_number_series> bm_ns_sort bm_ns_wrap_sort)

add_test(NAME bm_make_random_1_2 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:bm_make_random> bm_make_random1 bm_make_random2)
add_test(NAME bm_make_random_1_3 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:bm_make_random> bm_make_random1 bm_make_random3)
add_test(NAME bm_make_random_1_4 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:bm_make_random> bm_make_random1 bm_make_random4)
add_test(NAME bm_make_random_1_5 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:bm_make_random> bm_make_random1 bm_make_random5)

add_test(NAME bm_make_random_2_3 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:bm_make_random> bm_make_random2 bm_make_random3)
add_test(NAME bm_make_random_2_4 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:bm_make_random> bm_make_random2 bm_make_random4)
add_test(NAME bm_make_random_2_5 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:bm_make_random> bm_make_random2 bm_make_random5)

# Run benchmarks sequentially so that they do not interfere with others
set_tests_properties(
        bm_sort
        bm_number_series
        bm_ns_vs_wrap
        bm_make_random
        bm_make_random_1_2 bm_make_random_1_3 bm_make_random_1_4 bm_make_random_1_5
        bm_make_random_2_3 bm_make_random_2_4 bm_make_random_2_5
        PROPERTIES RUN_SERIAL TRUE)
