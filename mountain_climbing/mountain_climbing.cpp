#include <algorithm>
#include <tuple>
#include <vector>
#include <queue>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest_proxy.h"

/**
 * Given a grid of height, return the shortest path from the point with height=0 to the point with height = 27.
 * The path can only go to a new cell with an height at most one higher. No diagonals.
 * @param grid Grid of height. The size is equal to width * height.
 * @param width Width of the grid. Greater than 0.
 * @param height Height of the grid. Greater than 0.
 * @return The shortest path from the cell with height = 0 to the cell with height = 25.
 */

#define POSITION(row, col, width) ((row) * (width) + (col))

class Grid{
    private:
        size_t  &width;
        size_t  &height;
        std::vector<int> const &grid;
        std::vector<bool> visited(width * height, false);

    public:
        //Coplien form
        Grid();
        Grid(std::vector<int> const &grid, std::size_t width, std::size_t height): width(width), height(height), grid(grid)
        {

        }
    
};

int findStart(std::vector<int> const &grid, std::size_t width, std::size_t height)
{
    for (size_t i = 0; i < width * height; i++)
    {
        if (grid[i] == 0)
            return i;
    }
    //send error
    return -1;
}

void    displayGrid(size_t currentPosition ,std::vector<int> const &grid, std::size_t width, std::size_t height)
{
    std::cout << "\n";
    for (size_t i = 0; i < height; i++)
    {
        size_t pos = POSITION(i, 0, width);
        for (size_t j = 0; j < width; j++)
        {
            if (pos + j == currentPosition)
                std::cout << 'X';
            else if (grid[pos + j] == 0)
                std::cout << 'S';
            else if (grid[pos + j] == 27)
                std::cout << 'E';
            else
                std::cout << (char) (grid[pos + j] + 97 - 1);
        }
        std::cout << "\n";
    }
}

bool    IsInBoundaries(int position, size_t limit)
{
    return !(position >= (int)limit || position < 0);
}

void    updateVisits(size_t cellIdx, size_t pathLength, std::queue<std::tuple<int, int>> &toVisit, std::vector<bool> &visited)
{
    toVisit.push({cellIdx, pathLength});
    visited[cellIdx] = true;
}

int shortestPathLength(std::vector<int> const &grid, std::size_t width, std::size_t height)
{
    // Directions matrice to check the neighbors of a cell 
    int directions[4][2] = {
        {0,1},
        {1,0},
        {-1,0},
        {0,-1}
    };
    int shortestPath = -1;
    //A queue of cells to visit and their path length
    std::queue<std::tuple<int, int>> toVisit;
    //A vector for visited cells
    std::vector<bool> visited(width * height, false);

    //Finding the start S
    auto startIdx = findStart(grid, width, height);
    if (startIdx < 0)
        return (-1);
    //sets the cell index to true in visited and pushes it in toVisit
    updateVisits(startIdx, 0, toVisit, visited);

    while(!toVisit.empty())
    {
        //getting the cell index and pathLength at to front of the queue
        auto [currentIdx, pathLength] = toVisit.front();
        toVisit.pop();
        //rows and columns for readibility
        size_t row = currentIdx / width;
        size_t col = currentIdx % width;
        // const auto& dir : directions?
        for (size_t i = 0; i < 4; i++)
        {//Travelling through different directions to check neighbors of the current cell --> overflow int fix!
            int newCol = directions[i][0] + col;
            int newRow = directions[i][1] + row;
            if (!IsInBoundaries(newCol, width) || !IsInBoundaries(newRow, height))
            //Checking if new position is within boundaries
                continue;
            int neighborIdx = POSITION(newRow, newCol, width);
            if (!visited[neighborIdx] && (grid[neighborIdx] <= (grid[currentIdx] + 1)))
            {//if the cell wasn't visited and value is at most current value + 1
            
                if (grid[neighborIdx] == 27 && (pathLength + 1 < shortestPath || shortestPath < 0))
                // If end E found, updating shortest path
                    shortestPath = pathLength + 1;
                updateVisits(neighborIdx, pathLength + 1, toVisit, visited);
            }
        }
    }
    return (shortestPath);
}

/// Parse a grid of letters from a to z corresponding to height from 1 to 26, and S for height 0 and E for height 27.
/// \return A tuple with the grid, its width and its height.
std::tuple<std::vector<int>, std::size_t, std::size_t> parseGrid(std::string const &lines)
{
    std::vector<int> grid;
    std::size_t spaceIdx = lines.find('\n');
    std::size_t width = spaceIdx;

    std::size_t cursor = 0;
    while (spaceIdx != std::string::npos)
    {
        std::transform(lines.begin() + cursor,
                       lines.begin() + spaceIdx,
                       std::back_inserter(grid),
                       [](char c)
                       {
                           switch (c)
                           {
                               case 'S': return 0;
                               case 'E': return 27;
                               default: return 1 + c - 'a';
                           }
                       });
        cursor = std::min(spaceIdx + 1, lines.size());
        spaceIdx = lines.find('\n', cursor);
        if (spaceIdx == std::string::npos && cursor != lines.size())
            spaceIdx = lines.size();
    }
    return {grid, width, grid.size() / width};
}

TEST_CASE("Test parse grid")
{
    auto [grid1, width1, height1] = parseGrid("Sab\ncde\nfgE");
    CHECK_EQ(grid1, std::vector<int> {0, 1, 2, 3, 4, 5, 6, 7, 27});
    CHECK_EQ(width1, 3);
    CHECK_EQ(height1, 3);
}

TEST_CASE("Find shortest path")
{
    CHECK_EQ(std::apply(shortestPathLength, parseGrid(R"(Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi)")),
             31);
    CHECK_EQ(std::apply(shortestPathLength, parseGrid(R"(abcccccccccaaaaaaaaaaccccccccccccaaaaaaaaccaaccccccccccccccccccccccccccccccccccccccccccccaaaaaa
abccccccccccaaaaaaaaaccccccccccccaaaaaaaaaaaacccccccccccaacccacccccccccccccccccccccccccccaaaaaa
abcccccccccccaaaaaaacccccccccccccaaaaaaaaaaaaaacccccccccaaacaacccccccccaaaccccccccccccccccaaaaa
abccccccccccaaaaaaccccccccccccccaaaaaaaaaaaaaaaccccccccccaaaaaccccccccccaaacccccccccccccccccaaa
abccccccccccaaaaaaaccccccccccccaaaaaaaaaaaaaacccccccccccaaaaaacccccccccaaaacccccccccccccccccaac
abaaccaaccccaaccaaaccccccccaaaaaaaaaaaaaaacaaccccccccccaaaaaaaacccccccccaaalcccccccccccccccaaac
abaaaaaacccccccccaaccccccccaaaaaacccaaaacccaaccccccccccaaaaaaaaccccccccalllllllcccccccccccccccc
abaaaaaacccccccaaacccccccccaaaaccccccaaaccccaaaaacccccccccaacccccccaaaakllllllllcccccccaacccccc
abaaaaaacccccccaaaacccccccccaacccccccaaaccccaaaaacccccccccaacccccccaakkklllpllllccccacaaacccccc
abaaaaaaaccccccaaaaccccaaccccccccccccccccccaaaaaaccccccccccccccccccckkkkpppppplllcccaaaaaaacccc
abaaaaaaacaaaccaaaaccaaaaaaccccccccccccccccaaaaaacccccccaaaccccckkkkkkkpppppppplllcddaaaaaacccc
abcaaaacccaacccccccccaaaaaacccccaaaccccccccaaaaaacccccccaaaaccjkkkkkkkpppppuppplmmdddddaaaccccc
abccaaaaaaaaaccccccccaaaaaaccccaaaaaacccccccaaacccccccccaaaajjjkkkkkrpppuuuuupppmmmdddddacccccc
abccccaaaaaaaacccccccaaaaacccccaaaaaacccccccccccccccccccaaacjjjjrrrrrrppuuuuupqqmmmmmddddaccccc
abccccaaaaaaaaacccccccaaaacccccaaaaaaccccccccccccccccccccccjjjrrrrrrrrpuuuxuvvqqqmmmmmddddccccc
abccccaaaaaaaaacccccccccccccccccaaaaaccccaacccaccccccccaaccjjjrrrruuuuuuuxxyvvqqqqqmmmmmdddcccc
abccccaaaaaaaacccccccccaaaccccccaacaaccccaaacaacccaaacaaaccjjjrrrtuuuuuuuxxyvvvqqqqqmmmmdddcccc
abccaaaaaaaacccccccccccaaaaaccccccccccccccaaaaacccaaaaaaaccjjjrrttttxxxxxxyyvvvvvqqqqmmmmdeeccc
abccaaaccaaaccccccccaacaaaaacccccccccccccaaaaaacccaaaaaacccjjjrrtttxxxxxxxyyvvvvvvvqqqmmmeeeccc
abaaaaaaaaaacccaaaccaaaaaaaaaaaccaaaccccaaaaaaaacccaaaaaaaajjjqqrttxxxxxxxyyyyyyvvvqqqnnneeeccc
SbaaaaaaaaccccaaaaccaaaaaaaaaaaaaaaaacccaaaaaaaaccaaaaaaaaacjjjqqtttxxxxEzzyyyyvvvvqqqnnneeeccc
abcaaaaaacccccaaaaccccaaaaaaaccaaaaaaccccccaaccccaaaaaaaaaaciiiqqqtttxxxyyyyyyvvvvrrrnnneeecccc
abcaaaaaacccccaaaacccaaaaaaaaccaaaaaaccccccaaccccaaacaaacccciiiqqqqttxxyyyyyywvvvrrrnnneeeecccc
abcaaaaaaccccccccccccaaaaaaaaacaaaaacccccccccccccccccaaaccccciiiqqtttxxyyyyyywwrrrrnnnneeeccccc
abcaaacaacccccaacccccaaaaaaaaacaaaaacccccccccccccccccaaaccccciiiqqttxxxywwyyywwrrrnnnneeecccccc
abccccccccaaacaaccccccccccacccccccccccccccccccccccccccccccccciiqqqttxxwwwwwwywwrrrnnneeeccccccc
abccaacccccaaaaaccccccccccccccccccccccccccccccccccccccccaacaaiiqqqttwwwwsswwwwwrrrnnfffeccccccc
abaaaaccccccaaaaaacccccccccccccccccccccccccccccaaaccccccaaaaaiiqqqttssssssswwwwrrronfffaccccccc
abaaaaaacccaaaaaaacccccccccccccccccccccccccccaaaaaacccccaaaaaiiqqqssssssssssswrrrooofffaaaacccc
abaaaaaaccaaaaaacccccccccccccccccccccccccccccaaaaaacccccaaaaaiiqqqppssspppssssrrrooofffaaaacccc
abaaaaaaccaacaaacccccccccccccccccccccccccccccaaaaaacccccaaaaaiihpppppppppppossrrooofffaaaaacccc
abaaaaccccccccaacccccccccccccccccccccccccccccaaaaaccccccccaaahhhhppppppppppoooooooofffaaaaccccc
abaaaaccccccccccaacccccccccccccccccaaacccccccaaaaacccccccccccchhhhhhhhhhggpoooooooffffaaaaccccc
abccaacccccccacaaaccccccccccccccccaaaaacccccccccccccccccccccccchhhhhhhhhggggoooooffffaacaaacccc
abccccccccccaaaaacaaccccccccccccccaaaaaccccccccccccccccccccccccchhhhhhhhggggggggggffcaacccccccc
abccccccccccaaaaaaaaccccccccccccccaaaacccaacccccccccccaccccccccccccccaaaaaggggggggfcccccccccccc
abccccccccccccaaaaaccccaacccccccccaaaacaaaaccccccccaaaaccccccccccccccaaaacaaagggggcccccccccaccc
abcccccccccccaaaaacccccaacccccccccaaaaaaaaaccccccccaaaaaaccccccccccccaaaccaaaacccccccccccccaaac
abcccccccccccaacaaccaaaaaaaacccaaaaaaaaaaaccccccccccaaaaccccccccccccccaccccaaacccccccccccccaaaa
abccccccccccccccaaccaaaaaaaaccaaaaaaaaaaaccccccccccaaaaacccccccccccccccccccccacccccccccccccaaaa
abccccccccccccccccccccaaaaacccaaaaaaaaaaaacccccccccaacaacccccccccccccccccccccccccccccccccaaaaaa)")),
             422);
}
