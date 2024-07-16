#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "./../src/gameOfLife.hpp"

using namespace gol;

struct TestCase
{
  std::string input;
  std::string expectedOutput;
  int iterationCount;
};

const std::vector<TestCase> mechanicsTestCase =
{
  {
{
"#N Life 1.06\n\
#S 10 10\n\
#R B3/S23\n\
#C\n\
.**.......\n\
*.*.......\n\
.**.......\n\
..........\n\
..........\n\
..........\n\
..........\n\
..........\n\
..........\n\
.*.*......\n"
},
{
"#N Life 1.06\n\
#S 10 10\n\
#R B3/S23\n\
#C\n\
...***.***\n\
...**...**\n\
****.....*\n\
***.......\n\
.*........\n\
..........\n\
.*........\n\
***.......\n\
****.....*\n\
...**...**\n"
},
10
  },
  {
{
"#N Life 1.06\n\
#S 11 11\n\
#R B23/S234\n\
#C\n\
.**........\n\
*.*........\n\
.**........\n\
...**......\n\
..*.*......\n\
....*......\n\
...........\n\
...........\n\
...........\n\
.*.*.......\n\
..*.*......\n"
},
{
"#N Life 1.06\n\
#S 11 11\n\
#R B23/S234\n\
#C\n\
*.*..**....\n\
*****......\n\
****.*....*\n\
....**.....\n\
***.......*\n\
***.*..****\n\
.......**.*\n\
********..*\n\
....*.**.**\n\
*..*.*****.\n\
*.*.*.*.*..\n"
},
5
  },
  {
{
"#N Life 1.06\n\
#S 11 11\n\
#R B3/S23\n\
#C\n\
...........\n\
...***.....\n\
...........\n\
...........\n\
...........\n\
...***.....\n\
...........\n\
...........\n\
........***\n\
...........\n\
...........\n"
},
{
"#N Life 1.06\n\
#S 11 11\n\
#R B3/S23\n\
#C\n\
....*......\n\
....*......\n\
....*......\n\
...........\n\
....*......\n\
....*......\n\
....*......\n\
.........*.\n\
.........*.\n\
.........*.\n\
...........\n"
},
1
  }
};

TEST(mechanicsTest, outputTest)
{
  auto mechanicsTestFunction = [](const std::string& input, const std::string& expectedOutput, int iterationCount)
  {
    EXPECT_TRUE(true);
    std::stringstream inputStream{input};
    GameOfLife game{inputStream};

    for (int i = 0; i < iterationCount; ++i)
    {
      game.nextState();
    }

    std::stringstream outputStream;
    game.writeInStream(outputStream);

    std::stringstream expectedOutputStream{expectedOutput};

    std::string tmp1, tmp2;
    while ((outputStream >> tmp1) && (expectedOutputStream >> tmp2))
    {
      EXPECT_EQ(tmp1, tmp2);
    }
  };

  for (auto [input, expectedOutput, iterationCount] : mechanicsTestCase)
  {
    mechanicsTestFunction(input, expectedOutput, iterationCount);
  }
}
