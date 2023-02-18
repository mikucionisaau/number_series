include(FetchContent)
set(FETCHCONTENT_QUIET ON)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE) # suppress internal tests
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0)      # or "main" for latest
FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.7.1)              # or "master" for latest
FetchContent_MakeAvailable(googletest googlebenchmark)

message(STATUS "!!! Benchmark comparison require python3 with `pip install scipy` !!!")
set(benchmark_cmp "${googlebenchmark_SOURCE_DIR}/tools/compare.py")
