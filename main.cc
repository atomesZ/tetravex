#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct tile {
    char up;
    char left;
    char right;
    char down;
};

unsigned int E(const std::vector<tile>& s)
{
    unsigned int res = 0;

    

    return res;
}

void find_best_tiles_setup(std::vector<tile>& tiles, std::vector<int>& moveableTilesIndexes)
{
    std::vector<tile> s = tiles;
    std::vector<tile> g = tiles;

    unsigned int e = E(s);
    unsigned int m = e;
    unsigned int k = 0;

    
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
    find_best_tiles_setup(tiles, moveableTilesIndexes);

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
