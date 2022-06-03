# Dynamic Programming

Dynamic programming (DP) solves large problems recursively by building subproblems of smaller size in polynomial time.

Greedy algorithms can find the wrong globally optimal solution whereas DP considers all options at each stage. DP is characterised by overlapping subproblems where previously computed subsolutions can be reused.

> Pick dynamic programming if you want a divide-and-conquer approach but want to know every possible subproblem.

## Steps

1. Define the subproblem. We want a useful subproblem that will solve the larger problem.
1. Define the recurrence relation. It's easy to keep track of the subsolution in an array.
1. Define the base case.

## Methods

### Top-Down Approach

Before attempting to solve a subproblem, we check the table to see if it's already solved.

### Bottom-Up Approach

