#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s)
{
    return rtrim(ltrim(s));
}

std::vector<std::string> split_trim(const std::string &s, const char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, delim))
    {
        result.push_back(trim(token));
    }
    return result;
}

int main()
{
    std::ifstream inputFile;
    inputFile.open("input.txt");
    std::string data;
    int totalSum = 0;
    int gameId = 1;
    const int TOTAL_RED = 12;
    const int TOTAL_GREEN = 13;
    const int TOTAL_BLUE = 14;
    while (std::getline(inputFile, data))
    {
        std::vector<std::string> colonSep = split_trim(data, ':');
        // SAFETY: fuck safety, assume input is always valid
        std::string gamesEachRound = colonSep[1];
        std::vector<std::string> games = split_trim(gamesEachRound, ';');
        bool isValid = true;
        for (const std::string &game : games)
        {
            int redCount = 0;
            int greenCount = 0;
            int blueCount = 0;
            std::vector<std::string> cubeInfos = split_trim(game, ',');
            for (const std::string &cubeInfo : cubeInfos)
            {
                std::vector<std::string> temp = split_trim(cubeInfo, ' ');
                std::string colour = temp[1];
                int count = std::stoi(temp[0]);
                if (colour == "red")
                {
                    redCount = count;
                }
                else if (colour == "green")
                {
                    greenCount = count;
                }
                else if (colour == "blue")
                {
                    blueCount = count;
                }
            }
            if (redCount > TOTAL_RED || greenCount > TOTAL_GREEN || blueCount > TOTAL_BLUE)
            {
                isValid = false;
                break;
            }
        }
        if (isValid)
        {
            totalSum += gameId;
        }
        gameId++;
    }
    std::cout << totalSum << std::endl;
    return 0;
}