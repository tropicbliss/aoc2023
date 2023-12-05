#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstdint>
#include <limits>

struct Mapper
{
    uint64_t dstRangeStart;
    uint64_t srcRangeStart;
    uint64_t rangeLen;
};

uint64_t mapper(std::vector<Mapper> mappers, uint64_t input)
{
    for (const Mapper mapper : mappers)
    {
        if (input >= mapper.srcRangeStart && input < mapper.srcRangeStart + mapper.rangeLen)
        {
            uint64_t offset = input - mapper.srcRangeStart;
            uint64_t destination = mapper.dstRangeStart + offset;
            return destination;
        }
    }
    return input;
}

inline bool startsWith(const std::string &str, const std::string &prefix)
{
    return str.find(prefix) == 0;
}

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

inline bool containsSubstring(const std::string &str, const std::string &substring)
{
    return str.find(substring) != std::string::npos;
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

std::vector<uint64_t> mapToInt(std::vector<std::string> strVec)
{
    std::vector<uint64_t> intVec;
    for (const auto &str : strVec)
    {
        intVec.push_back(std::stoull(str));
    }
    return intVec;
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
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }
    std::vector<uint64_t> seeds;
    std::vector<std::vector<Mapper>> mapperPipeline;
    int pipelineIndex = -1;
    for (const std::string &line : lines)
    {
        if (startsWith(line, "seeds:"))
        {
            seeds = mapToInt(splitAndTrim(splitAndTrim(line, ':')[1], ' '));
        }
        else if (containsSubstring(line, "map"))
        {
            pipelineIndex++;
        }
        else if (line != "")
        {
            std::vector<uint64_t> rawMapper = mapToInt(splitAndTrim(line, ' '));
            Mapper mapper;
            mapper.dstRangeStart = rawMapper[0];
            mapper.srcRangeStart = rawMapper[1];
            mapper.rangeLen = rawMapper[2];
            if (mapperPipeline.size() == pipelineIndex)
            {
                mapperPipeline.push_back({mapper});
            }
            else
            {
                mapperPipeline[pipelineIndex].push_back(mapper);
            }
        }
    }
    uint64_t minLocNum = std::numeric_limits<uint64_t>::max();
    for (const uint64_t seed : seeds)
    {
        uint64_t currentStage = seed;
        for (const std::vector<Mapper> mappers : mapperPipeline)
        {
            currentStage = mapper(mappers, currentStage);
        }
        uint64_t locationNumber = currentStage;
        if (locationNumber < minLocNum)
        {
            minLocNum = locationNumber;
        }
    }
    std::cout << minLocNum << std::endl;
    return 0;
}