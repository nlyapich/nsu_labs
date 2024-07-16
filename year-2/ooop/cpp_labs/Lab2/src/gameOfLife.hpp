#ifndef GAMEOFLIFE_HPP
#define GAMEOFLIFE_HPP

#include <istream>
#include <ostream>

#include "rules.hpp"
#include "universe.hpp"

namespace gol
{
  class GameOfLife
  {
  public:
    GameOfLife();
    GameOfLife(const Universe& universe, const Rules& rules, const std::string& name);
    GameOfLife(std::istream& input);

    virtual ~GameOfLife();

    void nextState();

    void writeInStream(std::ostream& o);

  private:
    Universe _universe;
    Rules _rules;
    std::string _name;
  };

} // namespace gol

#endif // GAMEOFLIFE_HPP
