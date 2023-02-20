/**
 * Definition of number_series_wrap2 class.
 */

#ifndef INCLUDE_NUMBER_SERIES_WRAP2_H
#define INCLUDE_NUMBER_SERIES_WRAP2_H

#include "number_series.h"

namespace data_series
{
    /**
     * Minimal wrapper implementation with default move operations.
     */
    class number_series_wrap2
    {
        using value_type = number_series::value_type;
        std::unique_ptr<number_series> ptr = std::make_unique<number_series>();  // can become null if moved out

    public:
        number_series_wrap2() = default;  // ptr=std::make_unique...
        number_series_wrap2(const std::initializer_list<value_type>& values):
            ptr{std::make_unique<number_series>(values)}  // default initializer is skipped
        {}
        explicit number_series_wrap2(number_series ns): ptr{std::make_unique<number_series>(std::move(ns))} {}
        number_series_wrap2(const number_series_wrap2& other) { *this = other; }  // ptr=std::make_unique...
        number_series_wrap2& operator=(const number_series_wrap2& other)
        {
            *ptr = *other.ptr;  // deep copy assignment
            return *this;
        }
        // Recover default move operations (below), unfortunately they crash on size() after std::move:
        number_series_wrap2(number_series_wrap2&& other) noexcept = default;
        number_series_wrap2& operator=(number_series_wrap2&& other) noexcept = default;
        ~number_series_wrap2() noexcept = default;
        number_series_wrap2& add_value(value_type v)
        {
            ptr->add_value(v);
            return *this;
        }
        value_type get_min() const { return ptr->get_min(); }
        value_type get_max() const { return ptr->get_max(); }
        value_type amplitude() const { return get_max() - get_min(); }
        size_t size() const { return ptr->size(); }
        number_series_wrap2& operator+=(const number_series_wrap2& rhs)
        {
            *ptr += *rhs.ptr;  // DRY: reuse += over number_series
            return *this;
        }
        static number_series_wrap2 make_random(size_t count)
        {
            return number_series_wrap2{number_series::make_random(count)};
        }
    };
    inline number_series_wrap2 operator+(const number_series_wrap2& lhs, const number_series_wrap2& rhs)
    {
        auto res = lhs;
        res += rhs;  // DRY: reuse += implementation
        return res;
    }
    inline bool operator<(const number_series_wrap2& lhs, const number_series_wrap2& rhs)
    {
        return lhs.amplitude() < rhs.amplitude();
    }
}  // namespace data_series

#endif  // INCLUDE_NUMBER_SERIES_WRAP2_H
