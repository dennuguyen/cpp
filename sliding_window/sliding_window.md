# Sliding Window

Sliding window algorithms can be used when we want to refer to a subarray of contiguous elements of an array with an average complexity of $O(n)$.

The subarray of contiguous elements is captured by a "window" which can slide across an array to capture a different subarray of contiguous elements.

## Usage

The generic sliding window algorithm uses the fast/slow implementation i.e. window slides to right by 1 and the window length increases when the window restarts at the beginning index.

The algorithm accepts a `BinaryPredicate` and `BinaryOperation` where the arguments are `start` and `end` iterators.