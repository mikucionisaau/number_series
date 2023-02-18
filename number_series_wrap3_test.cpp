#include "number_series_wrap3.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/// number_series_wrap3 class

TEST_CASE("NSW3: Maintain minimum and maximum values")
{
    using series = data_series::number_series_wrap3;
    auto ns = series{};
    ns.add_value(10);
    CHECK(ns.get_min() == 10);
    CHECK(ns.get_max() == 10);
    SUBCASE("Add greater")
    {
        ns.add_value(15);
        CHECK(ns.get_min() == 10);
        CHECK(ns.get_max() == 15);
        ns.add_value(17);
        CHECK(ns.get_min() == 10);
        CHECK(ns.get_max() == 17);
        ns.add_value(13);
        CHECK(ns.get_min() == 10);
        CHECK(ns.get_max() == 17);
    }
    SUBCASE("Add lesser")
    {
        ns.add_value(5);
        CHECK(ns.get_min() == 5);
        CHECK(ns.get_max() == 10);
        ns.add_value(3);
        CHECK(ns.get_min() == 3);
        CHECK(ns.get_max() == 10);
        ns.add_value(7);
        CHECK(ns.get_min() == 3);
        CHECK(ns.get_max() == 10);
    }
}

TEST_CASE("NSW3: Special members: ctors, dtor, assignment")
{
    using series = data_series::number_series_wrap3;
    auto ns = series{11, 3, 7};
    CHECK(ns.size() == 3);
    CHECK(ns.get_min() == 3);
    CHECK(ns.get_max() == 11);
    SUBCASE("Copy construct")
    {
        auto copy = ns;
        CHECK(copy.size() == 3);  // identical
        CHECK(copy.get_min() == 3);
        CHECK(copy.get_max() == 11);
        CHECK(ns.size() == 3);  // intact
        CHECK(ns.get_min() == 3);
        CHECK(ns.get_max() == 11);
    }
    SUBCASE("Copy assign")
    {
        auto other = series{27, 20, 33, 23};
        CHECK(other.size() == 4);  // some other value
        CHECK(other.get_min() == 20);
        CHECK(other.get_max() == 33);
        other = ns;
        CHECK(other.size() == 3);  // identical
        CHECK(other.get_min() == 3);
        CHECK(other.get_max() == 11);
        CHECK(ns.size() == 3);  // intact
        CHECK(ns.get_min() == 3);
        CHECK(ns.get_max() == 11);
    }
    SUBCASE("Move construct")
    {
        auto copy = std::move(ns);  // move constructor
        CHECK(copy.size() == 3);    // identical to old ns value
        CHECK(copy.get_min() == 3);
        CHECK(copy.get_max() == 11);
        CHECK(ns.size() == 0);  // something else, but still a valid state
    }
    SUBCASE("Move assign")
    {
        auto other = series{27, 20, 33, 23};
        CHECK(other.size() == 4);  // some other value
        CHECK(other.get_min() == 20);
        CHECK(other.get_max() == 33);
        other = std::move(ns);
        CHECK(other.size() == 3);  // identical to old ns value
        CHECK(other.get_min() == 3);
        CHECK(other.get_max() == 11);
        CHECK(ns.size() == 0);  // something else, but still a valid state
        // Use after move (meaningless and dangerous):
        CHECK(ns.size() == 0);  // your implementation may differ or even crash
        ns = {};                // reinitialize after move for safe reuse
        CHECK(ns.size() == 0);  // OK, also deterministic.
    }
}

TEST_CASE("NSW3: Class should have a static factory method")
{
    using series = data_series::number_series_wrap3;
    auto ns = series::make_random(4);
    CHECK(ns.size() == 4);
}

TEST_CASE("NSW2: operator+ and operator+= over number series")
{
    using series = data_series::number_series_wrap3;
    auto ns1 = series::make_random(2);
    CHECK(ns1.size() == 2);
    auto ns2 = series::make_random(3);
    CHECK(ns2.size() == 3);
    auto ns3 = ns1 + ns2;
    CHECK(ns1.size() == 2);
    CHECK(ns2.size() == 3);
    CHECK(ns3.size() == 3);
    ns2.add_value(10);
    CHECK(ns2.size() == 4);
    (ns3 += ns1) += ns2;
    CHECK(ns1.size() == 2);
    CHECK(ns2.size() == 4);
    CHECK(ns3.size() == 4);
}

TEST_CASE("NSW3: operator< using amplitudes")
{
    using series = data_series::number_series_wrap3;
    auto ns1 = series{6, 3, 9};
    CHECK(ns1.amplitude() == 6);
    auto ns2 = series{24, 21, 22};
    CHECK(ns2.amplitude() == 3);
    CHECK(ns2 < ns1);
}

TEST_CASE("Test move behavior of std::vector")
{
    auto v = std::vector{1, 2, 3};
    REQUIRE(v.size() == 3);  // stop the test if this condition fails (protects against potential crash below)
    CHECK(v[0] == 1);        // crash if v.empty()
    CHECK(v[1] == 2);
    CHECK(v[2] == 3);
    CHECK(v.capacity() == 3);
    SUBCASE("Move construct")
    {
        auto u = std::move(v);   // move-construct
        REQUIRE(u.size() == 3);  // protect against potential crash below
        CHECK(u[0] == 1);        // crash if u.empty()
        CHECK(u[1] == 2);
        CHECK(u[2] == 3);
        CHECK(u.capacity() == 3);
        REQUIRE(v.size() == 0);    // your implementation may be different
        CHECK(v.capacity() == 0);  // your implementation may be different
    }
    SUBCASE("Move assign")
    {
        auto u = std::vector{3, 4};
        REQUIRE(u.size() == 2);
        CHECK(u[0] == 3);
        CHECK(u[1] == 4);
        CHECK(u.capacity() == 2);
        u = std::move(v);
        REQUIRE(u.size() == 3);
        CHECK(u[0] == 1);
        CHECK(u[1] == 2);
        CHECK(u[2] == 3);
        CHECK(u.capacity() == 3);
        REQUIRE(v.size() == 0);    // your implementation may differ(!)
        CHECK(v.capacity() == 0);  // your implementation may differ(!)
        // if the above passes, then it means that during move:
        // 1) initial v2 resources were released (not swapped)
        // 2) resources of v have been moved into v2
        // 3) v has been put into a valid state
    }
}
