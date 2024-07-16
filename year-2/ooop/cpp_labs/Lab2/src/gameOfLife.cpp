#include "gameOfLife.hpp"

#include <iostream>
#include <cstring>
#include <set>
#include <vector>

using namespace gol;

GameOfLife::GameOfLife() : _universe{}, _rules{}, _name{"Life 1.06"} {}

GameOfLife::GameOfLife(const Universe& universe, const Rules& rules, const std::string& name) : _universe{universe}, _rules{rules}, _name{name} {}

GameOfLife::GameOfLife(std::istream& input)
{
  bool noName = true;
  bool noRules = true;
  bool noSize = true;

  size_t height = 0;
  size_t width = 0;

  if (input.fail()) {
    throw std::invalid_argument("invalid file");
  }

  std::string buffer;
  size_t countLines = 0;
  while (std::getline(input, buffer))
  {
    countLines++;
    size_t curPos = 0;
    if (buffer[curPos] == '#')
    {
      curPos++;
      switch (buffer[curPos])
      {
        case 'N':
        {
          curPos++;
          if (std::isspace(buffer[curPos])) {
            curPos++;
          }
          else
          {
            std::cout << "No space after #N" << std::endl;
          }
          _name = std::string(buffer.c_str() + curPos);

          if (noName)
          {
            noName = false;
          }
          else
          {
            std::cout << "Two names in one file" << std::endl;
          }
        }
          break;
        case 'R':
        {
          curPos++;
          if (std::isspace(buffer[curPos]))
          {
            curPos++;
            if(buffer[curPos] != 'B')
            {
              throw std::invalid_argument("Game rules must start from B");
            }
          }
          else
          {
            std::cout << "No space after #R" << std::endl;
          }

          curPos++;
          std::set<int> birth;
          for(; (curPos < buffer.size()) && std::isdigit(buffer[curPos]); ++curPos)
          {
            if(birth.find(buffer[curPos]  - '0') != birth.end())
            {
            std::cout << "Number" << buffer[curPos] - '0' << " is defined twice in first part of the rule" << std::endl;
            }

            birth.insert(buffer[curPos] - '0');
          }

          if (curPos >= buffer.size())
          {
            throw std::invalid_argument("Not full rule");
          }

          if (buffer[curPos] != '/')
          {
            throw std::invalid_argument("Parts of rule must divided by /");
          }
          ++curPos;

          if (curPos >= buffer.size())
          {
            throw std::invalid_argument("Not full rule");
          }
          if (buffer[curPos] != 'S')
          {
            throw std::invalid_argument("second part must started from S");
          }
          ++curPos;

          std::set<int> survival;
          for (; (curPos < buffer.size()) && std::isdigit(buffer[curPos]); ++curPos)
          {
            if (survival.find(buffer[curPos] - '0') != survival.end())
            {
              std::cout << "Number" << buffer[curPos] - '0' << " is defined twice in second part of the rule" << std::endl;
            }

            survival.insert(buffer[curPos] - '0');
          }

          _rules = Rules(birth, survival);

          if (noRules)
          {
            noRules = false;
          }
          else
          {
            std::cout << "Two rules in one file" << std::endl;
          }
        }
          break;
        case 'S':
        {
          curPos++;
          if (std::isspace(buffer[curPos]))
          {
            curPos++;
          }
          else
          {
            std::cout << "No space after #S" << std::endl;
          }
          height = std::stoi(buffer.c_str() + curPos);
          curPos += std::to_string(height).size() + 1;
          width = std::stoi(buffer.c_str() + curPos);

          if (noSize)
          {
            noSize = false;
          }
          else
          {
            std::cout << "Two field size in one file" << std::endl;
          }
        }
          break;
        case 'C':
        {
          if (noSize)
          {
            throw std::invalid_argument("Field size must be before the field");
          }
          if (!height || !width)
          {
            throw std::invalid_argument("Field size is zero value");
          }
          _universe = Universe(height, width);
          for (size_t i = 0; i < height; ++i)
          {
            if (input.eof())
            {
              throw std::invalid_argument("Field is not valid");
            }

            std::getline(input, buffer);

            for (size_t j = 0; j < width; ++j)
            {
              if (buffer[j] == '*')
              {
                _universe.setCell(i, j, true);
              }
              else if (buffer[j] != '.')
                throw std::invalid_argument("Field is not valid");
            }
          }
          break;
        }
        default:
          std::cout << "Unknown value type" << std::endl;
          break;
      }
    }
  }

  if (noRules)
      std::cout << "No game rules, use default" << std::endl;
  if (noName)
      std::cout << "No world name, use file name as name" << std::endl;
  if (noSize)
      std::cout << "No field size, use, for example, field 16*16" << std::endl;
}

GameOfLife::~GameOfLife() {}

void GameOfLife::nextState()
{
  size_t height = _universe.height();
  size_t width = _universe.width();

  std::vector<Cell> newState;
  newState.reserve(height * width);
  for (size_t i = 0; i < (height * width); ++i)
  {
    Cell a{};
    newState.push_back(a);
  }

  for (size_t i = 0; i < height; ++i)
  {
    for (size_t j = 0; j < width; ++j)
    {
      int neighbors = _universe.countNeighbors(i, j);

      bool updatedCurCell = _rules.isActive
      (
        _universe.isActiveCell(i, j),
        neighbors
      );

      newState[i * height + j].setIsActive(updatedCurCell);
    }
  }

  for (size_t i = 0; i < height; ++i)
  {
    for (size_t j = 0; j < width; ++j)
    {
      _universe.setCell(i, j, newState[i * height + j].isActive());
    }
  }
}

void GameOfLife::writeInStream(std::ostream& o)
{
  o << "#N " << _name << std::endl;
	o << "#S " << _universe.height() << " " << _universe.width() << std::endl;
	o << "#R ";
  o << "B";
  for (auto birth : _rules.birth())
  {
    o << birth;
  }
  o << "/S";
  for (auto survival : _rules.survival())
  {
    o << survival;
  }
  o << std::endl;

  o << "#C" << std::endl;
  for (size_t i = 0; i < _universe.height(); ++i)
  {
    for (size_t j = 0; j < _universe.width(); ++j)
    {
      if (_universe.isActiveCell(i, j))
      {
        o << "*";
      }
      else
      {
        o << ".";
      }
    }
    o << std::endl;
  }
}
