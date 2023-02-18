/// Created by Marius Mikucionis <marius@cs.aau.dk>
/**
 * Definitions/implementation of some number_series methods
 */

#include "number_series.hpp"

#include <algorithm>
#include <random>

namespace data_series
{
    /// Compute min and max from scratch:
    void number_series::update_min_max()
    {
        if (size() > 0) {  // protect against empty series
            auto [min_it, max_it] = std::minmax_element(values.begin(), values.end());
            min_value = *min_it;
            max_value = *max_it;
        }
    }

    /// Implements element-wise addition
    number_series& number_series::operator+=(const number_series& rhs)
    {
        // Two containers with different sizes is a recipe for out-of-range disaster.
        // Principle: keep it simple, minimize code branching ("if (condition)").
        // Code branching demands mental effort and is hard to test (number of cases is doubled).
        // Idea: eventually values will contain `max(size(), rhs.size())` elements regardless,
        // so let's take care of the size right away:
        if (values.size() < rhs.size())    // not enough space to accommodate rhs
            values.resize(rhs.size(), 0);  // fill the extra values with zeros.
        // Add rhs values to the corresponding values, rhs may have fewer elements, but that's OK:
        for (auto i = 0; i < rhs.size(); ++i)  // idiomatic counting loop, uses condition -- code branching!
            values[i] += rhs.values[i];
        // Advanced way using algorithm (C++98), no branching:
        // std::transform(rhs.values.begin(), rhs.values.end(), values.begin(), values.begin(), std::plus{});
        // Advanced way using ranges (C++20), no branching, simpler:
        // std::ranges::transform(rhs.values, values, values.begin(), std::plus{});
        return *this;
    }

    /**
     * Generates one random integer value.
     * `static` function means that the function is available only to this translation unit (opposite of `export`).
     * `inline` means it's a candidate for inlining (embedding code instead of function call).
     * @return
     */
    static inline int random_int()
    {
        // Pseudo random number generator initialized with a value from true random device.
        // Here `static` means allocate statically (rather than on stack),
        // which means it's created just once for entire program lifetime
        // (preserves calls to true random device which may exhaust entropy and become slow).
        static auto gen = std::default_random_engine{std::random_device{}()};
        static auto dist =
            std::uniform_int_distribution{std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};
        return dist(gen);  // generate a random value based on the distribution
    }

    /// Straight-forward implementation using C-style counting loop
    number_series number_series::make_random(size_t count)
    {
        auto values = std::vector<value_type>{};
        values.reserve(count);               // pre-allocate memory, but do not add any values yet
        for (size_t i = 0; i < count; ++i)   // idiomatic counting loop, uses condition -- room for errors
            values.push_back(random_int());  // add the value to the vector
        return number_series{values};
    }

    /// Straight-forward implementation using ranged loop (C++11)
    number_series number_series::make_random2(size_t count)
    {
        auto values = std::vector<value_type>{};
        values.resize(count, 0);  // pre-allocate and populate with zeros, slight overhead
        // Note that such memory reset operations are very fast, thus zeroing overhead is negligible.
        for (auto& v : values)  // ranged loop: less room for errors, but note the reference!
            v = random_int();   // assign each member
        return number_series{values};
    }

    /// Advanced implementation using algorithm (C++98)
    number_series number_series::make_random3(size_t count)
    {
        auto values = std::vector<value_type>{};
        values.reserve(count);  // pre-allocate, but do not add any values yet (no overhead)
        // Use algorithm: specify count, no branching, a bit tricky:
        std::generate_n(std::back_inserter(values), count, random_int);
        // std::back_inserter calls push_back on the container with values returned from function.
        return number_series{values};
    }

    /// Advanced implementation using algorithm (C++98)
    number_series number_series::make_random4(size_t count)
    {
        auto values = std::vector<value_type>{};
        values.resize(count, 0);  // allocate and populate with zeros, slight overhead
        // Use algorithm: specify range, no branching,
        // can be tricky negotiations with a compiler, but eventually type-checked and no room for errors:
        std::generate(values.begin(), values.end(), random_int);
        return number_series{values};
    }

    /// Advanced implementation using ranges (C++20)
    number_series number_series::make_random5(size_t count)
    {
        auto values = std::vector<value_type>{};
        values.resize(count, 0);  // allocate and populate with zeros, slight overhead
        // Use ranges: specify container and function -- easy and no room for errors:
        std::ranges::generate(values, random_int);  // may be hard to read documentation...
        return number_series{values};
    }

}  // namespace data_series
