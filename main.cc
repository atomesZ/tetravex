#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

struct tile {
    char up;
    char left;
    char right;
    char down;
};

inline const int get_x_dim(const std::vector<tile>& s)
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

inline const unsigned int num_wrong_links(const std::vector<tile>& s, const unsigned int i)
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

const std::vector<tile> voisin(const std::vector<tile>& s0, const std::vector<int>& moveableTilesIndexes)
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

std::vector<tile> find_best_tiles_setup(std::vector<tile>& s0, const std::vector<int>& moveableTilesIndexes)
{
    std::vector<tile> s = s0;
    std::vector<tile> g = s0;

    unsigned int e = E(s);
    unsigned int m = e;
    unsigned int k = 0;


    while (true)
    {
        const std::vector<tile> sn = voisin(s, moveableTilesIndexes);
        const unsigned int en = E(sn);

        if (en < e || (float) rand()/RAND_MAX < 0)//P(en -e, temp(k/kmax)))
        {
            s = sn;
            e = en;
        }
        
        if (e < m)
        {
            g = s;
            m = e;

            if (e == 0)
                return g;
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
    std::vector<tile>::iterator it = tiles.begin();
    outputFile << (*it).up << (*it).left << (*it).right << (*it).down;
    ++it;

    for (; it != tiles.end(); ++it)
    {
        outputFile << "\n" << (*it).up << (*it).left << (*it).right << (*it).down;
    }

    // Close files
    inputFile.close();
    outputFile.close();

    return 0;
}
