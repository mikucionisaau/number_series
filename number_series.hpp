/// Created by Marius Mikucionis <marius@cs.aau.dk>
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
        // TODO: add data field
        // TODO: add minimum and maximum fields
    public:
        number_series();
        // TODO: add necessary ctors and dtors
        // TODO: add getter "size()" giving the count of numbers in the series
        // TODO: add getters for minimum and maximum
        // TODO: add add_value method to add numbers to the series and maintain min/max
        // TODO: add static member fn for factory method
        // TODO: add operator+= to forward calls to add_value
        // TODO: add operator+ which calls operator+
        // TODO: add operator< to compare amplitudes (difference between maximum and minimum)
    };

    // TODO: create a light-weight wrapper class around number_series
    class number_series_wrap
    {
        // TODO: add number_series member in a smart pointer
    public:
        number_series_wrap();
        // TODO: add the same interface as in number_series which forwards all the calls
    };

}  // namespace data_series

#endif  // INCLUDE_NUMBER_SERIES_H
