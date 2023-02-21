/**
 * Definitions of number_series and number_series_wrap classes.
 */

#ifndef INCLUDE_NUMBER_SERIES_H
#define INCLUDE_NUMBER_SERIES_H

#include <vector>
#include <limits>
#include <memory>
#include <iostream>

namespace data_series
{
    class number_series
    {
    public:
        using value_type = int;  ///< type alias for easy customization (one may change int to double etc)
        explicit number_series(std::vector<value_type> values): values(std::move(values)) { update_min_max(); }
        /// Support construction from list of values {1,2,3}:
        number_series(const std::initializer_list<value_type>& values): values{values} { update_min_max(); }
        /// Recover the default ctor without args since we lost it by declaring custom ctors:
        number_series() = default;
        number_series& add_value(int v)
        {
            values.push_back(v);
            update_min_max(v);
            return *this;
        }
        /** Returns the length of the number series.
         * `const` promises not to change the state of *this.
         * `const` methods are allowed to be called even when the object is `const`.
         */
        size_t size() const { return values.size(); }
        value_type get_min() const { return min_value; }
        value_type get_max() const { return max_value; }
        value_type amplitude() const { return max_value - min_value; }

        /// Returns a pointer to the raw memory storage, just for bm_make_random.cpp.
        const value_type* data() const { return values.data(); }

        /// Implements element-wise addition.
        number_series& operator+=(const number_series& rhs);
        /// Generates a random number series of length `count`:
        static number_series make_random(size_t count);
        /// Alternative implementation, see bm_make_random.cpp for performance:
        static number_series make_random2(size_t count);
        /// Alternative implementation using algorithm:
        static number_series make_random3(size_t count);
        /// Alternative implementation using algorithm:
        static number_series make_random4(size_t count);
        /// Alternative implementation using ranges:
        static number_series make_random5(size_t count);

    private:
        std::vector<value_type> values;
        value_type min_value = std::numeric_limits<value_type>::max();
        value_type max_value = std::numeric_limits<value_type>::min();
        /// maintain min and max internally (inline)
        void update_min_max(value_type v)
        {
            min_value = std::min(min_value, v);
            max_value = std::max(max_value, v);
        }
        void update_min_max();  ///< recompute min/max from scratch
        // int padding[100]{};     // experiment with sort performance
    };
    /// Compares two series by their amplitude:
    inline bool operator<(const number_series& lhs, const number_series& rhs)
    {
        return lhs.amplitude() < rhs.amplitude();
    }
    /// Implements element-wise addition:
    inline number_series operator+(const number_series& lhs, const number_series& rhs)
    {
        auto res = lhs;
        res += rhs;  // DRY: reuse += implementation
        return res;
    }
}  // namespace data_series

#endif  // INCLUDE_NUMBER_SERIES_H
