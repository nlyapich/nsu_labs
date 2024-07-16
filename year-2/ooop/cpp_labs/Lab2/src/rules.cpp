#include "rules.hpp"

using namespace gol;

Rules::Rules()
{
  _birth.insert(3);
  _survival.insert(2);
  _survival.insert(3);
}

Rules::Rules(const std::set<int>& birth, const std::set<int>& survival) : _birth{birth}, _survival{survival} {}

Rules::Rules(const Rules& rules) : _birth{rules._birth}, _survival{rules._survival} {}

Rules::~Rules() {}

bool Rules::isActive(bool isActiveCell, int neighbors) const
{
  if (isActiveCell)
  {
    return (_survival.find(neighbors) != _survival.end());
  }
  else
  {
    return (_birth.find(neighbors) != _birth.end());
  }
}

std::set<int> Rules::birth() const
{
    return _birth;
}

std::set<int> Rules::survival() const
{
    return _survival;
}
