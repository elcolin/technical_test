#ifndef MOUNTAIN_CLIMBING
#define MOUNTAIN_CLIMBING

#include <algorithm>
#include <tuple>
#include <vector>
#include <queue>
#include <iostream>

#define POSITION(row, col, width) ((row) * (width) + (col))

class Grid{
    private:
        size_t                              width;
        size_t                              height;
        int                                 currentPositon;
        int                                 shortestPath;
        std::vector<int> const              &grid;
        std::vector<bool>                   visited;
        std::queue<std::tuple<int, int>>    toVisit;

    public:
        //Coplien form
        Grid();
        Grid(std::vector<int> const &grid, std::size_t width, std::size_t height): width(width), height(height), grid(grid), visited(width * height, false), shortestPath(-1)
        {
            currentPositon = findStart();
            updateVisits(currentPositon, 0);
            // if (currentPositon < 0)
            // send error
        };
        size_t      getWidth() const  {return width;};
        size_t      getHeight() const {return height;};
        int         getCurrentPosition() const {return currentPositon;};
        const int   &operator[](size_t idx) const {return this->grid[idx];}
        int         findStart();
        void        updateVisits(size_t cellIdx, size_t pathLength);
        bool        empty() {toVisit.empty();};

};

std::ostream &operator<<(std::ostream &os, Grid const &grid)
{
    std::cout << "\n";
    for (size_t i = 0; i < grid.getHeight(); i++)
    {
        size_t pos = POSITION(i, 0, grid.getWidth());
        for (size_t j = 0; j < grid.getWidth(); j++)
        {
            if (pos + j == grid.getCurrentPosition())
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

#endif