#ifndef RULES_HPP
#define RULES_HPP

#include <set>

namespace gol
{
  class Rules
  {
  public:
    Rules();
    Rules(const std::set<int>& birth, const std::set<int>& survival);
    Rules(const Rules& rules);

    virtual ~Rules();

    bool isActive(bool isActiveCell, int neighbors) const;

    std::set<int> birth() const;
    std::set<int> survival() const;

  private:
    std::set<int> _birth;
    std::set<int> _survival;
  };
}  // namespace gol

#endif // RULES_HPP
