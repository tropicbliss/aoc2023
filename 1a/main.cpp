#include <iostream>
#include <fstream>
#include <cctype>

int main()
{
    std::ifstream inputFile;
    inputFile.open("input.txt");
    std::string data;
    int totalSum = 0;
    while (inputFile >> data)
    {
        std::string first;
        std::string last;
        for (char c : data)
        {
            if (isdigit(c))
            {
                if (first.empty())
                {
                    first = c;
                }
                else
                {
                    last = c;
                }
            }
        }
        if (last.empty())
        {
            first.append(first);
        }
        else
        {
            first.append(last);
        }
        int number = std::stoi(first);
        totalSum += number;
    }
    std::cout << totalSum << std::endl;
    return 0;
}