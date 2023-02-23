/**
 * Definition of number_series_wrap3 class.
 */

#ifndef INCLUDE_NUMBER_SERIES_WRAP3_H
#define INCLUDE_NUMBER_SERIES_WRAP3_H

#include "number_series.h"

namespace data_series
{
    /**
     * Another alternative implementation of a wrapper allowing ptr to be null.
     * number_series_wrap2 move operation release resource immediately and leave the rhs.ptr==nullptr,
     * which invalidates all other operations like size() (because ptr is null).
     * To accommodate possibility for ptr==nullptr and a potential reuse of such objects,
     * we use defensive programming against ptr==nullptr.
     */
    class number_series_wrap3
    {
        using value_type = number_series::value_type;
        std::unique_ptr<number_series> ptr;  // initialized with nullptr by default(!)

    public:
        number_series_wrap3() = default;  // ptr=nullptr
        number_series_wrap3(const std::initializer_list<value_type>& values):
            ptr{std::make_unique<number_series>(values)}
        {}
        explicit number_series_wrap3(number_series ns): ptr{std::make_unique<number_series>(std::move(ns))} {}
        number_series_wrap3(const number_series_wrap3& other) { *this = other; }  // ptr=nullptr
        number_series_wrap3& operator=(const number_series_wrap3& other)
        {
            if (other.ptr != nullptr) {  // defensive programming
                if (ptr == nullptr)      // defensive programming
                    ptr = std::make_unique<number_series>(*other.ptr);
                else
                    *ptr = *other.ptr;
            } else
                ptr.reset();
            return *this;
        }
        number_series_wrap3(number_series_wrap3&& other) noexcept = default;             // other.ptr=nullptr
        number_series_wrap3& operator=(number_series_wrap3&& other) noexcept = default;  // other.ptr=nullptr
        ~number_series_wrap3() noexcept = default;
        number_series_wrap3& add_value(value_type v)
        {
            if (ptr == nullptr)  // defensive programming
                ptr = std::make_unique<number_series>();
            ptr->add_value(v);
            return *this;
        }
        value_type get_min() const { return ptr ? ptr->get_min() : std::numeric_limits<value_type>::max(); }
        value_type get_max() const { return ptr ? ptr->get_max() : std::numeric_limits<value_type>::min(); }
        value_type amplitude() const { return get_max() - get_min(); }
        size_t size() const { return ptr ? ptr->size() : 0; }
        number_series_wrap3& operator+=(const number_series_wrap3& rhs)
        {
            if (rhs.ptr != nullptr) {  // defensive programming
                if (ptr == nullptr)    // defensive programming
                    ptr = std::make_unique<number_series>();
                *ptr += *rhs.ptr;  // DRY: reuse += over number_series
            }
            return *this;
        }
        static number_series_wrap3 make_random(size_t count)
        {
            return number_series_wrap3{number_series::make_random(count)};
        }
    };

    inline number_series_wrap3 operator+(const number_series_wrap3& lhs, const number_series_wrap3& rhs)
    {
        auto res = lhs;
        res += rhs;  // DRY: reuse += implementation
        return res;
    }
    inline bool operator<(const number_series_wrap3& lhs, const number_series_wrap3& rhs)
    {
        return lhs.amplitude() < rhs.amplitude();
    }
    /* Conclusion:
     * Implementation becomes overly complicated and slow if we allow moved-out variables to be used.
     * number_series_wrap seems to fix the issue with minimal effort but it may hide underlying issues.
     * Default move operations tend to release resources immediately and may fail fast (hopefully not in production).
     */
}  // namespace data_series

#endif  // INCLUDE_NUMBER_SERIES_WRAP3_H
