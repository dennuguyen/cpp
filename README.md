# Extended Template Library

## contains

`contains` is a convenience template function which uses `std::search` as its underlying behaviour and returns true if a sequence of elements [s_first, s_last) is contiguously in the range [first, last).

## transform_if

`transform_if` conditionally applies the given function to a range and stores the result in another range, keeping the original elements order and beginning at `d_first`.