#include <iostream>
#include <fstream>
#include <optional>
#include <string_view>

std::optional<char> checkStringNumber(std::string_view sv)
{
    if (sv.length() < 3)
    {
        return std::nullopt;
    }
    // SAFETY: The string view passed should never be empty or have a single character at this point
    switch (sv[0])
    {
    case 'o':
        if (sv.substr(1, 2) == "ne")
        {
            return '1';
        }
        break;
    case 't':
        switch (sv[1])
        {
        case 'w':
            if (sv.substr(2, 1) == "o")
            {
                return '2';
            }
        case 'h':
            if (sv.substr(2, 3) == "ree")
            {
                return '3';
            }
        }
        break;
    case 'f':
    {
        std::string_view substring = sv.substr(1, 3);
        if (substring == "our")
        {
            return '4';
        }
        if (substring == "ive")
        {
            return '5';
        }
        break;
    }
    case 's':
        switch (sv[1])
        {
        case 'i':
            if (sv.substr(2, 1) == "x")
            {
                return '6';
            }
        case 'e':
            if (sv.substr(2, 3) == "ven")
            {
                return '7';
            }
        }
        break;
    case 'e':
        if (sv.substr(1, 4) == "ight")
        {
            return '8';
        }
        break;
    case 'n':
        if (sv.substr(1, 3) == "ine")
        {
            return '9';
        }
    }
    return std::nullopt;
}

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
        int idx = 0;
        for (char c : data)
        {
            char current = '\0';
            if (isdigit(c))
            {
                current = c;
            }
            else
            {
                std::optional<char> result = checkStringNumber(std::string_view(data).substr(idx));
                if (result.has_value())
                {
                    current = *result;
                }
            }
            if (current != '\0')
            {
                if (first.empty())
                {
                    first = current;
                }
                else
                {
                    last = current;
                }
            }
            idx++;
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