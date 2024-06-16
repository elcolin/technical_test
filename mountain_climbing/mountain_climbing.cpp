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

int findStart(std::vector<int> const &grid, std::size_t width, std::size_t height)
{
    for (size_t i = 0; i < width * height; i++)
    {
        if (grid[i] == 0)
            return i;
    }
    return -1;
}

bool    IsInBoundaries(int position, size_t limit)
{
    return !(position >= (int)limit || position < 0);
}

int getNeighbor(int newCol, int newRow, size_t width, size_t height)
{
    if (!IsInBoundaries(newCol, width) || !IsInBoundaries(newRow, height))
        return -1;
    return (newRow * width + newCol);
}

void    updateVisits(size_t cellIdx, size_t pathLength, std::queue<std::tuple<int, int>> &toVisit, std::vector<bool> &visited)
{
    toVisit.push({cellIdx, pathLength});
    visited[cellIdx] = true;
}

int shortestPathLength(std::vector<int> const &grid, std::size_t width, std::size_t height)
{
    int                                 shortestPath = -1;
    int                                 directions[4][2] = { {0,1}, {1,0}, {-1,0}, {0,-1}}; // Pour chaque possibilité de mouvement sans les diagonales
    std::queue<std::tuple<int, int>>    toVisit; //Les cellules à visiter avec la taille du chemin
    std::vector<bool>                   visited(width * height, false); // Les cellules visitées
    auto                                startIdx = findStart(grid, width, height); // Trouver le 'S'

    if (startIdx < 0) // Si le S n'existe pas
        return (-1);
    updateVisits(startIdx, 0, toVisit, visited); //Ajoute startIdx dans les deux conteneurs

    while(!toVisit.empty())
    {
        auto [currentIdx, pathLength] = toVisit.front();
        toVisit.pop(); // On retire la cellule actuelle des cellules à visiter
        size_t row = currentIdx / width;
        size_t col = currentIdx % width;
        for (const auto& dir : directions)
        {
            int neighborIdx = getNeighbor(col + dir[0], row + dir[1], width, height); // La position du voisin

            if (neighborIdx > 0 && !visited[neighborIdx] && (grid[neighborIdx] <= (grid[currentIdx] + 1)))
            {//Si l'index et valide, la cellule jamais visitée et la valeur est tout au plus égale à la valeur + 1

                if (grid[neighborIdx] == 27 && (pathLength + 1 < shortestPath || shortestPath < 0))
                    shortestPath = pathLength + 1; //Si 'E' est trouvé et que le chemin est plus court
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


// void    displayGrid(size_t currentPosition ,std::vector<int> const &grid, std::size_t width, std::size_t height)
// {
//     std::cout << "\n";
//     for (size_t i = 0; i < height; i++)
//     {
//         size_t pos = POSITION(i, 0, width);
//         for (size_t j = 0; j < width; j++)
//         {
//             if (pos + j == currentPosition)
//                 std::cout << 'X';
//             else if (grid[pos + j] == 0)
//                 std::cout << 'S';
//             else if (grid[pos + j] == 27)
//                 std::cout << 'E';
//             else
//                 std::cout << (char) (grid[pos + j] + 97 - 1);
//         }
//         std::cout << "\n";
//     }
// }