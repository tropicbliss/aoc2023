#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>

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

int main()
{
    std::ifstream inputFile;
    inputFile.open("input.txt");
    std::string data;
    int totalSum = 0;
    std::map<int, int> cardCounts;
    int currCardNumber = 1;
    std::vector<std::string> lines;
    std::string line;
    if (inputFile.is_open())
    {
        while (getline(inputFile, line))
        {
            lines.push_back(line);
        }
        inputFile.close();
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }
    int lastGame = lines.size();
    for (const std::string &data : lines)
    {
        cardCounts[currCardNumber]++; // add original
        std::string cardInfo = split_trim(data, ':')[1];
        std::vector<std::string> temp = split_trim(cardInfo, '|');
        std::vector<int> winningNumbers = mapToInt(split_trim(temp[0], ' '));
        std::vector<int> numbersYouHave = mapToInt(split_trim(temp[1], ' '));
        int matches = 0;
        int multiplier = cardCounts[currCardNumber];
        for (int num : numbersYouHave)
        {
            if (std::find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end())
            {
                matches++;
            }
        }
        for (int i = 1; i < matches + 1; i++)
        {
            int cardNumber = currCardNumber + i;
            if (cardNumber <= lastGame)
            {
                cardCounts[cardNumber] += multiplier;
            }
        }
        currCardNumber++;
    }
    for (const auto &pair : cardCounts)
    {
        int cardCount = pair.second;
        totalSum += cardCount;
    }
    std::cout << totalSum << std::endl;
    return 0;
}