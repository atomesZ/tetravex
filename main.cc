#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <math.h>

struct tile {
    char up;
    char left;
    char right;
    char down;
};

inline int get_x_dim(const std::vector<tile>& s)
{
    const int vector_size = s.size();

    switch (vector_size)
    {
    case 4:
        return 2;
    case 9:
        return 3;
    case 16:
        return 4;
    case 25:
        return 5;
    case 36:
        return 6;

    default:
        exit(EXIT_FAILURE);
    }
}

inline unsigned int num_wrong_links(const std::vector<tile>& s, const unsigned int i)
{
    static const int x_dim = get_x_dim(s);

    unsigned int res = 0;

    // TODO speedup by not using if and adding a boolean directly

    /* We do not use those 2 since we iterate on every tiles, links would be counted 2 times
    static const int last_row_first_index = x_dim * (x_dim - 1);

    // If not on last row
    if (!(i >= last_row_first_index))
    {
        // Get num_wrong_links with lower tile
        res += s[i].down != s[i + x_dim].up;
    }

    // If not on last column
    if (!(i % x_dim == x_dim - 1))
    {
        // Get num_wrong_links with right tile
        res += s[i].right != s[i + 1].left;
    }
    */

    // If not en first column
    if (!(i % x_dim == 0))
    {
        // Get num_wrong_links with left tile
        res += s[i].left != s[i - 1].right;
    }

    // If not on first row
    if (!(i < x_dim))
    {
        // Get num_wrong_links with upper tile
        res += s[i].up != s[i - x_dim].down;
    }

    return res;
}

unsigned int E(const std::vector<tile>& s)
{
    unsigned int energy = 0;

    for (unsigned int i = 0; i < s.size(); ++i)
    {
        // Get current tile number of wrong links and add it to energy
        energy += num_wrong_links(s, i);
    }

    return energy;
}

std::vector<tile> voisin(const std::vector<tile>& s0, const std::vector<int>& moveableTilesIndexes)
{
    std::vector<int> out;
    std::sample(
        moveableTilesIndexes.begin(),
        moveableTilesIndexes.end(),
        std::back_inserter(out),
        2,
        std::mt19937{std::random_device{}()}
    );

    std::vector<tile> res = s0;

    std::swap(res[out[0]], res[out[1]]);

    return res;
}

inline double P(const int delta_e, const double t)
{
    return exp(-(double)delta_e / t);
}

// FIXME fix the temp function (rework everything to make it better)
inline double temp(const int energy)
{
    static double temperature = 1000.0;
    static int old_energy = energy;
    static int times_stuck_at_old_energy = 0;

    if (old_energy == energy)
    {
        ++times_stuck_at_old_energy;

        if (times_stuck_at_old_energy >= 100)
            temperature = 10.0;
    }
    else
    {
        times_stuck_at_old_energy = 0;
        old_energy = energy;
    }

    temperature *= 0.99;

    return temperature;
}

std::vector<tile> find_best_tiles_setup(std::vector<tile>& s0, const std::vector<int>& moveableTilesIndexes)
{
    std::vector<tile> s = s0;
    std::vector<tile> g = s0;

    int e = E(s);

    if (e == 0)
        return s;

    int m = e;
    unsigned int k = 0;


    while (true)
    {
        std::cout << e << std::endl;
        const std::vector<tile> sn = voisin(s, moveableTilesIndexes);
        const int en = E(sn);

        if (en < e || (float) rand()/RAND_MAX < P(en - e, temp(e)))
        {
            s = sn;
            e = en;
        }

        if (e < m)
        {
            if (e == 0)
                return s;

            g = s;
            m = e;
        }

        ++k;
    }
}

int main(int argc, char** argv)
{
    // Check that we have all parameters
    if (argc != 3)
    {
        exit(EXIT_FAILURE);
    }

    // Open input file
    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);
    if (!inputFile.is_open() || !outputFile.is_open())
        exit(EXIT_FAILURE);

    // Get lines of input file
    std::string line;

    std::vector<tile> tiles;
    std::vector<int> moveableTilesIndexes;
    int i_tile = 0;

    while (getline(inputFile, line)){ //read data from file object and put it into string.
        // Add tile
        tiles.push_back(tile{line[0], line[1], line[2], line[3]});

        // Add tile to moveables
        if (line.find('@') == std::string::npos)
            moveableTilesIndexes.push_back(i_tile);

        ++i_tile;
    }

    // Apply Simulated Annealing
    tiles = find_best_tiles_setup(tiles, moveableTilesIndexes);

    // Flush new tiles to output file
    for (std::vector<tile>::iterator it = tiles.begin(); it != tiles.end(); ++it)
    {
        outputFile << (*it).up << (*it).left << (*it).right << (*it).down << "\n";
    }

    // Close files
    inputFile.close();
    outputFile.close();

    return 0;
}
