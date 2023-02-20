#include "number_series_wrap.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/// number_series_wrap class

TEST_CASE("NSW: Maintain minimum and maximum values")
{
    using series = data_series::number_series_wrap;
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

TEST_CASE("NSW: Special members: ctors, dtor, assignment")
{
    using series = data_series::number_series_wrap;
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
        // Use after move (meaningless and dangerous):
        CHECK(ns.size() == 0);  // your implementation may differ or even crash
        ns = {};                // reinitialize after move for safe reuse
        CHECK(ns.size() == 0);  // OK, also deterministic.
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
        // Use after move (meaningless and dangerous):
        CHECK(ns.size() == 4);  // your implementation may differ or even crash
        ns = {};                // reinitialize after move for safe reuse
        CHECK(ns.size() == 0);  // OK, also deterministic.
    }
}

TEST_CASE("NSW: Class should have a static factory method")
{
    using series = data_series::number_series_wrap;
    auto ns = series::make_random(4);
    CHECK(ns.size() == 4);
}

TEST_CASE("NSW: operator+ and operator+= over number series")
{
    using series = data_series::number_series_wrap;
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

TEST_CASE("NSW: operator< using amplitudes")
{
    using series = data_series::number_series_wrap;
    auto ns1 = series{6, 3, 9};
    CHECK(ns1.amplitude() == 6);
    auto ns2 = series{24, 21, 22};
    CHECK(ns2.amplitude() == 3);
    CHECK(ns2 < ns1);
}
