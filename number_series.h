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
        // int padding[12]; // experiment with sort performance
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

    /**
     * Wrapper for number series with smart move operation (using std::swap).
     * Normally, one should never use variables after their resources have been moved,
     * unless re-initialized.
     * However, this implementation successfully maintains that ptr is never null,
     * even after move, ptr will point to (garbage) data, making all operations safe
     * (even though, not very meaningful).
     */
    class number_series_wrap
    {
        using value_type = number_series::value_type;
        std::unique_ptr<number_series> ptr = std::make_unique<number_series>();

    public:
        number_series_wrap() = default;
        number_series_wrap(const std::initializer_list<value_type>& values):
            ptr{std::make_unique<number_series>(values)}
        {}
        /** Ctor binds to both: lvalue and rvalue and constructs an independent number_series argument.
         * The argument is then moved into ptr.
         * This is slightly better than "const number_series&" as this ctor declares that it is going
         * to need a distinct copy regardless, and then the user has an option to move their number_series.
         * Meanwhile "const number_series&" will be making a copy in either case.
         * @param ns
         */
        number_series_wrap(number_series ns): ptr{std::make_unique<number_series>(std::move(ns))} {}
        number_series_wrap(const number_series_wrap& other) { *this = other; }
        number_series_wrap& operator=(const number_series_wrap& other)
        {
            *ptr = *other.ptr;  // assign by content
            return *this;
        }
        /* // default implementations (below) crash on size() after std::move:
        number_series_wrap(number_series_wrap&& other) noexcept = default;
        number_series_wrap& operator=(number_series_wrap&& other) noexcept = default;
        */
        number_series_wrap(number_series_wrap&& other) noexcept { *this = std::move(other); }
        number_series_wrap& operator=(number_series_wrap&& other) noexcept
        {
            std::swap(ptr, other.ptr);  // exchange the resources
            return *this;
        }

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
        static number_series_wrap make_random(size_t count) { return {number_series::make_random(count)}; }
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

    /**
     * Wrapper with default move releases resource immediately and leaves the rhs.ptr==nullptr,
     * which invalidates all other operations like size() (because ptr is null).
     * To accommodate possibility for ptr==nullptr and a potential reuse of such objects,
     * we use defensive programming against ptr==nullptr.
     */
    class number_series_wrap2
    {
        using value_type = number_series::value_type;
        std::unique_ptr<number_series> ptr;

    public:
        number_series_wrap2() = default;
        number_series_wrap2(const std::initializer_list<value_type>& values):
            ptr{std::make_unique<number_series>(values)}
        {}
        number_series_wrap2(number_series ns): ptr{std::make_unique<number_series>(std::move(ns))} {}
        number_series_wrap2(const number_series_wrap2& other) { *this = other; }
        number_series_wrap2& operator=(const number_series_wrap2& other)
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
        number_series_wrap2(number_series_wrap2&& other) noexcept = default;
        number_series_wrap2& operator=(number_series_wrap2&& other) noexcept = default;
        ~number_series_wrap2() noexcept = default;
        number_series_wrap2& add_value(value_type v)
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
        number_series_wrap2& operator+=(const number_series_wrap2& rhs)
        {
            if (rhs.ptr != nullptr) {  // defensive programming
                if (ptr == nullptr)    // defensive programming
                    ptr = std::make_unique<number_series>();
                *ptr += *rhs.ptr;  // DRY: reuse += over number_series
            }
            return *this;
        }
        static number_series_wrap2 make_random(size_t count) { return {number_series::make_random(count)}; }
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
    /* Conclusion: never use a variable after its content has been moved out.
     * That makes implementations much simpler by default.
     */
}  // namespace data_series

#endif  // INCLUDE_NUMBER_SERIES_H
