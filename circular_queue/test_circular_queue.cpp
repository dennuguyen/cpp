#include <initializer_list>
#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "circular_queue.hpp"
#include "doctest.h"

TEST_CASE("Initialiser list constructor") {
    CircularQueue<int, 3> cq = {1, 2, 3};
    CHECK(cq[0] == 1);
    CHECK(cq[1] == 2);
    CHECK(cq[2] == 3);
}

// Not in a queue.
// TEST_CASE("operator[]") {
//     SUBCASE("Elements should be assignable") {
//         CircularQueue<int, 3> cq = {1, 2, 3};
//         cq[0] = 42;
//         CHECK(cq[0] == 42);
//         cq[1] = 43;
//         CHECK(cq[1] == 43);
//         cq[2] = 44;
//         CHECK(cq[2] == 44);
//     }

//     // Should be undefined behaviour.
//     SUBCASE("Index greater than capacity should wrap around") {
//         CircularQueue<int, 3> cq = {1, 2, 3};
//         cq[3] = 42;
//         CHECK(cq[0] == 42);
//         CHECK(cq[1] == 2);
//         CHECK(cq[2] == 3);
//     }
// }

TEST_CASE("full()") {
    SUBCASE("size 3 with capacity 3 should be full") {
        CircularQueue<int, 3> cq = {1, 2, 3};
        CHECK(cq.full() == true);
    }

    SUBCASE("size 2 with capacity 3 should not be full") {
        CircularQueue<int, 3> cq = {1, 2};
        CHECK(cq.full() == false);
    }

    SUBCASE("size 0 with capacity 3 should not be full") {
        CircularQueue<int, 3> cq = {};
        CHECK(cq.full() == false);
    }
}

TEST_CASE("empty()") {
    SUBCASE("size 3 with capacity 3 should not be empty") {
        CircularQueue<int, 3> cq = {1, 2, 3};
        CHECK(cq.empty() == false);
    }

    SUBCASE("size 2 with capacity 3 should not be empty") {
        CircularQueue<int, 3> cq = {1, 2};
        CHECK(cq.empty() == false);
    }

    SUBCASE("size 0 with capacity 3 should be empty") {
        CircularQueue<int, 3> cq = {};
        CHECK(cq.empty() == true);
    }
}

TEST_CASE("push()") {
    SUBCASE("pushing empty circular queue") {
        CircularQueue<int, 3> cq = {};
        cq.push(1);
        CHECK(cq[0] == 1);

        cq.push(2);
        CHECK(cq[0] == 1);
        CHECK(cq[1] == 2);

        cq.push(3);
        CHECK(cq[0] == 1);
        CHECK(cq[1] == 2);
        CHECK(cq[2] == 3);
        CHECK(cq.full() == true);
    }

    SUBCASE("pushing full queue will pop oldest") {
        CircularQueue<int, 3> cq = {1, 2, 3};
        cq.push(4);
        CHECK(cq[0] == 2);
        CHECK(cq[1] == 3);
        CHECK(cq[2] == 4);
        CHECK(cq.full() == true);
    }
}

TEST_CASE("pop()") {
    SUBCASE("popping full queue") {
        CircularQueue<int, 3> cq = {1, 2, 3};
        cq.pop();
        CHECK(cq[0] == 2);
        CHECK(cq[1] == 3);
        cq.pop();
        CHECK(cq[0] == 3);
        cq.pop();
        CHECK(cq.empty());
    }

    SUBCASE("popping empty queue") {
        CircularQueue<int, 3> cq = {};
        CHECK(cq.pop() == false);
    }
}

TEST_CASE("front()") {
    SUBCASE("checking front after push") {
        CircularQueue<int, 3> cq;
        cq.push(1);
        CHECK(cq.front() == 1);
        cq.push(2);
        CHECK(cq.front() == 1);
        cq.push(3);
        CHECK(cq.front() == 1);
    }

    SUBCASE("checking front after pop") {
        CircularQueue<int, 3> cq = {1, 2, 3};
        CHECK(cq.front() == 1);
        cq.pop();
        CHECK(cq.front() == 2);
        cq.pop();
        CHECK(cq.front() == 3);
    }

    SUBCASE("push pop front") {
        CircularQueue<int, 3> cq = {};
        cq.push(1);
        CHECK(cq.front() == 1);
        CHECK(cq.empty() == false);
        cq.pop();
        CHECK(cq.empty() == true);
    }
}

// TEST_CASE("iterator +") {
//     SUBCASE("iterating forward from start of queue") {
//         CircularQueue<int, 3> cq = {1, 2, 3};
//         auto it = cq.begin();
//         CHECK(*(it + 0) == 1);
//         CHECK(*(it + 1) == 2);
//         CHECK(*(it + 2) == 3);
//     }

//     SUBCASE("iterating forward where head is in middle of queue") {
//         CircularQueue<int, 3> cq = {1, 2, 3};
//         cq.pop();
//         cq.push(4);
//         auto it = cq.begin();
//         CHECK(*(it + 0) == 2);
//         CHECK(*(it + 1) == 3);
//         CHECK(*(it + 2) == 4);
//         std::cout << cq << std::endl;
//     }

//     SUBCASE("iterating forward where head is at start of queue") {
//         // CircularQueue<int, 3> cq = {1, 2, 3};
//         // auto it = cq.begin();
//         // CHECK(*it == 1);
//         // CHECK(*(it + 1) == 2);
//         // CHECK(*(it + 2) == 3);
//     }

//     SUBCASE("iterating backward") {}
// }