#include <vector>
#include <tuple>
#include <queue>


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


struct Compare {
    bool operator()(const std::tuple<int, int>& a, const std::tuple<int, int>& b) {
        return std::get<1>(a) > std::get<1>(b);
    }
};

void    updatePath(size_t cellIdx, size_t pathCost, std::priority_queue<
                std::tuple<int, int>, 
                std::vector<std::tuple<int, int>>,
                Compare > &pq, std::vector<int> dist)
{//ressemble beaucoup a updateVisit, template?
    pq.push({cellIdx, pathCost});
    dist[cellIdx] = pathCost;
}

bool    IsInBoundaries(int position, size_t limit)// Si la nouvelle valeur respecte les limites
{
    return !(position >= (int)limit || position < 0);
}

int getNeighbor(int newCol, int newRow, size_t width, size_t height)
{
    if (!IsInBoundaries(newCol, width) || !IsInBoundaries(newRow, height))
        return -1;
    return (newRow * width + newCol);
}

int shortestPathCost(std::vector<int> const &grid, std::size_t width, std::size_t height)
{
        std::priority_queue<
                std::tuple<int, int>, 
                std::vector<std::tuple<int, int>>,
                Compare >       pq; // queue avec une priorité pour les chemins les moins coûteux
        std::vector<int>        dist(width * height, std::numeric_limits<int>::max());// vecteur des chemins les moins coûteux
        int                     cheapestPath = -1;
        int                     directions[2][2] = {{0,1}, {1,0}}; //directions sans les diagonales

        updatePath(0, grid[0], pq, dist);
        while(!pq.empty())
        {
                auto[currentIdx, pathCost] = pq.top();
                pq.pop();
                size_t row = currentIdx / width;
                size_t col = currentIdx % width;
                if (row == height - 1 && col == width - 1 && (cheapestPath > pathCost || cheapestPath < 0))
                //Si dernière cellule et que le coût ici est moindre que le coût précédent
                        cheapestPath = pathCost;
                for (const auto& dir : directions)
                {// On regarde les voisins en haut et à droite
                        int neighborIdx = getNeighbor(col + dir[0], row + dir[1], width, height);
                        if (neighborIdx < 0)
                                continue;
                        int newCost = pathCost + grid[neighborIdx];
                        if (newCost < dist[neighborIdx]) //Si le coût trouvé est moindre que le coût stocké dans dist à cet emplacement
                                updatePath(neighborIdx, newCost, pq, dist);
                }
        }
        return (cheapestPath);

}

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
