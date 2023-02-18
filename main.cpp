/// Created by Marius Mikucionis <marius@cs.aau.dk>
/**
 * Purpose: compare the performance of number_series and number_series_wrap.
 */
#include "number_series.hpp"
#include "number_series_wrap.hpp"

#include <chrono>
#include <algorithm>

constexpr auto ns_number = 100'000;
constexpr auto ns_length = 100;

int main()
{
    using namespace data_series;
    using clk = std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::milli;
    using std::cout;
    using std::endl;

    // Part 2
    std::vector<number_series> vv;
    vv.reserve(ns_number);  // preallocate memory

    // Populate the vv with data here
    for (size_t i = 0; i < ns_number; ++i)
        vv.push_back(number_series::make_random(ns_length));
    for (auto& ns : vv)
        ns += number_series::make_random(ns_length);

    auto t0 = clk::now();
    std::sort(vv.begin(), vv.end());
    auto t1 = clk::now();
    cout << "Sorting values: " << duration<double, milli>(t1 - t0).count() << " ms\n";

    // Part 3
    // Note that this is the exact same code as for Part 1 except using number_series_wrap
    auto vw = std::vector<number_series_wrap>{};
    vw.reserve(ns_number);  // preallocate memory

    // Populate the vw with data here
    for (size_t i = 0; i < ns_number; ++i)
        vw.push_back(number_series_wrap::make_random(ns_length));
    for (auto& ns : vw)
        ns += number_series_wrap::make_random(ns_length);

    t0 = clk::now();
    std::sort(vw.begin(), vw.end());
    t1 = clk::now();
    cout << "Sorting wrapped pointers: " << duration<double, milli>(t1 - t0).count() << " ms\n";

    cout << "sizeof(number_series): " << sizeof(number_series) << " bytes\n";
}

/**
Important: measure the optimized ("Release") build!

 My expectation: some slowdown in number_series_wrap due to extra pointer lookup,
 but difficult to guess how much in relation to copying operations in sorting.

Sample result:

Sorting values: 6.43198 ms
Sorting wrapped pointers: 23.9614 ms
sizeof(number_series): 32 bytes

Interpretation:

Part 3 is about 272% slower than Part 2.
Indirection introduces significant overhead.

Sample result, if number_series is padded with array of 72 ints:

Sorting values: 34.9439 ms
Sorting wrapped pointers: 27.854 ms
sizeof(number_series): 320 bytes

Part 3 is about 20% faster than Part 2.
Padded number_series has a larger memory footprint and thus swap operations become dominant when sorting,
slower than simple pointer swap.

See bm_number_series for more precise measurements.

*/
