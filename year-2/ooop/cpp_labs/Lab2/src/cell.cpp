#include "cell.hpp"

using namespace gol;

Cell::Cell() : _isActive{false} {}
Cell::Cell(bool isActive) : _isActive{isActive} {}
Cell::~Cell() {}

bool Cell::isActive() const
{
  return _isActive;
}

void Cell::setIsActive(bool isActive)
{
  _isActive = isActive;
}
