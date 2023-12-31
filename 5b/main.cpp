#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstdint>
#include <limits>
#include <unordered_set>
#include <chrono>

struct Mapper
{
    uint32_t dstRangeStart;
    uint32_t srcRangeStart;
    uint32_t rangeLen;
};

uint32_t mapper(std::vector<Mapper> mappers, uint32_t input)
{
    for (const Mapper mapper : mappers)
    {
        if (input >= mapper.srcRangeStart && input < mapper.srcRangeStart + mapper.rangeLen)
        {
            uint32_t offset = input - mapper.srcRangeStart;
            uint32_t destination = mapper.dstRangeStart + offset;
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

std::vector<uint32_t> mapToInt(std::vector<std::string> strVec)
{
    std::vector<uint32_t> intVec;
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
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<Mapper>> mapperPipeline;
    std::unordered_set<uint32_t> startingSegments = {0};
    uint32_t minSeed = std::numeric_limits<uint32_t>::max();
    uint32_t maxSeed = 0; // inclusive
    int pipelineIndex = -1;
    for (const std::string &line : lines)
    {
        if (startsWith(line, "seeds:"))
        {
            std::vector<uint32_t> seedData = mapToInt(splitAndTrim(splitAndTrim(line, ':')[1], ' '));
            for (int seedIdx = 0; seedIdx < seedData.size() / 2; seedIdx++)
            {
                int actualIdx = seedIdx * 2;
                uint32_t startingSeed = seedData[actualIdx];
                uint32_t rangeLength = seedData[actualIdx + 1];
                uint32_t endingSeed = startingSeed + rangeLength - 1;
                if (startingSeed < minSeed)
                {
                    minSeed = startingSeed;
                }
                if (endingSeed > maxSeed)
                {
                    maxSeed = endingSeed;
                }
            }
        }
        else if (containsSubstring(line, "map"))
        {
            pipelineIndex++;
        }
        else if (line != "")
        {
            std::vector<uint32_t> rawMapper = mapToInt(splitAndTrim(line, ' '));
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
            uint32_t startOfChunk = mapper.srcRangeStart;
            uint32_t endOfChunkExclusive = mapper.srcRangeStart + mapper.rangeLen;
            startingSegments.insert(startOfChunk);
            startingSegments.insert(endOfChunkExclusive);
        }
    }
    uint32_t minLocNum = std::numeric_limits<uint32_t>::max();
    for (uint32_t seed : startingSegments)
    {
        if (seed >= minSeed && seed <= maxSeed)
        {
            uint32_t currentStage = seed;
            for (const std::vector<Mapper> mappers : mapperPipeline)
            {
                currentStage = mapper(mappers, currentStage);
            }
            if (currentStage < minLocNum)
            {
                minLocNum = currentStage;
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << minLocNum << std::endl;
    std::chrono::duration<double, std::micro> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " us\n";
    return 0;
}