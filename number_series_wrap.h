/**
 * Definitions of number_series and number_series_wrap classes.
 */

#ifndef INCLUDE_NUMBER_SERIES_WRAP_H
#define INCLUDE_NUMBER_SERIES_WRAP_H

#include "number_series.h"

namespace data_series
{
    /**
     * Wrapper for a number series with smart move operation (using std::swap).
     * Normally, one should never use variables after their resources have been moved,
     * unless re-initialized.
     * However, this implementation successfully maintains that ptr is never null:
     * even after move, ptr will point to (garbage) data, making all operations safe
     * (even though, not very meaningful).
     */
    class number_series_wrap
    {
        using value_type = number_series::value_type;
        std::unique_ptr<number_series> ptr = std::make_unique<number_series>();

    public:
        number_series_wrap() = default;  // Compiler: ptr=std::make_unique...
        number_series_wrap(const std::initializer_list<value_type>& values):
            ptr{std::make_unique<number_series>(values)}  // skips default initializer
        {}
        /** Ctor binds to both: lvalue and rvalue and constructs an independent number_series.
         * The argument is then moved into ptr.
         * This is slightly better than "const number_series&" as it declares that it is going
         * to need a distinct copy regardless, and then the user has an option to move-in their number_series.
         * Meanwhile "const number_series&" will be making a deep copy in either case.
         * @param ns
         */
        explicit number_series_wrap(number_series ns): ptr{std::make_unique<number_series>(std::move(ns))} {}
        number_series_wrap(const number_series_wrap& other) { *this = other; }  // Compiler: ptr=std::make_unique...
        number_series_wrap& operator=(const number_series_wrap& other)
        {
            *ptr = *other.ptr;  // assign by content
            return *this;
        }
        /**
         * Custom move constructor.
         * Note that ptr is not mentioned here, but it is still initialized as in its declaration.
         */
        number_series_wrap(number_series_wrap&& other) noexcept { *this = std::move(other); }
        /// Custom move: postpone resource release to dtor and maintain ptr!=nullptr.
        number_series_wrap& operator=(number_series_wrap&& other) noexcept
        {
            std::swap(ptr, other.ptr);  // exchange the resources, let the other release our garbage.
            return *this;
        }
        /// Adds a new element to the wrapped number series
        number_series_wrap& add_value(value_type v)
        {
            ptr->add_value(v);
            return *this;
        }
        value_type get_min() const { return ptr->get_min(); }
        value_type get_max() const { return ptr->get_max(); }
        value_type amplitude() const { return get_max() - get_min(); }
        size_t size() const { return ptr->size(); }
        number_series_wrap& operator+=(const number_series_wrap& rhs)
        {
            *ptr += *rhs.ptr;  // DRY: reuse += over number_series
            return *this;
        }
        static number_series_wrap make_random(size_t count)
        {
            return number_series_wrap{number_series::make_random(count)};
        }
    };

    inline number_series_wrap operator+(const number_series_wrap& lhs, const number_series_wrap& rhs)
    {
        auto res = lhs;
        res += rhs;  // DRY: reuse += implementation
        return res;
    }
    inline bool operator<(const number_series_wrap& lhs, const number_series_wrap& rhs)
    {
        return lhs.amplitude() < rhs.amplitude();
    }
}  // namespace data_series

#endif  // INCLUDE_NUMBER_SERIES_WRAP_H
