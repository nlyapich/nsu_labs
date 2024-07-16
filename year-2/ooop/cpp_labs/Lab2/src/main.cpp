#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <vector>

#include "gameOfLife.hpp"

using namespace gol;

struct InputData
{
    std::string outputFile;
    std::string inputFile;
    int iterationCount;
    bool hasUnknown;
};

void printHelp()
{
    std::cout << "Parameters:" << std::endl
              << "    -i <n> or --iterations=<n> - count of iterations." << std::endl
              << "    -in <filename> or --input=<file name> - input file." << std::endl
              << "    -o <filename> or --output=<filename> - output file." << std::endl;
}

InputData readParameters(int size, char** argv)
{
    InputData data {"", "", 0, false};
    for(int i = 1; i < size; i++)
    {
      if (std::string(argv[i]).find("-in") == 0)
      {
        if (++i < size)
        {
          data.inputFile = argv[i];
        }
        continue;
      }

      if (std::string(argv[i]).find("--input=") == 0)
      {
          data.inputFile = argv[i] + 8;
          continue;
      }

      if (std::string(argv[i]).find("-i") == 0)
      {
        if (++i < size)
        {
          try
          {
            data.iterationCount = std::stoi(argv[i]);
          }
          catch (const std::exception& e)
          {
            throw std::invalid_argument("Invalid argument with flag -i");
          }
        }
        else
        {
          std::cout << "No value for key -i" << std::endl;
        }
        continue;
      }

      if (std::string(argv[i]).find("--iterations=") == 0)
      {
        try
        {
          data.iterationCount = std::stoi(argv[i] + 13);
        }
        catch (const std::exception& e)
        {
          throw std::invalid_argument("Invalid argument with flag --iterations");
        }

        if(data.iterationCount <= 0)
        {
          std::cout << "Count of iterations not natural number" << std::endl;
        }
        continue;
      }
      if (std::string(argv[i]).find("-o") == 0)
      {
        if (++i < size)
        {
          data.outputFile = argv[i];
        }
        continue;
      }

      if (std::string(argv[i]).find("--output=") == 0)
      {
        data.outputFile = argv[i] + 9;
        continue;
      }

      data.hasUnknown = true;
    }

    return data;
}

void startOnline(const InputData& data)
{
  int iterationCount = data.iterationCount;

  std::ifstream input(data.inputFile);
  if (input.fail())
  {
    throw std::invalid_argument("invalid file");
  }

  GameOfLife game;

  try
  {
    game = GameOfLife{input};
  }
  catch (const std::exception& e)
  {
    throw std::invalid_argument("Invalid input file");
  }

  input.close();

  for (int i = 0; i < iterationCount; ++i)
  {
    game.nextState();
  }

  while(true)
  {
    std::string buffer;
    std::getline(std::cin, buffer);

    if (buffer == "")
    {
      continue;
    }

    std::vector <std::string> commands;
    std::string command;
    std::stringstream s(buffer);

    while (s >> command)
    {
      commands.push_back(command);
    }

    if(commands[0] == "exit")
    {
      break;
    }

    if(commands[0] == "tick" )
    {
      int n = 1;
      if(commands.size() > 1)
      {
        n = std::stoi(commands[1]);
      }

      for(int i = 0; i < n; ++i)
      {
        game.nextState();
        iterationCount++;
      }

      std::cout << "iteration number = " << iterationCount << std::endl;
      game.writeInStream(std::cout);
      continue;
    }

    if(commands[0] == "dump")
    {
      if(commands.size() == 1)
      {
        printHelp();
        continue;
      }

      std::ofstream output{commands[1]};
      if (output.fail())
      {
        std::cout << "Invalid output file" << std::endl;
      }

      game.writeInStream(output);

      continue;
    }

    if (commands[0] == "help")
    {
      printHelp();
    }
  }
}

// the correctness of the transmitted data is guaranteed
void startOfline(const InputData& data)
{
  std::ifstream input(data.inputFile);
  std::ofstream output(data.outputFile);
  if (input.fail() || output.fail())
  {
    throw std::invalid_argument("Invalid file");
  }

  try
  {
    GameOfLife game{input};

    for (int i = 0; i < data.iterationCount; ++i)
    {
      game.nextState();
    }

    game.writeInStream(output);
  }
  catch (const std::exception& e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }

  input.close();
  output.close();
}

int main(int argc, char** argv)
{
  InputData data;
  try
  {
    data = readParameters(argc, argv);
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << std::endl;
    return 0;
  }

  if(data.hasUnknown || (data.inputFile == "")) {
      printHelp();
      return 0;
  }

  bool isOnline = ((data.outputFile == "") || (data.iterationCount <= 0));

  try
  {
    if (isOnline)
    {
      startOnline(data);
    }
    else
    {
      startOfline(data);
    }
  }
  catch (const std::exception& e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }

  return 0;
}
