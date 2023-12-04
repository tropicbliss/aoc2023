#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

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
        token = trim(token);
        if (!token.empty())
        {
            result.push_back(token);
        }
    }
    return result;
}

std::vector<int> mapToInt(std::vector<std::string> strVec)
{
    std::vector<int> intVec;
    for (const auto &str : strVec)
    {
        intVec.push_back(std::stoi(str));
    }
    return intVec;
}

int getPoints(int matches)
{
    if (matches < 1)
    {
        return 0;
    }
    return pow(2, matches - 1);
}

int main()
{
    std::ifstream inputFile;
    inputFile.open("input.txt");
    std::string data;
    int totalSum = 0;
    while (std::getline(inputFile, data))
    {
        std::string cardInfo = split_trim(data, ':')[1];
        std::vector<std::string> temp = split_trim(cardInfo, '|');
        std::vector<int> winningNumbers = mapToInt(split_trim(temp[0], ' '));
        std::vector<int> numbersYouHave = mapToInt(split_trim(temp[1], ' '));
        int matches = 0;
        for (int num : numbersYouHave)
        {
            if (std::find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end())
            {
                matches++;
            }
        }
        int points = getPoints(matches);
        totalSum += points;
    }
    std::cout << totalSum << std::endl;
    return 0;
}