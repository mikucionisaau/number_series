cmake_minimum_required(VERSION 3.15)
project(Extended1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF) # Stick with statically linked libraries

include(sanitizers.cmake)
include(doctest.cmake)
include(benchmark.cmake)

enable_testing()

add_executable(main main.cpp number_series.cpp number_series.hpp)
add_test(NAME main COMMAND main)

add_executable(number_series_test number_series_test.cpp number_series.cpp number_series.hpp)
target_link_libraries(number_series_test PRIVATE doctest::doctest_with_main)
add_test(NAME number_series_test COMMAND number_series_test)

add_executable(number_series_test_wrap number_series_wrap_test.cpp number_series.cpp)
target_link_libraries(number_series_test_wrap PRIVATE doctest::doctest)
add_test(NAME number_series_test_wrap COMMAND number_series_test_wrap)

add_executable(number_series_test_wrap2 number_series_wrap2_test.cpp number_series.cpp)
target_link_libraries(number_series_test_wrap2 PRIVATE doctest::doctest)
add_test(NAME number_series_test_wrap2 COMMAND number_series_test_wrap2)

add_executable(number_series_test_wrap3 number_series_wrap3_test.cpp number_series.cpp)
target_link_libraries(number_series_test_wrap3 PRIVATE doctest::doctest)
add_test(NAME number_series_test_wrap3 COMMAND number_series_test_wrap3)

add_executable(sort_bm sort_bm.cpp)
target_link_libraries(sort_bm PRIVATE benchmark::benchmark_main)
add_test(NAME sort_bm COMMAND sort_bm)

add_executable(number_series_bm number_series_bm.cpp number_series.cpp number_series.hpp)
target_link_libraries(number_series_bm PRIVATE benchmark::benchmark_main)
add_test(NAME number_series_bm COMMAND number_series_bm --benchmark_time_unit=ms --benchmark_min_time=4)

add_executable(make_random_bm make_random_bm.cpp number_series.cpp number_series.hpp)
target_link_libraries(make_random_bm PRIVATE benchmark::benchmark)
add_test(NAME make_random_bm COMMAND make_random_bm)

# Benchmark comparisons:
add_test(NAME ns_vs_wrap_bm COMMAND ${benchmark_cmp} filters $<TARGET_FILE:number_series_bm> bm_ns_sort bm_ns_wrap_sort --benchmark_time_unit=ms --benchmark_min_time=4)
add_test(NAME ns_vs_wrap2_bm COMMAND ${benchmark_cmp} filters $<TARGET_FILE:number_series_bm> bm_ns_sort bm_ns_wrap2_sort --benchmark_time_unit=ms --benchmark_min_time=4)
add_test(NAME ns_vs_wrap3_bm COMMAND ${benchmark_cmp} filters $<TARGET_FILE:number_series_bm> bm_ns_sort bm_ns_wrap3_sort --benchmark_time_unit=ms --benchmark_min_time=4)

add_test(NAME make_random_1vs2 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:make_random_bm> make_random1_bm make_random2_bm)
add_test(NAME make_random_1vs3 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:make_random_bm> make_random1_bm make_random3_bm)
add_test(NAME make_random_1vs4 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:make_random_bm> make_random1_bm make_random4_bm)
add_test(NAME make_random_1vs5 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:make_random_bm> make_random1_bm make_random5_bm)

add_test(NAME make_random_2vs3 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:make_random_bm> make_random2_bm make_random3_bm)
add_test(NAME make_random_2vs4 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:make_random_bm> make_random2_bm make_random4_bm)
add_test(NAME make_random_2vs5 COMMAND ${benchmark_cmp} filters $<TARGET_FILE:make_random_bm> make_random2_bm make_random5_bm)

# Run benchmarks sequentially so that they do not interfere with others
set_tests_properties(
        main
        sort_bm
        number_series_bm
        ns_vs_wrap_bm ns_vs_wrap2_bm ns_vs_wrap3_bm
        make_random_bm
        make_random_1vs2 make_random_1vs3 make_random_1vs4 make_random_1vs5
        make_random_2vs3 make_random_2vs4 make_random_2vs5
        PROPERTIES RUN_SERIAL TRUE)
