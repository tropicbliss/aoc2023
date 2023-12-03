#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

enum State
{
    NOT_A_NUMBER,
    START_OF_NUMBER,
    SOLE_DIGIT,
    MID_OF_NUMBER,
    END_OF_NUMBER,
    IS_ADJACENT
};

void exit_with_error()
{
    std::cout << "Unreachable point" << std::endl;
    exit(1);
}

int maxRow = 0;
int maxCol = 0;

bool bounds_check(int row, int col)
{
    if (row < 0 || row >= maxRow)
    {
        return false;
    }
    if (col < 0 || col >= maxCol)
    {
        return false;
    }
    return true;
}

int main()
{
    std::ifstream inputFile("input.txt");
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
    maxRow = lines.size();
    maxCol = lines[0].length();
    int totalSum = 0;
    int currRow = 0;
    for (const std::string &data : lines)
    {
        State currentState = NOT_A_NUMBER;
        int currCol = 0;
        std::string buffer;
        for (char c : data)
        {
            State previousState = currentState;
            if (isdigit(c))
            {
                buffer += c;
                switch (previousState)
                {
                case NOT_A_NUMBER:
                    if (currCol + 1 == data.length() || !isdigit(data[currCol + 1]))
                    {
                        currentState = SOLE_DIGIT;
                    }
                    else
                    {
                        currentState = START_OF_NUMBER;
                    }
                    break;
                case START_OF_NUMBER:
                case MID_OF_NUMBER:
                    if (!bounds_check(currRow, currCol + 1) || !isdigit(data[currCol + 1]))
                    {
                        currentState = END_OF_NUMBER;
                    }
                    else
                    {
                        currentState = MID_OF_NUMBER;
                    }
                    break;
                case IS_ADJACENT:
                    break;
                case END_OF_NUMBER:
                    exit_with_error();
                }
            }
            else
            {
                buffer = "";
                currentState = NOT_A_NUMBER;
            }
            // S S M E E
            // S 1 2 3 E
            // S S M E E
            //
            // S S E E
            // S 1 3 E
            // S S E E
            std::vector<std::tuple<int, int>> offsets; // col, row
            switch (currentState)
            {
            case START_OF_NUMBER:
                offsets.push_back(std::make_tuple(0, -1));
                offsets.push_back(std::make_tuple(-1, -1));
                offsets.push_back(std::make_tuple(-1, 0));
                offsets.push_back(std::make_tuple(-1, 1));
                offsets.push_back(std::make_tuple(0, 1));
                break;
            case MID_OF_NUMBER:
                offsets.push_back(std::make_tuple(0, -1));
                offsets.push_back(std::make_tuple(0, 1));
                break;
            case END_OF_NUMBER:
                offsets.push_back(std::make_tuple(0, -1));
                offsets.push_back(std::make_tuple(1, -1));
                offsets.push_back(std::make_tuple(1, 0));
                offsets.push_back(std::make_tuple(1, 1));
                offsets.push_back(std::make_tuple(0, 1));
                break;
            case SOLE_DIGIT:
                offsets.push_back(std::make_tuple(0, -1));
                offsets.push_back(std::make_tuple(-1, -1));
                offsets.push_back(std::make_tuple(-1, 0));
                offsets.push_back(std::make_tuple(-1, 1));
                offsets.push_back(std::make_tuple(0, 1));
                offsets.push_back(std::make_tuple(1, 1));
                offsets.push_back(std::make_tuple(1, 0));
                offsets.push_back(std::make_tuple(1, -1));
                break;
            case IS_ADJACENT:
            case NOT_A_NUMBER:
                break;
            }
            for (std::tuple<int, int> offset : offsets)
            {
                int rowToCheck = std::get<1>(offset) + currRow;
                int colToCheck = std::get<0>(offset) + currCol;
                if (bounds_check(rowToCheck, colToCheck))
                {
                    char elem = lines[rowToCheck][colToCheck];
                    if (elem != '.' && !isdigit(elem))
                    {
                        currentState = IS_ADJACENT;
                        break;
                    }
                }
            }
            if (currentState == IS_ADJACENT && (!bounds_check(currRow, currCol + 1) || !isdigit(data[currCol + 1])))
            {
                // The last number is adjacent to symbols
                int number = std::stoi(buffer);
                totalSum += number;
                // std::cout << buffer << std::endl;
            }
            currCol++;
        }
        currRow++;
    }
    std::cout << totalSum << std::endl;
    return 0;
}