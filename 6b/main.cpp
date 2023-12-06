#include <vector>
#include <iostream>
#include <fstream>
#include <fmt/core.h>
#include "absl/strings/match.h"
#include <sstream>
#include <tuple>
#include <cmath>
#include "absl/time/time.h"
#include "absl/time/clock.h"
#include "absl/strings/str_join.h"

void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

std::vector<std::string> splitAndTrim(const std::string &str, char delim)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delim))
    {
        trim(token);
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
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

std::tuple<int, int> getTimeHeld(long distance, long raceDuration)
{
    int a = -1;
    long b = raceDuration;
    long c = -distance;
    long discriminant = b * b - 4 * a * c;
    double root1 = (-b + sqrt(discriminant)) / (2 * a);
    double root2 = (-b - sqrt(discriminant)) / (2 * a);
    std::tuple<int, int> result;
    int roundedRoot1 = static_cast<int>(std::ceil(root1));
    int roundedRoot2 = static_cast<int>(std::floor(root2));
    if (root1 == roundedRoot1)
    {
        roundedRoot1++;
    }
    if (root2 == roundedRoot2)
    {
        roundedRoot2--;
    }
    std::get<0>(result) = roundedRoot1;
    std::get<1>(result) = roundedRoot2;
    return result;
}

int main()
{
    std::ifstream inputFile;
    inputFile.open("input.txt");
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
        fmt::println("Unable to open file");
        return 1;
    }
    absl::Time start = absl::Now();
    long times = 0;
    long distances = 0;
    for (const std::string &line : lines)
    {
        if (absl::StartsWith(line, "Time"))
        {
            auto split = splitAndTrim(line, ':');
            times = std::stol(absl::StrJoin(splitAndTrim(split[1], ' '), ""));
        }
        else if (absl::StartsWith(line, "Distance"))
        {
            auto split = splitAndTrim(line, ':');
            distances = std::stol(absl::StrJoin(splitAndTrim(split[1], ' '), ""));
        }
    }
    auto result = getTimeHeld(distances, times);
    int winningOutcomes = std::get<1>(result) - std::get<0>(result) + 1;
    absl::Time end = absl::Now();
    absl::Duration duration = end - start;
    fmt::println("{}", winningOutcomes);
    fmt::println("Time taken: {} us", absl::ToDoubleMicroseconds(duration));
    return 0;
}