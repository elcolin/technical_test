#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest_proxy.h"

/**
 * Given a grid of cost, return the total cost of the smallest-cost path from the top-left corner to the bottom-right
 * corner, going only right or down (no diagonals).
 * @param grid Grid of costs. The vector is width * height long.
 * @param width Width of the grid. Greater than 0.
 * @param height Height of the grid. Greater than 0.
 * @return The total cost of the shortest path, including the start and end cells costs.
 */
int shortestPathCost(std::vector<int> const &grid, std::size_t width, std::size_t height);

TEST_CASE("Find shortest path")
{
    CHECK(shortestPathCost({
                                   1, 1,
                                   2, 1,
                           },
                           2,
                           2)
          == 3);
    CHECK(shortestPathCost({
                                   8, 2, 3, 4,
                                   5, 6, 5, 4,
                                   9, 9, 9, 9,
                           },
                           4,
                           3)
          == (8 + 2 + 3 + 4 + 4 + 9));
    CHECK(shortestPathCost({
                                   8, 2, 8, 4,
                                   5, 3, 2, 4,
                                   9, 1, 9, 9,
                           },
                           4,
                           3)
          == (8 + 2 + 3 + 2 + 4 + 9));
}
