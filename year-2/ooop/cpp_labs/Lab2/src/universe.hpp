#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <cstddef>
#include <vector>

#include "cell.hpp"

namespace gol
{
  const size_t DEFAULT_HEIGHT = 10;
  const size_t DEFAULT_WIDTH = 10;

  class Universe
  {
  public:
    Universe();
    Universe(size_t height, size_t width);
    Universe(const Universe& universe);

    virtual ~Universe();

    Universe& operator=(const Universe& u);

    bool isActiveCell(size_t x, size_t y) const;
    void setCell(size_t x, size_t y, bool isActive);

    int countNeighbors(size_t x, size_t y) const;

    size_t height() const;
    size_t width() const;

  private:
    size_t _height;
    size_t _width;

    std::vector<Cell> _universe;

    void initUniverse();
  };
}  // namespace gol

#endif // UNIVERSE_HPP
