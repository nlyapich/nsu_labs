#include <iostream>
#include <tuple>
#include <fstream>

#include "CSVParser.hpp"

using namespace csv;

int main()
{
  std::ifstream input("test.csv");
  CSVParser<int, std::string> parser(input, 0 /*skip first lines count*/);
  for (std::tuple<int, std::string> rs : parser)
  {
    std::cout << rs << std::endl;
  }
}
