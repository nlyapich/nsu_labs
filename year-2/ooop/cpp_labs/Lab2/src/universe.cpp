#include "universe.hpp"

#include <stdexcept>

using namespace gol;

void Universe::initUniverse()
{
  for (size_t i = 0; i < _height; ++i)
  {
    for (size_t j = 0; j < _width; ++j)
    {
      _universe.push_back(Cell{false});
    }
  }
}

Universe::Universe() : _height{DEFAULT_HEIGHT}, _width{DEFAULT_WIDTH}
{
  _universe.reserve(_height * _width);
  initUniverse();
}

Universe::Universe(size_t height, size_t width) : _height{height}, _width{width}
{
  _universe.reserve(_height * _width);
  initUniverse();
}

Universe::Universe(const Universe& universe) : _height{universe._height}, _width{universe._width}
{
  _universe.reserve(_height * _width);
  initUniverse();
  for (size_t i = 0; i < _height; ++i)
  {
    for (size_t j = 0; j < _width; ++j)
    {
      setCell(i, j, universe.isActiveCell(i, j));
    }
  }
}

Universe::~Universe() {}

Universe& Universe::operator=(const Universe& u)
{
  if (this == &u)
  {
    return *this;
  }

  _height = u._height;
  _width = u._width;

  _universe.reserve(_height * _width);
  _universe.clear();
  initUniverse();

  for (size_t i = 0; i < _height; ++i)
  {
    for (size_t j = 0; j < _width; ++j)
    {
      setCell(i, j, u.isActiveCell(i, j));
    }
  }

  return *this;
}

bool Universe::isActiveCell(size_t x, size_t y) const
{
  if ((x >= _height) || (y >= _width))
    throw std::out_of_range("There is no cell with such coordinates");

  return _universe[x * _height + y].isActive();
}

void Universe::setCell(size_t x, size_t y, bool isActive)
{
  _universe[x * _height + y].setIsActive(isActive);
}

int Universe::countNeighbors(size_t x, size_t y) const
{
  if ((x >= _height) || (y >= _width))
    throw std::out_of_range("There is no cell with such coordinates");

  int neighbors = isActiveCell(x, (y == 0) ? (_width - 1) : (y - 1))
                + isActiveCell(x, (y == (_width - 1)) ? 0 : (y + 1))
                + isActiveCell((x == 0) ? (_height - 1) : (x - 1), (y == 0) ? (_width - 1) : (y - 1))
                + isActiveCell((x == 0) ? (_height - 1) : (x - 1), y)
                + isActiveCell((x == 0) ? (_height - 1) : (x - 1), (y == (_width - 1)) ? 0 : (y + 1))
                + isActiveCell((x == (_height - 1)) ? 0 : (x + 1), (y == 0) ? (_width - 1) : (y - 1))
                + isActiveCell((x == (_height - 1)) ? 0 : (x + 1), y)
                + isActiveCell((x == (_height - 1)) ? 0 : (x + 1), (y == (_width - 1)) ? 0 : (y + 1));

  return neighbors;
}

size_t Universe::height() const
{
  return _height;
}

size_t Universe::width() const
{
  return _width;
}
